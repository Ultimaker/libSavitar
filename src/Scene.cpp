#include "Scene.h"

using namespace Savitar;

Scene::Scene()
{

}

Scene::~Scene()
{

}

std::vector< SceneNode > Scene::getSceneNodes()
{
    return this->scene_nodes;
}

void Scene::addSceneNode(SceneNode node)
{
    this->scene_nodes.push_back(node);
}
