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

#include "ThreeMFParser.h"
#include "Namespace.h"
#include "Scene.h"
#include <iostream>
#include <sstream>
#include <locale.h>

using namespace Savitar;

ThreeMFParser::ThreeMFParser()
{
    setlocale(LC_ALL, "C");
}

ThreeMFParser::~ThreeMFParser()
{

}

Scene ThreeMFParser::parse(std::string xml_string)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_string(xml_string.c_str());

    Scene scene;

    scene.fillByXMLNode(document.child("model"));


    return scene;
}

std::string ThreeMFParser::sceneToString(Scene scene)
{
    pugi::xml_document document;
    pugi::xml_node model_node = document.append_child("model");
    pugi::xml_node resources_node = model_node.append_child("resources");
    pugi::xml_node build_node = model_node.append_child("build");

    model_node.append_attribute("unit") = scene.getUnit().c_str();
    model_node.append_attribute("xmlns") = xml_namespace::getDefaultUri().c_str();
    model_node.append_attribute("xmlns:cura") = xml_namespace::getCuraUri().c_str();
    model_node.append_attribute("xml:lang") ="en-US";
    
    for(int i = 0; i < scene.getAllSceneNodes().size(); i++)
    {
        SceneNode* scene_node = scene.getAllSceneNodes().at(i);
        scene_node->setId(std::to_string(i + 1));
    }

    for(SceneNode* scene_node: scene.getAllSceneNodes())
    {
        // Create item
        pugi::xml_node object = resources_node.append_child("object");
        object.append_attribute("id") = scene_node->getId().c_str();
        if(!scene_node->getName().empty())
        {
            object.append_attribute("name") = scene_node->getName().c_str();
        }
        object.append_attribute("type") = scene_node->getType().c_str();

        const std::map<std::string, MetadataEntry>& per_object_settings = scene_node->getSettings();
        if(!per_object_settings.empty())
        {
            pugi::xml_node settings = object.append_child("metadatagroup");
            for(const std::pair<std::string, MetadataEntry>& setting_pair: per_object_settings)
            {
                pugi::xml_node setting = settings.append_child("metadata");
                setting.append_attribute("name") = setting_pair.first.c_str();
                setting.text().set(setting_pair.second.value.c_str());
                if(setting_pair.second.type != "xs:string") //xs:string is the default type and doesn't need to be written.
                {
                    setting.append_attribute("type") = setting_pair.second.type.c_str();
                }
                if(setting_pair.second.preserve)
                {
                    setting.append_attribute("preserve") = "true";
                }
            }
        }
        
        if(scene_node->getMeshData().getVertices().size() != 0)
        {
            pugi::xml_node mesh = object.append_child("mesh");
            scene_node->getMeshData().toXmlNode(mesh);
        }

        if(scene_node->getChildren().size() != 0)
        {
            pugi::xml_node components = object.append_child("components");
            for(SceneNode* child_scene_node: scene_node->getChildren())
            {
                pugi::xml_node component = components.append_child("component");
                component.append_attribute("objectid") = child_scene_node->getId().c_str();
                component.append_attribute("transform") = child_scene_node->getTransformation().c_str();
            }
        }
        if(scene_node->getMeshNode() != nullptr)
        {
            if(!object.child("metadatagroup"))
            {
                object.append_child("metadatagroup");
            }
            pugi::xml_node mesh_node_setting = object.child("metadatagroup").append_child("metadata");
            mesh_node_setting.append_attribute("name") = "mesh_node_objectid";
            mesh_node_setting.text().set(scene_node->getMeshNode()->getId().c_str());
            mesh_node_setting.append_attribute("preserve") = "true";
        }
    }

    for(SceneNode* scene_node: scene.getSceneNodes())
    {
        pugi::xml_node item = build_node.append_child("item");
        item.append_attribute("objectid") = scene_node->getId().c_str();
        item.append_attribute("transform") = scene_node->getTransformation().c_str();
    }
    
    for(const std::pair<std::string, MetadataEntry>& metadata_pair: scene.getMetadata())
    {
        pugi::xml_node metadata_node = model_node.append_child("metadata");
        metadata_node.append_attribute("name") = metadata_pair.first.c_str();
        metadata_node.text().set(metadata_pair.second.value.c_str());
        if(metadata_pair.second.type != "xs:string") //xs:string is the default and doesn't need to get written then.
        {
            metadata_node.append_attribute("type") = metadata_pair.second.type.c_str();
        }
        if(metadata_pair.second.preserve)
        {
            metadata_node.append_attribute("preserve") = "true";
        }
    }
    
    std::stringstream ss;
    document.save(ss);
    return ss.str();
}
