// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef SCENE_H
#define SCENE_H

#include "Savitar/SceneNode.h"

#include <map> // For std::map
#include <string> // For std::string
#include <vector> // For std::vector

// Forward declaration
namespace pugi
{
class xml_node;
}

namespace Savitar
{
class Scene
{
public:
    /**
     * A Scene is an object that holds a number of SceneNodes, but also has some meta data entries. It also defines a unit in which the data is expressed.
     */
    Scene();
    virtual ~Scene();

    /**
     * Get the scene nodes in this scene.
     * \return The scene nodes that are in the scene.
     */
    std::vector<SceneNode*> getSceneNodes();

    std::vector<SceneNode*> getAllSceneNodes();

    /**
     * Add a scene node to the scene.
     * \param node The SceneNode to be added.
     */
    void addSceneNode(SceneNode* node);

    /**
     * Set the data of this SceneNode by giving it a xml node
     */
    void fillByXMLNode(pugi::xml_node xml_node);

    /**
     * Store a metadata entry as metadata.
     * @param key The key of the metadata.
     * @param entry A MetadataEntry object containing metadata and
     * additional properties.
     */
    void setMetaDataEntry(const std::string& key, const MetadataEntry& entry);

    /**
     * Set a meta data entry of the scene.
     *
     * Note that this not adhere to the full 3mf spec yet. All keys are accepted.
     *
     * \param key The key of the metadata.
     * \param value The value of the metadata.
     * \param type The data type of the metadata.
     * \param preserve Whether the metadata entry needs to be preserved
     * through save-load loops of the 3MF document.
     */
    void setMetaDataEntry(const std::string& key, const std::string& value, const std::string& type = "xs:string", const bool preserve = false);

    /**
     * Get all meta data entries
     */
    const std::map<std::string, MetadataEntry>& getMetadata() const;

    /**
     * Get the unit (milimeter, inch, etc) of the scene.
     * This is in milimeter by default.
     */
    std::string getUnit();

    void setUnit(std::string unit);

protected:
    std::vector<SceneNode*> scene_nodes;
    std::map<std::string, MetadataEntry> metadata;
    std::string unit;

    /**
     * Used to recursively create SceneNode objects based on xml nodes.
     * Because 3mf uses references, we also need to provide the root_node, so it's know what the reference points to
     * \returns The created SceneNode.
     */
    SceneNode* createSceneNodeFromObject(pugi::xml_node root_node, pugi::xml_node object_node);
};
} // namespace Savitar
#endif