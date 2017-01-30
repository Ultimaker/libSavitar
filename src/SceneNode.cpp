#include "SceneNode.h"

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

