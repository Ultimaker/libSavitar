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

class MeshData
{
    %TypeHeaderCode
    #include "MeshData.h"
    %End
public:
    MeshData();
    virtual ~MeshData();
    std::vector<uint8_t> getVerticesAsBytes();
    std::vector<uint8_t> getFacesAsBytes();

    std::vector<uint8_t> getFlatVerticesAsBytes();

    void setVerticesFromBytes(const std::vector<uint8_t> &data);

    void setFacesFromBytes(const std::vector<uint8_t> &data);
};