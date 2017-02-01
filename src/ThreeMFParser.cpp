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

#include "ThreeMFParser.h"
#include "Scene.h"
#include <iostream>

using namespace Savitar;

ThreeMFParser::ThreeMFParser()
{

}

ThreeMFParser::~ThreeMFParser()
{

}

Scene ThreeMFParser::parse(std::string xml_string)
{
    pugi::xml_document document;
    pugi::xml_parse_result result = document.load_string(xml_string.c_str());

    Scene scene;

    scene.fillByXMLNode(document.child("model"));


    return scene;
}