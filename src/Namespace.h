//Copyright (c) 2020 Ultimaker B.V.
//libSavitar is released under the terms of the AGPLv3 or higher.

#ifndef NAMESPACE_H
#define NAMESPACE_H

#include "SavitarExport.h"
#include <map>
#include <set>
#include <string>

// Forward declaration
namespace pugi
{
    class xml_node;
}

namespace xml_namespace
{
    typedef std::map<std::string, std::set<std::string>> xmlns_map_t;

    std::string getCuraUri();
    std::string getDefaultUri();

    xmlns_map_t SAVITAR_EXPORT getAncestralNamespaces(const pugi::xml_node& xml_node);
    std::set<std::string> SAVITAR_EXPORT getNamesFor(const xmlns_map_t& map, const std::string& uri);
} //namespace xml_namespace

#endif