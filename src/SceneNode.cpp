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

SceneNode* SceneNode::getChildByIndex(int index)
{
    if(index < 0 || index >= children.size())
    {
        return NULL;
    }

    return &this->children.at(index);
}

int SceneNode::getNumChildren()
{
    return children.size();
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
    id = xml_node.attribute("id").as_string();

    if(xml_node.child("mesh"))
    {
        mesh_data.clear();
        mesh_data.fillByXMLNode(xml_node.child("mesh"));
    }
}

std::string SceneNode::getId()
{
    return this->id;
}

