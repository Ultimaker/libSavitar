/*
 * This file is part of libSavitar
 *
 * Copyright (C) 2017 Ultimaker b.v. <j.vankessel@ultimaker.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FACE_H
#define FACE_H

#include "SavitarExport.h"

class SAVITAR_EXPORT Face
{
public:
    /**
     * A face uses the index of 3 vertices to describe a triangle
     */
    Face(int v1, int v2, int v3);
    ~Face();

    int getV1();
    int getV2();
    int getV3();

protected:
    int vertex_1_index;
    int vertex_2_index;
    int vertex_3_index;
};

#endif // FACE_H
