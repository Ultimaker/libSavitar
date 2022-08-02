// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/MeshData.h"

#include <array>
#include <fstream>
#include <gtest/gtest.h>
#include <iostream>
#include <iterator>
#include <string>

#include <pugixml.hpp>

namespace Savitar
{

constexpr int NUM_COORDS = 12;
constexpr int NUM_INDICES = 6;
const std::array<float, NUM_COORDS> vertices =
{
    2.f, 2.f, -1.f,
    4.f, 2.f,  1.f,
    4.f, 4.f, -1.f,
    2.f, 4.f,  1.f
};
const std::array<int, NUM_INDICES> faces =
{
    0, 1, 2,
    2, 1, 3
};


/*
 * Fixture with some settings and testing mesh data to test with.
 */
class MeshDataTest : public testing::Test
{
public:
    const uint8_t* vertex_bytes;
    const uint8_t* face_bytes;

    pugi::xml_node node;
    MeshData* mesh_data;

    void SetUp()
    {
        mesh_data = new MeshData();

        vertex_bytes = reinterpret_cast<const uint8_t*>(vertices.data());
        face_bytes = reinterpret_cast<const uint8_t*>(faces.data());

        std::vector<uint8_t> vertex_vector;
        for (int i_byte = 0; i_byte < NUM_COORDS * sizeof(float); ++i_byte)
        {
            vertex_vector.push_back(vertex_bytes[i_byte]);
        }

        std::vector<uint8_t> face_vector;
        for (int i_byte = 0; i_byte < NUM_INDICES * sizeof(int); ++i_byte)
        {
            face_vector.push_back(face_bytes[i_byte]);
        }

        mesh_data->setVerticesFromBytes(vertex_vector);
        mesh_data->setFacesFromBytes(face_vector);
    }

    void TearDown()
    {
        delete mesh_data;
    }
};

TEST_F(MeshDataTest, toXmlNode)
{
    pugi::xml_document document;
    node = document.append_child("mesh");
    mesh_data->toXmlNode(node);

    pugi::xml_node vertices_node = node.child("vertices");
    ASSERT_TRUE(vertices_node);
    ASSERT_FALSE(vertices_node.next_sibling("vertices"));

    int i_coord = 0;
    for (pugi::xml_node vertex_node : vertices_node.children("vertex"))
    {
        EXPECT_LT(i_coord, NUM_COORDS);

        pugi::xml_attribute x_attr = vertex_node.attribute("x");
        pugi::xml_attribute y_attr = vertex_node.attribute("y");
        pugi::xml_attribute z_attr = vertex_node.attribute("z");
        EXPECT_TRUE(x_attr);
        EXPECT_TRUE(y_attr);
        EXPECT_TRUE(z_attr);

        float x, y, z;
        EXPECT_NO_THROW(x = x_attr.as_float());
        EXPECT_NO_THROW(y = y_attr.as_float());
        EXPECT_NO_THROW(z = z_attr.as_float());

        EXPECT_EQ(x, vertices[i_coord]);
        EXPECT_EQ(y, vertices[i_coord + 1]);
        EXPECT_EQ(z, vertices[i_coord + 2]);

        i_coord += 3;
    }
    EXPECT_EQ(i_coord, NUM_COORDS);

    pugi::xml_node triangles_node = node.child("triangles");
    ASSERT_TRUE(triangles_node);
    EXPECT_FALSE(triangles_node.next_sibling("triangles"));

    int i_poly = 0;
    for (pugi::xml_node vertex_node : triangles_node.children("triangle"))
    {
        EXPECT_LT(i_poly, NUM_INDICES);

        pugi::xml_attribute v1_attr = vertex_node.attribute("v1");
        pugi::xml_attribute v2_attr = vertex_node.attribute("v2");
        pugi::xml_attribute v3_attr = vertex_node.attribute("v3");
        EXPECT_TRUE(v1_attr);
        EXPECT_TRUE(v2_attr);
        EXPECT_TRUE(v3_attr);

        int v1, v2, v3;
        EXPECT_NO_THROW(v1 = v1_attr.as_int());
        EXPECT_NO_THROW(v2 = v2_attr.as_int());
        EXPECT_NO_THROW(v3 = v3_attr.as_int());

        EXPECT_EQ(v1, faces[i_poly]);
        EXPECT_EQ(v2, faces[i_poly + 1]);
        EXPECT_EQ(v3, faces[i_poly + 2]);

        i_poly += 3;
    }
    EXPECT_EQ(i_poly, NUM_INDICES);

    mesh_data->clear();
}

TEST_F(MeshDataTest, fromXmlNode)
{
    pugi::xml_document document;
    node = document.append_child("mesh");
    mesh_data->toXmlNode(node);
    mesh_data->clear();

    mesh_data->fillByXMLNode(node);

    const bytearray got_vertex_bytes = mesh_data->getVerticesAsBytes();
    ASSERT_EQ(got_vertex_bytes.size(), sizeof(float) * vertices.size());

    int i_vertex_byte = -1;
    for (const uint8_t& byte : got_vertex_bytes)
    {
        EXPECT_EQ(byte, vertex_bytes[++i_vertex_byte]);
    }

    const bytearray got_face_bytes = mesh_data->getFacesAsBytes();
    ASSERT_EQ(got_face_bytes.size(), sizeof(int) * faces.size());

    int i_face_byte = -1;
    for (const uint8_t& byte : got_face_bytes)
    {
        EXPECT_EQ(byte, face_bytes[++i_face_byte]);
    }
}

} // namespace Savitar
