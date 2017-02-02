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
        MeshData();
        virtual ~MeshData();

        /**
         * Set the data of this MeshData by giving it a xml node (An object node in 3mf)
         */
        void fillByXMLNode(pugi::xml_node xml_node);

        PyObject* getVerticesAsBytes();
        PyObject* getFacesAsBytes();
        void clear();
    protected:
        std::vector<Vertex> vertices;
        std::vector<Face> faces;
    };
}

#endif