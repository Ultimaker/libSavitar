// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef MESHDATA_H
#define MESHDATA_H

#include <cstdint>
#include <string>
#include <vector>

#include "Savitar/Face.h"
#include "Savitar/Types.h"
#include "Savitar/Vertex.h"

#include <pugixml.hpp>

namespace Savitar
{
class MeshData
{
public:
    /**
     * MeshData holds all data regarding a mesh (vertices, faces, etc).
     * It can be filled by means of a byte array by using setVerticesFromBytes or setFacesFromBytes.
     */
    MeshData() = default;
    virtual ~MeshData() = default;

    /**
     * Set the data of this MeshData by giving it a xml node (An object node in 3mf)
     */
    void fillByXMLNode(pugi::xml_node xml_node);

    /**
     * Serialise the meshData to xml_node
     */
    void toXmlNode(pugi::xml_node& xml_node);

    /**
     * Return the vertices as flattend bytes.
     *
     * If there for example is a single vertex, it will return a byte array containing 3 floats (so 3 * 4 bytes)
     */
    [[nodiscard]] bytearray getVerticesAsBytes();

    /**
     * Return the faces as flattend bytes.
     *
     * If there for example is a single face, it will return a byte array containing 3 ints (so 3 * 4 bytes)
     */
    [[nodiscard]] bytearray getFacesAsBytes();

    /**
     * Instead of getting all unique vertices, this function returns a bytearray with 3 vertices per face.
     * This is usefull if you want to mimic the data type of STL files.
     */
    [[nodiscard]] bytearray getFlatVerticesAsBytes();

    /**
     * Set the vertices of the meshdata by bytearray (as set from python)
     *
     * For every vertex it's assumed that there are 12 bytes (3 floats * 4).
     */
    void setVerticesFromBytes(const bytearray& data);

    /**
     * Set the faces of the meshdata by bytearray (as set from python)
     *
     * For every face it's assumed that there are 12 bytes (3 int * 4).
     */
    void setFacesFromBytes(const bytearray& data);

    [[nodiscard]] std::vector<Vertex> getVertices();

    /**
     * Reset the data of the MeshData object.
     */
    void clear();

private:
    std::vector<Vertex> vertices_;
    std::vector<Face> faces_;
};
} // namespace Savitar

#endif