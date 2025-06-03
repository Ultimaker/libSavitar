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
class TextureData
{
public:
    struct UVCoordinatesGroup
    {
        int texture_id;
        std::vector<UVCoordinate> coordinates;
    };

    TextureData();
    virtual ~TextureData() = default;

    void fillByXMLNode(pugi::xml_node xml_node);

    void toXmlNode(pugi::xml_node& resources_node);

    [[nodiscard]] std::string getTexturePath(const int texture_id) const;

    [[nodiscard]] const UVCoordinatesGroup* getUVCoordinatesGroup(const int id) const;

    [[nodiscard]] int setUVCoordinatesGroupFromBytes(const bytearray& data, const int texture_id);

    [[nodiscard]] int addTexturePath(const std::string& texture_path);

    [[nodiscard]] std::string getTexturePathFromGroupId(const int uv_group_id) const;

private:
    std::map<int, std::string> textures_paths_;
    std::map<int, UVCoordinatesGroup> uv_coordinates_;
};
} // namespace Savitar

#endif