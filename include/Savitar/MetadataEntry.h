/*
 * This file is part of libSavitar
 *
 * Copyright (C) 2021 Ultimaker B.V. <j.vankessel@ultimaker.com>
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

#ifndef METADATAENTRY_H
#define METADATAENTRY_H

#include <string>

namespace Savitar
{

/*!
 * Represents an entry of metadata from 3MF documents.
 *
 * These entries are stored by a key in scenes and scene nodes.
 */
struct MetadataEntry
{
    /*!
     * The value of the metadata entry.
     *
     * Regardless of the actual type, this value will be stored in serialised
     * form here. The user of the 3MF data will need to interpret the data as
     * is.
     */
    std::string value;

    /*!
     * The type of value. Must be an "XML type" as specified by the ODF, e.g.
     * "xs:string" for text.
     */
    std::string type;

    /*!
     * Whether the data should be preserved when saving the scene to a new 3MF
     * document.
     */
    bool preserve;

    MetadataEntry(const std::string& value, const std::string& type = "xs:string", const bool preserve = false)
        : value(value)
        , type(type)
        , preserve(preserve)
    {}
};

}

#endif