// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <map>
#include <set>
#include <string>

#include <pugixml.hpp>

namespace xml_namespace
{
using xmlns_map_t = std::map<std::string, std::set<std::string>>;

[[nodiscard]] std::string getCuraUri();
[[nodiscard]] std::string getDefaultUri();

[[nodiscard]] xmlns_map_t getAncestralNamespaces(const pugi::xml_node& xml_node);
[[nodiscard]] std::set<std::string> getNamesFor(const xmlns_map_t& map, const std::string& uri);
} // namespace xml_namespace

#endif