/*
 * This file is part of libSavitar
 *
 * Copyright (C) 2017 Ultimaker b.v. <j.vankessel@ultimaker.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ThreeMFParser.h"
#include "Scene.h"
#include <iostream>
#include <sstream>


using namespace Savitar;

ThreeMFParser::ThreeMFParser()
{

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
    model_node.append_attribute("xmlns") = "http://schemas.microsoft.com/3dmanufacturing/core/2015/02";
    model_node.append_attribute("xml:lang") ="en-US";

    /*int i = 0;
    for(; i < scene.getAllSceneNodes().size(); i++)
    {
        SceneNode scene_node = scene.getAllSceneNodes().at(i);
        scene_node.setId(std::to_string(i));
    }

    for(int j = 0; j < scene.getAllSceneNodes().size(); j++)
    {
        std::cout << scene.getAllSceneNodes().at(j).getId() << std::endl;
    }*/

    /*for(; i < scene.getSceneNodes().size(); i++)
    {
        pugi::xml_node temp_item_node = build_node.append_child("item");
        SceneNode scene_node = scene.getSceneNodes().at(i);
        scene_node.setId(std::to_string(i));
        temp_item_node.append_attribute("objectid") = scene_node.getId().c_str();
    }*/

    //DEBUG TEST STUFFS
    /*SceneNode node = scene.getSceneNodes().at(0);
    pugi::xml_node item = build_node.append_child("item");
    item.append_attribute("objectid") = "1";

    pugi::xml_node object = resources_node.append_child("object");
    object.append_attribute("id") = "1";
    object.append_attribute("type") = "model";
    pugi::xml_node mesh = object.append_child("mesh");
    node.getMeshData().toXmlNode(mesh);*/




    std::stringstream ss;
    document.save(ss);
    return ss.str();
}