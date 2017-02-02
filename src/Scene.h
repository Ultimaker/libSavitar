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

#ifndef SCENE_H
#define SCENE_H
#include "SavitarExport.h"
#include "SceneNode.h"

#include <vector> // For std::vector
#include <map> // For std::map
#include <string> // For std::string

// Forward declaration
namespace pugi
{
    class xml_node;
}

namespace Savitar
{
    class SAVITAR_EXPORT Scene
    {
    public:
        Scene();
        virtual ~Scene();

        std::vector< SceneNode > getSceneNodes();
        void addSceneNode(SceneNode node);

        /**
         * Set the data of this SceneNode by giving it a xml node
         */
        void fillByXMLNode(pugi::xml_node xml_node);

        void setMetaDataEntry(std::string key, std::string value);

        int getNumSceneNodes();
        SceneNode* getSceneNodeByIndex(int index);

        std::map<std::string, std::string> getMetadata();

        std::string getUnit();

    protected:
        std::vector< SceneNode > scene_nodes;
        std::map<std::string, std::string> metadata;
        std::string unit;
        SceneNode createSceneNodeFromObject(pugi::xml_node root_node, pugi::xml_node object_node);
    };
}
#endif