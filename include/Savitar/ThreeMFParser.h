// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef THREEMFPARSER_H
#define THREEMFPARSER_H

#include "Savitar/SceneNode.h"
#include <pugixml.hpp>

#include <string>
namespace Savitar
{
class Scene;

class ThreeMFParser
{
public:
    ThreeMFParser();
    virtual ~ThreeMFParser() = default;

    /**
     * Parse the string containing xml data that was obtained from 3D/3dmodel.model in a 3mf file
     * \param xml_string the xml string to be parsed by the 3mf parser.
     * \returns A filled Scene object
     */
    Scene parse(const std::string& xml_string);

    [[nodiscard]] std::string sceneToString(Scene scene);
};
} // namespace Savitar
#endif