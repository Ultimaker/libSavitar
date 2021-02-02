/*
 * This file is part of libSavitar
 *
 * Copyright (C) 2021 Ultimaker B.V. <j.vankessel@ultimaker.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Scene.h"
#include "../pugixml/src/pugixml.hpp"
#include <iostream>
#include <string>
using namespace Savitar;

Scene::Scene()
{
    unit = "millimeter";
}

Scene::~Scene()
{

}

std::vector< SceneNode*> Scene::getSceneNodes()
{
    return this->scene_nodes;
}

void Scene::addSceneNode(SceneNode* node)
{
    if(node == nullptr)
    {
        return;
    }
    this->scene_nodes.push_back(node);
}


void Scene::fillByXMLNode(pugi::xml_node xml_node)
{
    unit = xml_node.attribute("unit").as_string();

    pugi::xml_node resources = xml_node.child("resources");

    // Handle metadata:
    for(pugi::xml_node metadata_node = xml_node.child("metadata"); metadata_node; metadata_node = metadata_node.next_sibling("metadata"))
    {
        setMetaDataEntry(metadata_node.attribute("name").as_string(), metadata_node.text().as_string());
    }

    pugi::xml_node build = xml_node.child("build");
    for(pugi::xml_node item = build.child("item"); item; item = item.next_sibling("item"))
    {
        // Found a item in the build. The items are linked to objects by objectid.
        pugi::xml_node object_node = resources.find_child_by_attribute("object", "id", item.attribute("objectid").value());
        if(object_node)
        {
            SceneNode* temp_scene_node = createSceneNodeFromObject(xml_node, object_node);
            temp_scene_node->setTransformation(item.attribute("transform").as_string());
            
            // Get all metadata from the item and update that.
            const pugi::xml_node metadatagroup_node = item.child("metadatagroup");
            if (metadatagroup_node)
            {
                for (pugi::xml_node setting = metadatagroup_node.child("metadata"); setting; setting = setting.next_sibling("metadata"))
                {
                    const std::string key = setting.attribute("name").as_string();
                    const std::string value = setting.text().as_string();
                    std::string type = setting.attribute("type").as_string();
                    if(type == "") //Not specified.
                    {
                        type = "xs:string";
                    }
                    const std::string preserve_str = setting.attribute("preserve").as_string(); //Needs to be true if string is not "0", which is less strict than .as_bool();
                    const bool preserve = (preserve_str != "" && preserve_str != "0");
                    temp_scene_node->setSetting(key, value, type, preserve);
                }
            }
            
            scene_nodes.push_back(temp_scene_node);
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
    SceneNode* scene_node = new SceneNode();
    scene_node->fillByXMLNode(object_node);
    
    std::map<std::string, std::string>::iterator it;
    const bool has_mesh_node = scene_node->getSettings().find("mesh_node_objectid") != scene_node->getSettings().end();
    
    std::string mesh_node_object_id = "";
    
    if(has_mesh_node)
    {
        mesh_node_object_id = scene_node->getSettings().at("mesh_node_objectid").value;
    }

    // We have to do the checking for children outside of the SceneNode creation itself, because it only has references.
    if(components)
    {
        
        SceneNode* mesh_node = nullptr;
        for(pugi::xml_node component = components.child("component"); component; component = component.next_sibling("component"))
        {
            // This node has children. Add them one by one.
            pugi::xml_node child_object_node = root_node.child("resources").find_child_by_attribute("object", "id", component.attribute("objectid").value());
            if(child_object_node)
            {
                SceneNode* child_node = createSceneNodeFromObject(root_node, child_object_node);
                if(has_mesh_node && mesh_node_object_id == component.attribute("objectid").as_string())
                {
                    // Don't add a node with the mesh_node_objectid metadata. Store it until last so we can copy it's mesh to the parent node
                    mesh_node = child_node;
                } else
                {
                    child_node->setTransformation(component.attribute("transform").as_string());
                    scene_node->addChild(child_node);
                }
            } else
            {
                // TODO: ADD proper error handling here.
                std::cout << "Child_object_node not found :( " << std::endl;
            }
        }
        
        if(mesh_node != nullptr)
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
    
    for(SceneNode* scene_node: scene_nodes)
    {
        std::vector<SceneNode*> temp_children = scene_node->getAllChildren();
        all_nodes.insert(all_nodes.end(), temp_children.begin(), temp_children.end());
    }
    
    // We put them at the end so that the "simplicity" rule of 3MF is kept:
    // "In keeping with the use of a simple parser, producers MUST define objects prior to referencing them as components."
    all_nodes.insert(all_nodes.end(), scene_nodes.begin(), scene_nodes.end());
    return all_nodes;
}

void Scene::setMetaDataEntry(const std::string& key, const std::string& value, const std::string& type, const bool preserve)
{
    metadata.emplace(key, MetadataEntry(value, type, preserve));
}

std::string Scene::getUnit()
{
    return unit;
}

const std::map<std::string, MetadataEntry>& Scene::getMetadata() const
{
    return metadata;
}

void Scene::setUnit(std::string unit)
{
    this->unit = unit;
}
