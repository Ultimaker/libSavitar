/*
 * This file is part of libSavitar
 *
 * Copyright (C) 2017 Ultimaker b.v. <j.vankessel@ultimaker.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MESHDATA_H
#define MESHDATA_H
#include "SavitarExport.h"
#include <vector>
#include <string>
#include <Python.h> // For PyObject

#include "Vertex.h"
#include "Face.h"

// Forward declarations
namespace pugi
{
    class xml_node;
}

namespace Savitar
{
    class SAVITAR_EXPORT MeshData
    {
    public:
        /**
         * MeshData holds all data regarding a mesh (vertices, faces, etc).
         * It can be filled by means of a byte array by using setVerticesFromBytes or setFacesFromBytes.
         */
        MeshData();
        virtual ~MeshData();

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
        PyObject* getVerticesAsBytes();

        /**
         * Return the faces as flattend bytes.
         *
         * If there for example is a single face, it will return a byte array containing 3 ints (so 3 * 4 bytes)
         */
        PyObject* getFacesAsBytes();

        /**
         * Instead of getting all unique vertices, this function returns a bytearray with 3 vertices per face.
         * This is usefull if you want to mimic the data type of STL files.
         */
        PyObject* getFlatVerticesAsBytes();

        /**
         * Set the vertices of the meshdata by bytearray (as set from python)
         *
         * For every vertex it's assumed that there are 12 bytes (3 floats * 4).
         */
        void setVerticesFromBytes(PyObject* py_bytes);

        /**
         * Set the faces of the meshdata by bytearray (as set from python)
         *
         * For every face it's assumed that there are 12 bytes (3 int * 4).
         */
        void setFacesFromBytes(PyObject* py_bytes);

        std::vector<Vertex> getVertices();

        /**
         * Reset the data of the MeshData object.
         */
        void clear();
    protected:
        std::vector<Vertex> vertices;
        std::vector<Face> faces;
    };
}

#endif