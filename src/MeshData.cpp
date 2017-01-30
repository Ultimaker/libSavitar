#include "MeshData.h"
#include "../pugixml/src/pugixml.hpp"
#include <iostream>

using namespace Savitar;

MeshData::MeshData()
{

}

MeshData::~MeshData()
{

}

void MeshData::fillByXMLNode(pugi::xml_node xml_node)
{
    this->vertices.clear();
    this->faces.clear();

    // TODO: Add error handling (what to do if there is no vertices object, etc)

    // Add every vertex.
    pugi::xml_node xml_vertices = xml_node.child("vertices");
    for(pugi::xml_node vertex = xml_vertices.child("vertex"); vertex; vertex = vertex.next_sibling("vertex"))
    {
        Vertex temp_vertex = Vertex(vertex.attribute("x").as_float(), vertex.attribute("y").as_float(), vertex.attribute("z").as_float());
        this->vertices.push_back(temp_vertex);
    }


    // Add every face.
    pugi::xml_node xml_triangles = xml_node.child("triangles");
    for(pugi::xml_node face = xml_triangles.child("triangle"); face; face = face.next_sibling("triangle"))
    {
        Face temp_face = Face(face.attribute("v1").as_int(), face.attribute("v2").as_int(), face.attribute("v3").as_int());
        this->faces.push_back(temp_face);
    }
}
