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

class Scene
{
    %TypeHeaderCode
    #include "Scene.h"
    %End
public:
    Scene();
    virtual ~Scene();

    std::vector< SceneNode *> getSceneNodes();
    std::vector<SceneNode*> getAllSceneNodes();

    void addSceneNode(SceneNode* node /Transfer/);

    std::map<std::string, std::string> getMetadata();
    void setMetaDataEntry(std::string key, std::string value);

    std::string getUnit();
    void setUnit(std::string unit);
};

