#include "Scene.h"
#include "../pugixml/src/pugixml.hpp"
#include <iostream>

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


void Scene::fillByXMLNode(pugi::xml_node xml_node)
{
    pugi::xml_node resources = xml_node.child("resources");
    std::vector<MeshData> meshes;
    std::vector<SceneNode> nodes_to_add;
    // For each object in resources node
    for(pugi::xml_node object = resources.child("object"); object; object = object.next_sibling("object"))
    {
        if(object.child("mesh") && std::string(object.attribute("type").value()) == "model")
        {
            MeshData temp_mesh;
            temp_mesh.fillByXMLNode(object);
            meshes.push_back(temp_mesh);
        }

        if(object.child("components"))
        {
            // Handle all scene nodes that are nested somehow.
        }
    }
    pugi::xml_node build = xml_node.child("build");
    for(pugi::xml_node item = build.child("item"); item; item = item.next_sibling("item"))
    {
        std::cout << "YAY" << std::endl;
        SceneNode temp_scene_node;
        temp_scene_node.fillByXMLNode(item);
    }
}
