//Copyright (c) 2020 Ultimaker B.V.
//libSavitar is released under the terms of the AGPLv3 or higher.

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

    xmlns_map_t getAncestralNamespaces(const pugi::xml_node& xml_node);
    std::set<std::string> getNamesFor(const xmlns_map_t& map, const std::string& uri);
} //namespace xml_namespace
