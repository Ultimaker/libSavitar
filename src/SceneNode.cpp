// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/SceneNode.h"
#include "Savitar/Namespace.h"

#include <pugixml.hpp>

using namespace Savitar;

std::string SceneNode::getTransformation()
{
    return transformation_;
}

void SceneNode::setTransformation(std::string transformation)
{
    transformation_ = transformation;
}

SceneNode* SceneNode::getMeshNode()
{
    return mesh_node_;
}

std::string SceneNode::getComponentPath() const
{
    return component_path_;
}

std::vector<SceneNode*> SceneNode::getChildren()
{
    return children_;
}

bool SceneNode::addChild(SceneNode* node)
{
    if (node == nullptr) // No node given
    {
        return false;
    }

    if (! mesh_data_.getVertices().empty()) // This node already has mesh data, so we need to move that data to a child node
    {
        mesh_node_ = new SceneNode();
        mesh_node_->setMeshData(this->mesh_data_); // Copy the data to the new child.
        mesh_data_.clear(); // Clear our own data
        children_.push_back(mesh_node_);
    }

    children_.push_back(node);
    return true;
}

MeshData& SceneNode::getMeshData()
{
    return mesh_data_;
}

void SceneNode::setMeshData(const MeshData& mesh_data)
{
    mesh_data_ = mesh_data;
}

std::string SceneNode::getType()
{
    return type_;
}

void SceneNode::setType(const std::string& type)
{
    if (type == "model" || type == "solidsupport" || type == "support" || type == "surface" || type == "other")
    {
        type_ = type;
    }
}

void SceneNode::fillByXMLNode(pugi::xml_node xml_node)
{
    settings_.clear();
    id_ = xml_node.attribute("id").as_string();
    name_ = xml_node.attribute("name").as_string();

    if (xml_node.child("mesh") != nullptr)
    {
        mesh_data_.clear();
        mesh_data_.fillByXMLNode(xml_node.child("mesh"));
    }

    // Read settings the old way -which didn't conform  to the 3MF standard- for backwards compat.:
    const pugi::xml_node settings_node = xml_node.child("settings");
    if (settings_node != nullptr)
    {
        for (pugi::xml_node setting = settings_node.child("setting"); setting != nullptr; setting = setting.next_sibling("setting"))
        {
            const std::string key = setting.attribute("key").as_string();
            const std::string value = setting.text().as_string();
            setSetting(key, value);
        }
    }

    // Read settings the conformant way:
    const pugi::xml_node metadatagroup_node = xml_node.child("metadatagroup");
    if (metadatagroup_node != nullptr)
    {
        for (pugi::xml_node setting = metadatagroup_node.child("metadata"); setting != nullptr; setting = setting.next_sibling("metadata"))
        {
            // NOTE: In theory this could be slow, since we look up the entire ancestry tree for each setting.
            //       In practice it's expected to be negligible compared to the parsing of the mesh.
            const xml_namespace::xmlns_map_t namespaces = xml_namespace::getAncestralNamespaces(setting);
            std::set<std::string> cura_equivalent_namespaces = xml_namespace::getNamesFor(namespaces, xml_namespace::getCuraUri());
            cura_equivalent_namespaces.insert("cura"); // <- Just to be sure: If there was ever a version out that uses 'cura' without the specification of the xmlns-id.

            std::string key = setting.attribute("name").as_string();
            const size_t pos = key.find_first_of(':');

            // Make 'cura' namespace behave like the default.
            if (pos != std::string::npos && cura_equivalent_namespaces.count(key.substr(0, pos)) == 1)
            {
                key = key.substr(pos + 1);
            }
            const std::string value = setting.text().as_string();
            std::string type = setting.attribute("type").as_string();
            if (type.empty())
            {
                type = "xs:string";
            }
            std::string preserve_str = setting.attribute("preserve").as_string(); // as_bool is too strict. Any non-zero value evaluates as true. Parse this ourselves.
            const bool preserve = (! preserve_str.empty() && preserve_str != "0");
            setSetting(key, value, type, preserve);
        }
    }

    // Read components
    const pugi::xml_node components_node = xml_node.child("components");
    if (components_node != nullptr)
    {
        for (pugi::xml_node component = components_node.child("component"); component != nullptr; component = component.next_sibling("component"))
        {
            std::string path = component.attribute("p:path").as_string();
            if (! path.empty())
            {
                component_path_ = path;
            }

            std::string transformation = component.attribute("transform").as_string();
            if (! transformation.empty())
            {
                transformation_ = transformation;
            }
        }
    }
}

void SceneNode::parseComponentData(const std::string& xml_string)
{
    pugi::xml_document document;
    document.load_string(xml_string.c_str());

    pugi::xml_node xml_node = document;
    for (const std::string child_name : {"model", "resources", "object", "mesh"})
    {
        xml_node = xml_node.child(child_name.c_str());
        if (xml_node == nullptr)
        {
            return;
        }
    }

    mesh_data_.clear();
    mesh_data_.fillByXMLNode(xml_node);
}

std::string SceneNode::getPath()
{
    return path_;
}

void SceneNode::setPath(std::string path)
{
    path_ = path;
}

std::string SceneNode::getId()
{
    return id_;
}

void SceneNode::setId(std::string id)
{
    id_ = id;
}

std::string SceneNode::getName()
{
    return name_;
}

void SceneNode::setName(std::string name)
{
    name_ = name;
}

const std::map<std::string, MetadataEntry>& SceneNode::getSettings() const
{
    return settings_;
}

void SceneNode::setSetting(const std::string& key, const MetadataEntry& entry)
{
    settings_.emplace(key, entry);
}

void SceneNode::setSetting(const std::string& key, const std::string& value, const std::string& type, const bool preserve)
{
    settings_.emplace(key, MetadataEntry(value, type, preserve));
}

void SceneNode::removeSetting(std::string key)
{
    settings_.erase(key);
}


std::vector<SceneNode*> SceneNode::getAllChildren()
{
    std::vector<SceneNode*> all_children;

    for (SceneNode* scene_node : children_)
    {
        std::vector<SceneNode*> temp_children = scene_node->getAllChildren();
        all_children.insert(all_children.end(), temp_children.begin(), temp_children.end());
    }

    // Add all direct children_ to the result to return.
    // We put them at the end so that the "simplicity" rule of 3MF is kept:
    // "In keeping with the use of a simple parser, producers MUST define objects prior to referencing them as components."
    all_children.insert(all_children.end(), children_.begin(), children_.end());
    return all_children;
}
