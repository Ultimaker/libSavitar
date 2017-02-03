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

%Module(name = Savitar, call_super_init = True)

%Include Types.sip
%Include SceneNode.sip
%Include Scene.sip
%Include MeshData.sip

%ModuleHeaderCode
namespace Savitar{} //Ensure that the namespace already exists
using namespace Savitar;
%End

class ThreeMFParser
{
    %TypeHeaderCode
    #include "ThreeMFParser.h"
    %End

public:
    ThreeMFParser();
    virtual ~ThreeMFParser();
    Scene parse(std::string xml_string);
    std::string sceneToString(Scene scene);
};