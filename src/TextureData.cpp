// Copyright (c) 2025 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/TextureData.h"

#include <filesystem>

using namespace Savitar;

TextureData::TextureData()
{
}

void TextureData::fillByXMLNode(pugi::xml_node xml_node)
{
    // Handle textures paths
    for (pugi::xml_node texture_node = xml_node.child("m:texture2d"); texture_node; texture_node = texture_node.next_sibling("m:texture2d"))
    {
        const int id = texture_node.attribute("id").as_int(-1);
        const std::string path = texture_node.attribute("path").as_string();
        if (id >= 0 && ! path.empty())
        {
            textures_paths_.emplace(id, std::move(path));
        }
    }

    // Handle UV coordinates groups
    for (pugi::xml_node texture_group_node = xml_node.child("m:texture2dgroup"); texture_group_node; texture_group_node = texture_group_node.next_sibling("m:texture2dgroup"))
    {
        const int id = texture_group_node.attribute("id").as_int(-1);
        const int texture_id = texture_group_node.attribute("texid").as_int(-1);
        if (id >= 0 && texture_id >= 0 && textures_paths_.find(texture_id) != textures_paths_.end())
        {
            UVCoordinatesGroup group;
            group.texture_id = texture_id;

            for (pugi::xml_node uv_coordinate_node = texture_group_node.child("m:tex2coord"); uv_coordinate_node;
                 uv_coordinate_node = uv_coordinate_node.next_sibling("m:tex2coord"))
            {
                group.coordinates.emplace_back(uv_coordinate_node.attribute("u").as_float(), uv_coordinate_node.attribute("v").as_float());
            }

            uv_coordinates_.emplace(id, std::move(group));
        }
    }
}

const TextureData::UVCoordinatesGroup* TextureData::getUVCoordinatesGroup(const int id) const
{
    auto iterator = uv_coordinates_.find(id);
    return iterator != uv_coordinates_.end() ? &iterator->second : nullptr;
}

std::string TextureData::getTexturePath(const int texture_id) const
{
    auto iterator = textures_paths_.find(texture_id);
    return iterator != textures_paths_.end() ? iterator->second : "";
}

std::string TextureData::getTexturePathFromGroupId(const int uv_group_id) const
{
    const UVCoordinatesGroup* group = getUVCoordinatesGroup(uv_group_id);
    if (! group)
    {
        return "";
    }

    return getTexturePath(group->texture_id);
}
