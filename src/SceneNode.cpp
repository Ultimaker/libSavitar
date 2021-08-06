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

#include "SceneNode.h"
#include "Namespace.h"
#include <pugixml.hpp>
#include <iostream>
using namespace Savitar;

SceneNode::SceneNode()
{
    transformation = "1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0";
    type = "model";
    mesh_node = nullptr;
}

SceneNode::~SceneNode()
{

}

std::string SceneNode::getTransformation()
{
    return this->transformation;
}

void SceneNode::setTransformation(std::string transformation)
{
    this->transformation = transformation;
}

SceneNode* SceneNode::getMeshNode()
{
    return mesh_node;
}

std::vector<SceneNode*> SceneNode::getChildren()
{
    return this->children;
}

bool SceneNode::addChild(SceneNode* node)
{
    if(node == nullptr) // No node given
    {
        return false;
    }
    
    if(this->mesh_data.getVertices().size() != 0) // This node already has mesh data, so we need to move that data to a child node
    {
        mesh_node = new SceneNode();
        mesh_node->setMeshData(this->mesh_data); // Copy the data to the new child.
        mesh_data.clear(); // Clear our own data
        this->children.push_back(mesh_node);
    }
    
    this->children.push_back(node);
    return true;
}

MeshData& SceneNode::getMeshData()
{
    return mesh_data;
}

void SceneNode::setMeshData(MeshData mesh_data)
{
    this->mesh_data = mesh_data;
}

std::string SceneNode::getType()
{
    return this->type;
}

void SceneNode::setType(std::string type)
{
    if(type == "model" || type == "solidsupport" || type == "support" || type == "surface" || type == "other")
    {
        this->type = type;
    }
}

void SceneNode::fillByXMLNode(pugi::xml_node xml_node)
{
    settings.clear();
    id = xml_node.attribute("id").as_string();
    name = xml_node.attribute("name").as_string();

    if(xml_node.child("mesh"))
    {
        mesh_data.clear();
        mesh_data.fillByXMLNode(xml_node.child("mesh"));
    }

    // Read settings the old way -which didn't conform  to the 3MF standard- for backwards compat.:
    const pugi::xml_node settings_node = xml_node.child("settings");
    if(settings_node)
    {
        for(pugi::xml_node setting = settings_node.child("setting"); setting; setting = setting.next_sibling("setting"))
        {
            const std::string key = setting.attribute("key").as_string();
            const std::string value = setting.text().as_string();
            setSetting(key, value);
        }
    }

    // Read settings the conformant way:
    const pugi::xml_node metadatagroup_node = xml_node.child("metadatagroup");
    if (metadatagroup_node)
    {
        for (pugi::xml_node setting = metadatagroup_node.child("metadata"); setting; setting = setting.next_sibling("metadata"))
        {
            // NOTE: In theory this could be slow, since we look up the entire ancestry tree for each setting.
            //       In practice it's expected to be negligible compared to the parsing of the mesh.
            const xml_namespace::xmlns_map_t namespaces = xml_namespace::getAncestralNamespaces(setting);
            std::set<std::string> cura_equivalent_namespaces = xml_namespace::getNamesFor(namespaces, xml_namespace::getCuraUri());
            cura_equivalent_namespaces.insert("cura"); // <- Just to be sure: If there was ever a version out that uses 'cura' without the specification of the xmlns-id.

            std::string key = setting.attribute("name").as_string();
            const size_t pos = key.find_first_of(':');

            //Make 'cura' namespace behave like the default.
            if (pos != std::string::npos && cura_equivalent_namespaces.count(key.substr(0, pos)) == 1)
            {
                key = key.substr(pos + 1);
            }
            const std::string value = setting.text().as_string();
            std::string type = setting.attribute("type").as_string();
            if(type == "")
            {
                type = "xs:string";
            }
            std::string preserve_str = setting.attribute("preserve").as_string(); //as_bool is too strict. Any non-zero value evaluates as true. Parse this ourselves.
            const bool preserve = (preserve_str != "" && preserve_str != "0");
            setSetting(key, value, type, preserve);
        }
    }
}

std::string SceneNode::getId()
{
    return this->id;
}

void SceneNode::setId(std::string id)
{
    this->id = id;
}

std::string SceneNode::getName()
{
    return this->name;
}

void SceneNode::setName(std::string name)
{
    this->name = name;
}

const std::map<std::string, MetadataEntry>& SceneNode::getSettings() const
{
    return settings;
}

void SceneNode::setSetting(const std::string& key, const MetadataEntry& entry)
{
    settings.emplace(key, entry);
}

void SceneNode::setSetting(const std::string& key, const std::string& value, const std::string& type, const bool preserve)
{
    settings.emplace(key, MetadataEntry(value, type, preserve));
}

void SceneNode::removeSetting(std::string key)
{
    settings.erase(key);
}


std::vector< SceneNode*> SceneNode::getAllChildren()
{
    std::vector<SceneNode*> all_children;
    
    for(SceneNode* scene_node: children)
    {
        std::vector<SceneNode*> temp_children = scene_node->getAllChildren();
        all_children.insert(all_children.end(), temp_children.begin(), temp_children.end());
    }
    
    // Add all direct children to the result to return. 
    // We put them at the end so that the "simplicity" rule of 3MF is kept:
    // "In keeping with the use of a simple parser, producers MUST define objects prior to referencing them as components."
    all_children.insert(all_children.end(), children.begin(), children.end());
    return all_children;
}

