// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/MeshData.h"
#include <cstring>
#include <iostream>
#include <pugixml.hpp>
#include <stdexcept> //For std::runtime_error.

using namespace Savitar;

float parseFloat(const pugi::xml_attribute& attribute)
{
    if (std::strchr(attribute.value(), ',') != nullptr)
    {
        // Until the implementation of more robust error-handling, it'll have to be done this way:
        throw std::runtime_error("Comma's should not be used as decimal separators, locale should be set to \"C\" for .3MF files.");
    }
    return attribute.as_float();
}

void MeshData::fillByXMLNode(pugi::xml_node xml_node)
{
    this->vertices_.clear();
    this->faces_.clear();

    // TODO: Add error handling (what to do if there is no vertices object, etc)
    // Add every vertex.
    pugi::xml_node xml_vertices = xml_node.child("vertices");
    for (pugi::xml_node vertex = xml_vertices.child("vertex"); vertex != nullptr; vertex = vertex.next_sibling("vertex"))
    {
        Vertex temp_vertex = Vertex(parseFloat(vertex.attribute("x")), parseFloat(vertex.attribute("y")), parseFloat(vertex.attribute("z")));
        this->vertices_.push_back(temp_vertex);
    }

    // Add every face.
    pugi::xml_node xml_triangles = xml_node.child("triangles");
    for (pugi::xml_node face = xml_triangles.child("triangle"); face != nullptr; face = face.next_sibling("triangle"))
    {
        Face temp_face = Face(face.attribute("v1").as_int(), face.attribute("v2").as_int(), face.attribute("v3").as_int());
        this->faces_.push_back(temp_face);
    }
}

void MeshData::clear()
{
    this->faces_.clear();
    this->vertices_.clear();
}

bytearray MeshData::getVerticesAsBytes()
{
    bytearray vertices_data;

    for (auto& vertice : vertices_)
    {
        float x = vertice.getX();
        float y = vertice.getY();
        float z = vertice.getZ();
        vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&x), reinterpret_cast<const uint8_t*>(&x) + sizeof(float));
        vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&y), reinterpret_cast<const uint8_t*>(&y) + sizeof(float));
        vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&z), reinterpret_cast<const uint8_t*>(&z) + sizeof(float));
    }
    return vertices_data;
}

bytearray MeshData::getFlatVerticesAsBytes()
{
    bytearray vertices_data;
    for (auto& face : faces_)
    {
        int v1 = face.getV1();
        int v2 = face.getV2();
        int v3 = face.getV3();
        float x;
        float y;
        float z;

        // Add vertices for face 1
        if (v1 >= 0 && v1 < vertices_.size())
        {
            x = vertices_.at(v1).getX();
            y = vertices_.at(v1).getY();
            z = vertices_.at(v1).getZ();
            vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&x), reinterpret_cast<const uint8_t*>(&x) + sizeof(float));
            vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&y), reinterpret_cast<const uint8_t*>(&y) + sizeof(float));
            vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&z), reinterpret_cast<const uint8_t*>(&z) + sizeof(float));
        }
        else
        {
            return {};
        }

        // Add vertices for face 2
        if (v2 >= 0 && v2 < vertices_.size())
        {
            x = vertices_.at(v2).getX();
            y = vertices_.at(v2).getY();
            z = vertices_.at(v2).getZ();
            vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&x), reinterpret_cast<const uint8_t*>(&x) + sizeof(float));
            vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&y), reinterpret_cast<const uint8_t*>(&y) + sizeof(float));
            vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&z), reinterpret_cast<const uint8_t*>(&z) + sizeof(float));
        }
        else
        {
            return {};
        }

        // Add vertices for face 3
        if (v3 >= 0 && v3 < vertices_.size())
        {
            x = vertices_.at(v3).getX();
            y = vertices_.at(v3).getY();
            z = vertices_.at(v3).getZ();
            vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&x), reinterpret_cast<const uint8_t*>(&x) + sizeof(float));
            vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&y), reinterpret_cast<const uint8_t*>(&y) + sizeof(float));
            vertices_data.insert(vertices_data.end(), reinterpret_cast<const uint8_t*>(&z), reinterpret_cast<const uint8_t*>(&z) + sizeof(float));
        }
        else
        {
            return {};
        }
    }
    return vertices_data;
}

bytearray MeshData::getFacesAsBytes()
{
    bytearray face_data;

    for (auto& face : faces_)
    {
        int v1 = face.getV1();
        int v2 = face.getV2();
        int v3 = face.getV3();
        face_data.insert(face_data.end(), reinterpret_cast<const uint8_t*>(&v1), reinterpret_cast<const uint8_t*>(&v1) + sizeof(int));
        face_data.insert(face_data.end(), reinterpret_cast<const uint8_t*>(&v2), reinterpret_cast<const uint8_t*>(&v2) + sizeof(int));
        face_data.insert(face_data.end(), reinterpret_cast<const uint8_t*>(&v3), reinterpret_cast<const uint8_t*>(&v3) + sizeof(int));
    }
    return face_data;
}

void MeshData::toXmlNode(pugi::xml_node& node)
{
    pugi::xml_node vertices_node = node.append_child("vertices");
    for (auto& vertice : vertices_)
    {
        pugi::xml_node vertex_node = vertices_node.append_child("vertex");
        vertex_node.append_attribute("x") = vertice.getX();
        vertex_node.append_attribute("y") = vertice.getY();
        vertex_node.append_attribute("z") = vertice.getZ();
    }

    pugi::xml_node triangles_node = node.append_child("triangles");
    for (auto& face : faces_)
    {
        pugi::xml_node triangle_node = triangles_node.append_child("triangle");
        triangle_node.append_attribute("v1") = face.getV1();
        triangle_node.append_attribute("v2") = face.getV2();
        triangle_node.append_attribute("v3") = face.getV3();
    }
}

void MeshData::setVerticesFromBytes(const bytearray& data)
{
    vertices_.clear();
    const uint8_t* bytes = data.data();
    auto num_bytes = data.size();
    auto num_floats = num_bytes / sizeof(float);

    // Interpret byte array as array of floats.
    const auto* float_array = reinterpret_cast<const float*>(bytes);

    for (int i = 0; i + 2 < num_floats; i += 3)
    {
        Vertex temp_vertex = Vertex(float_array[i], float_array[i + 1], float_array[i + 2]);
        vertices_.push_back(temp_vertex);
    }
}

void MeshData::setFacesFromBytes(const bytearray& data)
{
    faces_.clear();
    const uint8_t* bytes = data.data();
    auto num_bytes = data.size();
    auto num_ints = num_bytes / sizeof(int);

    // Interpret byte array as array of ints.
    const int* int_array = reinterpret_cast<const int*>(bytes);

    for (int i = 0; i + 2 < num_ints; i += 3)
    {
        Face temp_face = Face(int_array[i], int_array[i + 1], int_array[i + 2]);
        faces_.push_back(temp_face);
    }
}

std::vector<Vertex> MeshData::getVertices()
{
    return vertices_;
}
