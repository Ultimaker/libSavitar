#include "Scene.h"
#include "../pugixml/src/pugixml.hpp"
#include <iostream>
#include <string>
using namespace Savitar;

Scene::Scene()
{
    unit = "millimeter";
}

Scene::~Scene()
{

}

std::vector< SceneNode*> Scene::getSceneNodes()
{
    return this->scene_nodes;
}

void Scene::addSceneNode(SceneNode* node)
{
    this->scene_nodes.push_back(node);
}


void Scene::fillByXMLNode(pugi::xml_node xml_node)
{
    unit = xml_node.attribute("unit").as_string();

    pugi::xml_node resources = xml_node.child("resources");

    // Handle metadata:
    for(pugi::xml_node metadata_node = xml_node.child("metadata"); metadata_node; metadata_node = metadata_node.next_sibling("metadata"))
    {
        setMetaDataEntry(metadata_node.attribute("name").as_string(), metadata_node.text().as_string());
    }

    pugi::xml_node build = xml_node.child("build");
    for(pugi::xml_node item = build.child("item"); item; item = item.next_sibling("item"))
    {
        // Found a item in the build. The items are linked to objects by objectid.
        pugi::xml_node object_node = resources.find_child_by_attribute("object", "id", item.attribute("objectid").value());
        if(object_node)
        {
            SceneNode* temp_scene_node = createSceneNodeFromObject(xml_node, object_node);
            temp_scene_node->setTransformation(item.attribute("transform").as_string());
            scene_nodes.push_back(temp_scene_node);
        }
        else
        {
            // TODO: add proper error handling
            std::cout << "Could not find object by given ID" << std::endl;
        }
    }
}

SceneNode* Scene::createSceneNodeFromObject(pugi::xml_node root_node, pugi::xml_node object_node)
{
    pugi::xml_node components = object_node.child("components");
    SceneNode* scene_node = new SceneNode();
    scene_node->fillByXMLNode(object_node);

    // We have to do the checking for children outside of the SceneNode creation itself, because it only has references.
    if(components)
    {
        for(pugi::xml_node component = components.child("component"); component; component = component.next_sibling("component"))
        {
            // This node has children. Add them one by one.
            pugi::xml_node child_object_node = root_node.child("resources").find_child_by_attribute("object", "id", component.attribute("objectid").value());
            if(child_object_node)
            {
                SceneNode* child_node = createSceneNodeFromObject(root_node, child_object_node);
                child_node->setTransformation(component.attribute("transform").as_string());
                scene_node->addChild(child_node);
            } else
            {
                // TODO: ADD proper error handling here.
                std::cout << "Child_object_node not found :( " << std::endl;
            }
        }
    }
    return scene_node;
}

std::vector<SceneNode*> Scene::getAllSceneNodes()
{
    std::vector<SceneNode*> all_nodes;
    all_nodes.insert(all_nodes.end(), scene_nodes.begin(), scene_nodes.end());
    for(SceneNode* scene_node: scene_nodes)
    {
        std::vector<SceneNode*> temp_children = scene_node->getAllChildren();
        all_nodes.insert(all_nodes.end(), temp_children.begin(), temp_children.end());
    }
    return all_nodes;
}

void Scene::setMetaDataEntry(std::string key, std::string value)
{
    metadata[key] = value;
}

std::string Scene::getUnit()
{
    return unit;
}

std::map< std::string, std::string > Scene::getMetadata()
{
    return metadata;
}

void Scene::setUnit(std::string unit)
{
    this->unit = unit;
}
