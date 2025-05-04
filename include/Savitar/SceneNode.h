// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef SAVITAR_SCENENODE_H
#define SAVITAR_SCENENODE_H

#include "Savitar/MeshData.h"
#include "Savitar/MetadataEntry.h"

#include <map> // For std::map
#include <string>
#include <vector>

#include <pugixml.hpp>

namespace Savitar
{
class SceneNode
{
public:
    SceneNode() = default;
    virtual ~SceneNode() = default;

    [[nodiscard]] std::string getTransformation();
    void setTransformation(std::string);

    [[nodiscard]] std::vector<SceneNode*> getChildren();
    [[nodiscard]] std::vector<SceneNode*> getAllChildren();
    bool addChild(SceneNode* node);

    MeshData& getMeshData();
    void setMeshData(const MeshData& mesh_data);

    /**
     * Set the data of this SceneNode by giving it a xml node
     */
    void fillByXMLNode(pugi::xml_node xml_node);

    /**
     * Parses the (mesh) data stored in the external component file
     */
    void parseComponentData(const std::string& xml_string);

    /**
     * Get the (unique) identifier of the node.
     */
    [[nodiscard]] std::string getPath();

    void setPath(std::string id);

    /**
     * Get the (unique) identifier of the node.
     */
    [[nodiscard]] std::string getId();

    void setId(std::string id);

    /**
     * Get the (non-unique) display name of the node.
     */
    [[nodiscard]] std::string getName();

    [[maybe_unused]] void setName(std::string name);

    /**
     * Get the (per-object) settings attached to this SceneNode.
     * Note that this is part of the Cura Extension and not 3mf Core.
     */
    [[nodiscard]] const std::map<std::string, MetadataEntry>& getSettings() const;

    [[maybe_unused]] void setSetting(const std::string& key, const MetadataEntry& entry);
    void setSetting(const std::string& key, const std::string& value, const std::string& type = "xs:string", const bool preserve = false);
    void removeSetting(std::string key);

    /**
     * Type of the scene node. Can be "model", "solidsupport", "support", "surface", or "other".
     * This defaults to "model"
     */
    [[nodiscard]] std::string getType();
    [[maybe_unused]] void setType(const std::string& type);

    [[nodiscard]] SceneNode* getMeshNode();

    [[nodiscard]] std::string getComponentPath() const;

private:
    std::string transformation_{ "1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0" };
    std::vector<SceneNode*> children_;
    MeshData mesh_data_;
    std::map<std::string, MetadataEntry> settings_;
    std::string path_;
    std::string id_;
    std::string name_;
    std::string type_{ "model" };
    std::string component_path_;

    // 3MF does not support having an Object that has a mesh and components.
    // This is solved by the concept of the "mesh" node, which is added as a child.
    // This then gets a bit of metadata set so we can restore the graph in the way that we expect it.
    SceneNode* mesh_node_{ nullptr };
};
} // namespace Savitar

#endif
