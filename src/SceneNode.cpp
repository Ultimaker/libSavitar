/*
 * This file is part of libSavitar
 *
 * Copyright (C) 2017 Ultimaker b.v. <j.vankessel@ultimaker.com>
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
#include <pugixml.hpp>
#include <iostream>
using namespace Savitar;

SceneNode::SceneNode()
{

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


std::vector<SceneNode*> SceneNode::getChildren()
{
    return this->children;
}

void SceneNode::addChild(SceneNode* node)
{
    if(node == nullptr)
    {
        return;
    }
    this->children.push_back(node);
}

MeshData& SceneNode::getMeshData()
{
    return mesh_data;
}

void SceneNode::setMeshData(MeshData mesh_data)
{
    this->mesh_data = mesh_data;
}

void SceneNode::fillByXMLNode(pugi::xml_node xml_node)
{
    settings.clear();
    id = xml_node.attribute("id").as_string();

    if(xml_node.child("mesh"))
    {
        mesh_data.clear();
        mesh_data.fillByXMLNode(xml_node.child("mesh"));
    }
    pugi::xml_node settings_node = xml_node.child("settings");
    if(settings_node)
    {
        for(pugi::xml_node setting = settings_node.child("setting"); setting; setting = setting.next_sibling("setting"))
        {
            std::string key = setting.attribute("key").as_string();
            std::string value = setting.text().as_string();
            settings[key] = value;
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

std::map< std::string, std::string > SceneNode::getSettings()
{
    return settings;
}

void SceneNode::setSetting(std::string key, std::string value)
{
    settings[key] = value;
}


std::vector< SceneNode*> SceneNode::getAllChildren()
{
    std::vector<SceneNode*> all_children;
    // Add all direct children to the result to return
    all_children.insert(all_children.end(), children.begin(), children.end());
    for(SceneNode* scene_node: children)
    {
        std::vector<SceneNode*> temp_children = scene_node->getAllChildren();
        all_children.insert(all_children.end(), temp_children.begin(), temp_children.end());
    }
    return all_children;
}
