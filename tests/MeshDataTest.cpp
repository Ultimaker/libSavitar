#include "MeshDataTest.h"

#include "../src/MeshData.h"

#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>

namespace Savitar
{
    CPPUNIT_TEST_SUITE_REGISTRATION(MeshDataTest);

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

    void MeshDataTest::setUp()
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

    void MeshDataTest::tearDown()
    {
        delete mesh_data;
    }

    void MeshDataTest::toXmlNodeTest()
    {
        pugi::xml_document document;
        node = document.append_child("mesh");
        mesh_data->toXmlNode(node);

        pugi::xml_node vertices_node = node.child("vertices");
        CPPUNIT_ASSERT(vertices_node);
        CPPUNIT_ASSERT(! vertices_node.next_sibling("vertices"));

        int i_coord = 0;
        for (pugi::xml_node vertex_node : vertices_node.children("vertex"))
        {
            CPPUNIT_ASSERT_LESS(NUM_COORDS, i_coord);

            pugi::xml_attribute x_attr = vertex_node.attribute("x");
            pugi::xml_attribute y_attr = vertex_node.attribute("y");
            pugi::xml_attribute z_attr = vertex_node.attribute("z");
            CPPUNIT_ASSERT(x_attr);
            CPPUNIT_ASSERT(y_attr);
            CPPUNIT_ASSERT(z_attr);

            float x, y, z;
            CPPUNIT_ASSERT_NO_THROW(x = x_attr.as_float());
            CPPUNIT_ASSERT_NO_THROW(y = y_attr.as_float());
            CPPUNIT_ASSERT_NO_THROW(z = z_attr.as_float());

            CPPUNIT_ASSERT_EQUAL(x, vertices[i_coord]);
            CPPUNIT_ASSERT_EQUAL(y, vertices[i_coord + 1]);
            CPPUNIT_ASSERT_EQUAL(z, vertices[i_coord + 2]);

            i_coord += 3;
        }
        CPPUNIT_ASSERT_EQUAL(i_coord, NUM_COORDS);

        pugi::xml_node triangles_node = node.child("triangles");
        CPPUNIT_ASSERT(triangles_node);
        CPPUNIT_ASSERT(! triangles_node.next_sibling("triangles"));

        int i_poly = 0;
        for (pugi::xml_node vertex_node : triangles_node.children("triangle"))
        {
            CPPUNIT_ASSERT_LESS(NUM_INDICES, i_poly);

            pugi::xml_attribute v1_attr = vertex_node.attribute("v1");
            pugi::xml_attribute v2_attr = vertex_node.attribute("v2");
            pugi::xml_attribute v3_attr = vertex_node.attribute("v3");
            CPPUNIT_ASSERT(v1_attr);
            CPPUNIT_ASSERT(v2_attr);
            CPPUNIT_ASSERT(v3_attr);

            int v1, v2, v3;
            CPPUNIT_ASSERT_NO_THROW(v1 = v1_attr.as_int());
            CPPUNIT_ASSERT_NO_THROW(v2 = v2_attr.as_int());
            CPPUNIT_ASSERT_NO_THROW(v3 = v3_attr.as_int());

            CPPUNIT_ASSERT_EQUAL(v1, faces[i_poly]);
            CPPUNIT_ASSERT_EQUAL(v2, faces[i_poly + 1]);
            CPPUNIT_ASSERT_EQUAL(v3, faces[i_poly + 2]);

            i_poly += 3;
        }
        CPPUNIT_ASSERT_EQUAL(i_poly, NUM_INDICES);

        mesh_data->clear();
    }

    void MeshDataTest::fromXmlNodeTest()
    {
        pugi::xml_document document;
        node = document.append_child("mesh");
        mesh_data->toXmlNode(node);
        mesh_data->clear();

        mesh_data->fillByXMLNode(node);

        const bytearray got_vertex_bytes = mesh_data->getVerticesAsBytes();
        CPPUNIT_ASSERT_EQUAL(got_vertex_bytes.size(), sizeof(float) * vertices.size());

        int i_vertex_byte = -1;
        for (const uint8_t& byte : got_vertex_bytes)
        {
            CPPUNIT_ASSERT_EQUAL(byte, vertex_bytes[++i_vertex_byte]);
        }

        const bytearray got_face_bytes = mesh_data->getFacesAsBytes();
        CPPUNIT_ASSERT_EQUAL(got_face_bytes.size(), sizeof(int) * faces.size());

        int i_face_byte = -1;
        for (const uint8_t& byte : got_face_bytes)
        {
            CPPUNIT_ASSERT_EQUAL(byte, face_bytes[++i_face_byte]);
        }
    }

} // namespace Savitar
