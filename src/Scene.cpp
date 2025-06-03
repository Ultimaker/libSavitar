// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/Scene.h"
#include "Savitar/Namespace.h"

#include <algorithm>
#include <iostream>
#include <string>

using namespace Savitar;

std::vector<SceneNode*> Scene::getSceneNodes()
{
    return scene_nodes_;
}

void Scene::addSceneNode(SceneNode* node)
{
    if (node == nullptr)
    {
        return;
    }
    scene_nodes_.push_back(node);
}


void Scene::fillByXMLNode(pugi::xml_node xml_node)
{
    unit_ = xml_node.attribute("unit").as_string();

    // Handle metadata:
    for (pugi::xml_node metadata_node = xml_node.child("metadata"); metadata_node; metadata_node = metadata_node.next_sibling("metadata"))
    {
        const std::string key = metadata_node.attribute("name").as_string();
        const std::string value = metadata_node.text().as_string();
        std::string type = metadata_node.attribute("type").as_string();
        if (type.empty())
        {
            type = "xs:string"; // Fill in the default type if it's not present.
        }
        const std::string preserve_str = metadata_node.attribute("preserve").as_string(); // Don't use as_bool since 3MF's boolean parsing is more lenient.
        const bool preserve = (! preserve_str.empty() && preserve_str != "0");
        setMetaDataEntry(key, value, type, preserve);
    }

    // Handle UV coordinates, which are stored outside the objects
    pugi::xml_node resources = xml_node.child("resources");
    texture_data_.fillByXMLNode(resources);

    pugi::xml_node build = xml_node.child("build");
    for (pugi::xml_node item = build.child("item"); item != nullptr; item = item.next_sibling("item"))
    {
        // Found an item in the build. The items are linked to objects by objectid.
        pugi::xml_node object_node = resources.find_child_by_attribute("object", "id", item.attribute("objectid").value());
        if (object_node != nullptr)
        {
            SceneNode* temp_scene_node = createSceneNodeFromObject(xml_node, object_node);
            temp_scene_node->setTransformation(item.attribute("transform").as_string());

            // Get all metadata from the item and update that.
            const pugi::xml_node metadatagroup_node = item.child("metadatagroup");
            if (metadatagroup_node != nullptr)
            {
                for (pugi::xml_node setting = metadatagroup_node.child("metadata"); setting != nullptr; setting = setting.next_sibling("metadata"))
                {
                    const std::string key = setting.attribute("name").as_string();
                    const std::string value = setting.text().as_string();
                    std::string type = setting.attribute("type").as_string();
                    if (type.empty()) // Not specified.
                    {
                        type = "xs:string";
                    }
                    const std::string preserve_str = setting.attribute("preserve").as_string(); // Needs to be true if string is not "0", which is less strict than .as_bool();
                    const bool preserve = (! preserve_str.empty() && preserve_str != "0");
                    temp_scene_node->setSetting(key, value, type, preserve);
                }
            }

            scene_nodes_.push_back(temp_scene_node);
        }
        else
        {
            // TODO: add proper error handling
            std::cout << "Could not find object by given ID" << std::endl;
        }
    }
}

void Scene::toXmlNode(pugi::xml_node& model_node)
{
    pugi::xml_node resources_node = model_node.append_child("resources");
    pugi::xml_node build_node = model_node.append_child("build");

    model_node.append_attribute("unit") = getUnit().c_str();
    model_node.append_attribute("xmlns") = xml_namespace::getDefaultUri().c_str();
    model_node.append_attribute("xmlns:cura") = xml_namespace::getCuraUri().c_str();
    model_node.append_attribute("xml:lang") = "en-US";

    texture_data_.toXmlNode(resources_node);

    const std::vector<SceneNode*> all_nodes = getAllSceneNodes();
    for (SceneNode* scene_node : all_nodes)
    {
        scene_node->setId(getNextAvailableResourceId());
    }

    for (SceneNode* scene_node : all_nodes)
    {
        // Create item
        pugi::xml_node object = resources_node.append_child("object");
        object.append_attribute("id") = std::to_string(scene_node->getId()).c_str();
        if (! scene_node->getName().empty())
        {
            object.append_attribute("name") = scene_node->getName().c_str();
        }
        object.append_attribute("type") = scene_node->getType().c_str();

        const std::map<std::string, MetadataEntry>& per_object_settings = scene_node->getSettings();
        if (! per_object_settings.empty())
        {
            pugi::xml_node settings = object.append_child("metadatagroup");
            for (const auto& setting_pair : per_object_settings)
            {
                pugi::xml_node setting = settings.append_child("metadata");
                setting.append_attribute("name") = setting_pair.first.c_str();
                setting.text().set(setting_pair.second.value.c_str());
                if (setting_pair.second.type != "xs:string") // xs:string is the default type and doesn't need to be written.
                {
                    setting.append_attribute("type") = setting_pair.second.type.c_str();
                }
                if (setting_pair.second.preserve)
                {
                    setting.append_attribute("preserve") = "true";
                }
            }
        }

        if (! scene_node->getMeshData().getVertices().empty())
        {
            pugi::xml_node mesh = object.append_child("mesh");
            scene_node->getMeshData().toXmlNode(mesh);
        }

        if (! scene_node->getChildren().empty())
        {
            pugi::xml_node components = object.append_child("components");
            for (SceneNode* child_scene_node : scene_node->getChildren())
            {
                pugi::xml_node component = components.append_child("component");
                component.append_attribute("objectid") = std::to_string(child_scene_node->getId()).c_str();
                component.append_attribute("transform") = child_scene_node->getTransformation().c_str();
            }
        }
        if (scene_node->getMeshNode() != nullptr)
        {
            if (! object.child("metadatagroup"))
            {
                object.append_child("metadatagroup");
            }
            pugi::xml_node mesh_node_setting = object.child("metadatagroup").append_child("metadata");
            mesh_node_setting.append_attribute("name") = "mesh_node_objectid";
            mesh_node_setting.text().set(std::to_string(scene_node->getMeshNode()->getId()).c_str());
            mesh_node_setting.append_attribute("preserve") = "true";
        }
    }

    for (SceneNode* scene_node : getSceneNodes())
    {
        pugi::xml_node item = build_node.append_child("item");
        item.append_attribute("objectid") = std::to_string(scene_node->getId()).c_str();
        item.append_attribute("transform") = scene_node->getTransformation().c_str();
    }

    for (const auto& metadata_pair : getMetadata())
    {
        pugi::xml_node metadata_node = model_node.append_child("metadata");
        metadata_node.append_attribute("name") = metadata_pair.first.c_str();
        metadata_node.text().set(metadata_pair.second.value.c_str());
        if (metadata_pair.second.type != "xs:string") // xs:string is the default and doesn't need to get written then.
        {
            metadata_node.append_attribute("type") = metadata_pair.second.type.c_str();
        }
        if (metadata_pair.second.preserve)
        {
            metadata_node.append_attribute("preserve") = "true";
        }
    }
}

SceneNode* Scene::createSceneNodeFromObject(pugi::xml_node root_node, pugi::xml_node object_node)
{
    pugi::xml_node components = object_node.child("components");
    auto* scene_node = new SceneNode();
    scene_node->fillByXMLNode(object_node);

    std::map<std::string, std::string>::iterator it;
    const bool has_mesh_node = scene_node->getSettings().find("mesh_node_objectid") != scene_node->getSettings().end();

    std::string mesh_node_object_id;

    if (has_mesh_node)
    {
        mesh_node_object_id = scene_node->getSettings().at("mesh_node_objectid").value;
    }

    // We have to do the checking for children outside of the SceneNode creation itself, because it only has references.
    if (components != nullptr)
    {
        SceneNode* mesh_node = nullptr;
        for (pugi::xml_node component = components.child("component"); component != nullptr; component = component.next_sibling("component"))
        {
            // This node has children. Add them one by one.
            pugi::xml_node child_object_node = root_node.child("resources").find_child_by_attribute("object", "id", component.attribute("objectid").value());
            if (child_object_node != nullptr)
            {
                SceneNode* child_node = createSceneNodeFromObject(root_node, child_object_node);
                if (has_mesh_node && mesh_node_object_id == component.attribute("objectid").as_string())
                {
                    // Don't add a node with the mesh_node_objectid metadata. Store it until last so we can copy it's mesh to the parent node
                    mesh_node = child_node;
                }
                else
                {
                    child_node->setTransformation(component.attribute("transform").as_string());
                    scene_node->addChild(child_node);
                }
            }
            else
            {
                // TODO: ADD proper error handling here.
                std::cout << "Child_object_node not found :( " << std::endl;
            }
        }

        if (mesh_node != nullptr)
        {
            scene_node->setMeshData(mesh_node->getMeshData());
            scene_node->removeSetting("mesh_node_objectid"); // No need to keep it. It's job is done!
            delete mesh_node;
            mesh_node = nullptr;
        }
    }
    return scene_node;
}

int Scene::getNextAvailableResourceId() const
{
    int id = 0;

    std::vector<SceneNode*> all_nodes = getAllSceneNodes();
    const auto iterator_max = std::max_element(all_nodes.begin(), all_nodes.end(), [](const SceneNode* lhs, const SceneNode* rhs) { return lhs->getId() < rhs->getId(); });
    if (iterator_max != all_nodes.end())
    {
        id = (*iterator_max)->getId() + 1;
    }

    id = std::max(id, texture_data_.getNextAvailableResourceId());

    return id;
}

std::vector<SceneNode*> Scene::getAllSceneNodes() const
{
    std::vector<SceneNode*> all_nodes;

    for (SceneNode* scene_node : scene_nodes_)
    {
        std::vector<SceneNode*> temp_children = scene_node->getAllChildren();
        all_nodes.insert(all_nodes.end(), temp_children.begin(), temp_children.end());
    }

    // We put them at the end so that the "simplicity" rule of 3MF is kept:
    // "In keeping with the use of a simple parser, producers MUST define objects prior to referencing them as components."
    all_nodes.insert(all_nodes.end(), scene_nodes_.begin(), scene_nodes_.end());
    return all_nodes;
}

void Scene::setMetaDataEntry(const std::string& key, const MetadataEntry& entry)
{
    metadata_.emplace(key, entry);
}

void Scene::setMetaDataEntry(const std::string& key, const std::string& value, const std::string& type, const bool preserve)
{
    metadata_.emplace(key, MetadataEntry(value, type, preserve));
}

std::string Scene::getUnit()
{
    return unit_;
}

const std::map<std::string, MetadataEntry>& Scene::getMetadata() const
{
    return metadata_;
}

void Scene::setUnit(std::string unit)
{
    unit_ = unit;
}

std::string Scene::getTexturePathFromGroupId(const int uv_group_id) const
{
    return texture_data_.getTexturePathFromGroupId(uv_group_id);
}

const TextureData::UVCoordinatesGroup* Scene::getUVCoordinatesGroup(const int uv_group_id) const
{
    return texture_data_.getUVCoordinatesGroup(uv_group_id);
}

void Scene::addTexturePath(const std::string& texture_path, const int texture_id)
{
    texture_data_.addTexturePath(texture_path, texture_id);
}

void Scene::setUVCoordinatesGroupFromBytes(const bytearray& data, const int texture_id, const int group_id)
{
    texture_data_.setUVCoordinatesGroupFromBytes(data, texture_id, group_id);
}
