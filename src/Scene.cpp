// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/Scene.h"
#include <iostream>
#include <pugixml.hpp>
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

    pugi::xml_node resources = xml_node.child("resources");

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

    pugi::xml_node build = xml_node.child("build");
    for (pugi::xml_node item = build.child("item"); item != nullptr; item = item.next_sibling("item"))
    {
        // Found a item in the build. The items are linked to objects by objectid.
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

std::vector<SceneNode*> Scene::getAllSceneNodes()
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
