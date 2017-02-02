#include "SceneNode.h"
#include "../pugixml/src/pugixml.hpp"
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

void SceneNode::setStransformation(std::string transformation)
{
    this->transformation = transformation;
}


std::vector<SceneNode> SceneNode::getChildren()
{
    return this->children;
}

void SceneNode::addChild(SceneNode node)
{
    this->children.push_back(node);
}

MeshData* SceneNode::getMeshData()
{
    return &mesh_data;
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

std::map< std::string, std::string > SceneNode::getSettings()
{
    return settings;
}


