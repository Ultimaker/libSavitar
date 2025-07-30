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

void TextureData::toXmlNode(pugi::xml_node& resources_node)
{
    // Handle textures paths
    for (const auto& texture_path : textures_paths_)
    {
        pugi::xml_node texture_node = resources_node.append_child("m:texture2d");
        texture_node.append_attribute("id") = texture_path.first;
        texture_node.append_attribute("path") = texture_path.second.c_str();
        texture_node.append_attribute("contenttype") = "image/png";
    }

    // Handle UV coordinates groups
    for (const auto& uv_coordinates_group : uv_coordinates_)
    {
        pugi::xml_node group_node = resources_node.append_child("m:texture2dgroup");
        group_node.append_attribute("id") = uv_coordinates_group.first;
        group_node.append_attribute("texid") = uv_coordinates_group.second.texture_id;

        for (const UVCoordinate& coordinate : uv_coordinates_group.second.coordinates)
        {
            pugi::xml_node coordinate_node = group_node.append_child("m:tex2coord");
            coordinate_node.append_attribute("u") = coordinate.getU();
            coordinate_node.append_attribute("v") = coordinate.getV();
        }
    }
}

const TextureData::UVCoordinatesGroup* TextureData::getUVCoordinatesGroup(const int id) const
{
    auto iterator = uv_coordinates_.find(id);
    return iterator != uv_coordinates_.end() ? &iterator->second : nullptr;
}

void TextureData::setUVCoordinatesGroupFromBytes(const bytearray& data, const int texture_id, const int group_id)
{
    // Interpret byte array as array of floats.
    const float* float_array = reinterpret_cast<const float*>(data.data());
    const size_t num_bytes = data.size();
    const size_t num_coordinates = (num_bytes / sizeof(float)) / 2;

    UVCoordinatesGroup group;
    group.texture_id = texture_id;
    for (size_t i = 0; i < num_coordinates; ++i)
    {
        group.coordinates.emplace_back(float_array[i * 2], float_array[i * 2 + 1]);
    }

    if (! group.coordinates.empty())
    {
        uv_coordinates_.emplace(group_id, std::move(group));
    }
}

void TextureData::addTexturePath(const std::string& texture_path, const int id)
{
    textures_paths_.emplace(id, texture_path);
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

int TextureData::getNextAvailableResourceId() const
{
    int id = 0;
    while (textures_paths_.find(id) != textures_paths_.end() || uv_coordinates_.find(id) != uv_coordinates_.end())
    {
        ++id;
    }

    return id;
}
