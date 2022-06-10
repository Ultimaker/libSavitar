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

#ifndef VERTEX_H
#define VERTEX_H
#include "SavitarExport.h"

namespace Savitar
{
    class SAVITAR_EXPORT Vertex
    {
    public:
        /**
         * A vertex is a point in 3D space.
         */
        Vertex(float x, float y, float z);
        virtual ~Vertex();

        float getX();
        float getY();
        float getZ();
    protected:
        float x;
        float y;
        float z;
    };
}

#endif