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

#ifndef THREEMFPARSER_H
#define THREEMFPARSER_H

#include "SavitarExport.h"
#include "SceneNode.h"

#include <string>
namespace Savitar
{
    class Scene;

    class SAVITAR_EXPORT ThreeMFParser
    {
    public:
        ThreeMFParser();
        virtual ~ThreeMFParser();

        /**
         * Parse the string containing xml data that was obtained from 3D/3dmodel.model in a 3mf file
         * \param xml_string the xml string to be parsed by the 3mf parser.
         * \returns A filled Scene object
         */
        Scene parse(std::string xml_string);

        std::string sceneToString(Scene scene);
    };
}
#endif