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
    std::string name = std::string(xml_node.name());
    if(name == "item")
    {

        std::cout << xml_node.name() << std::endl;
    }
    else if (name == "object")
    {

    }
}

