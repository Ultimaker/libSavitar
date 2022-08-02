// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef SAVITAR_SCENENODE_H
#define SAVITAR_SCENENODE_H

#include "Savitar/MeshData.h"
#include "Savitar/MetadataEntry.h"

#include <map> // For std::map
#include <string>
#include <vector>

namespace pugi
{
class xml_node;
}

namespace Savitar
{
class SceneNode
{
public:
    SceneNode();
    virtual ~SceneNode();

    std::string getTransformation();
    void setTransformation(std::string);

    std::vector<SceneNode*> getChildren();
    std::vector<SceneNode*> getAllChildren();
    bool addChild(SceneNode* node);

    MeshData& getMeshData();
    void setMeshData(MeshData mesh_data);

    /**
     * Set the data of this SceneNode by giving it a xml node
     */
    void fillByXMLNode(pugi::xml_node xml_node);

    /**
     * Get the (unique) identifier of the node.
     */
    std::string getId();

    void setId(std::string id);

    /**
     * Get the (non-unique) display name of the node.
     */
    std::string getName();

    void setName(std::string name);

    /**
     * Get the (per-object) settings attached to this SceneNode.
     * Note that this is part of the Cura Extension and not 3mf Core.
     */
    const std::map<std::string, MetadataEntry>& getSettings() const;

    void setSetting(const std::string& key, const MetadataEntry& entry);
    void setSetting(const std::string& key, const std::string& value, const std::string& type = "xs:string", const bool preserve = false);
    void removeSetting(std::string key);

    /**
     * Type of the scene node. Can be "model", "solidsupport", "support", "surface", or "other".
     * This defaults to "model"
     */
    std::string getType();
    void setType(std::string type);

    SceneNode* getMeshNode();

protected:
    std::string transformation;
    std::vector<SceneNode*> children;
    MeshData mesh_data;
    std::map<std::string, MetadataEntry> settings;
    std::string id;
    std::string name;
    std::string type;

    // 3MF does not support having an Object that has a mesh and components.
    // This is solved by the concept of the "mesh" node, which is added as a child.
    // This then gets a bit of metadata set so we can restore the graph in the way that we expect it.
    SceneNode* mesh_node;
};
} // namespace Savitar

#endif
