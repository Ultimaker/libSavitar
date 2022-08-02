// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

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

    MetadataEntry(const std::string& value, const std::string& type = "xs:string", const bool preserve = false) : value(value), type(type), preserve(preserve)
    {
    }
};

} // namespace Savitar

#endif