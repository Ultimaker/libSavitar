// Copyright (c) 2025 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef TEXTUREDATA_H
#define TEXTUREDATA_H

#include "Types.h"
#include "UVCoordinate.h"

#include <map>
#include <pugixml.hpp>
#include <string>
#include <vector>

namespace Savitar
{
/**
 * The TextureData stores UV coordinates groups and textures paths, that will end-up as resources in the global model description
 */
class TextureData
{
public:
    struct UVCoordinatesGroup
    {
        int texture_id; // The ID of the associated texture
        std::vector<UVCoordinate> coordinates; // The actual UV coordinates of the group
    };

    TextureData();
    virtual ~TextureData() = default;

    void fillByXMLNode(pugi::xml_node xml_node);

    void toXmlNode(pugi::xml_node& resources_node);

    [[nodiscard]] std::string getTexturePath(const int texture_id) const;

    [[nodiscard]] const UVCoordinatesGroup* getUVCoordinatesGroup(const int id) const;

    /**
     * Loads a UV coordinates group from raw data
     * @param data The actual UV coordinates as an array of floats
     * @param texture_id The ID of the associated texture
     * @param group_id The ID of the newly created group
     */
    void setUVCoordinatesGroupFromBytes(const bytearray& data, const int texture_id, const int group_id);

    void addTexturePath(const std::string& texture_path, const int id);

    [[nodiscard]] std::string getTexturePathFromGroupId(const int uv_group_id) const;

    /**
     * Find the next available resource ID amongst actually stored texture and UV coordinates. This should be called before
     * adding any of these resources, so that IDs are unique in the end.
     */
    [[nodiscard]] int getNextAvailableResourceId() const;

private:
    std::map<int, std::string> textures_paths_;
    std::map<int, UVCoordinatesGroup> uv_coordinates_;
};
} // namespace Savitar

#endif