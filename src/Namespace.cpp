#include "Namespace.h"

#include "../pugixml/src/pugixml.hpp"

namespace xml_namespace
{
    std::string getCuraUri() { return std::string("http://software.ultimaker.com/xml/cura/3mf/2015/10"); }
    std::string getDefaultUri() { return std::string("http://schemas.microsoft.com/3dmanufacturing/core/2015/02"); }

    void appendNamespaceAttributes(xmlns_map_t& map, const pugi::xml_node& xml_node)
    {
        for (const pugi::xml_attribute& attribute : xml_node.attributes())
        {
            const std::string name = attribute.name();
            const std::string name_start = name.substr(0, name.size() >= 5 ? 5 : std::string::npos);
            if (name_start.compare("xmlns") != 0)
            {
                continue;
            }

            const std::string namespace_name = name.size() <= 5 ? "" : name.substr(6);
            const std::string namespace_glob = attribute.value();
            if (map.count(namespace_glob) < 1)
            {
                map[namespace_glob] = std::set<std::string>();
            }
            map[namespace_glob].insert(namespace_name);
        }
    }

    xmlns_map_t getAncestralNamespaces(const pugi::xml_node& xml_node)
    {
        xmlns_map_t result;
        for (pugi::xml_node current_node = xml_node; current_node; current_node = current_node.parent())
        {
            appendNamespaceAttributes(result, current_node);
        }
        return result;
    }

    std::set<std::string> getNamesFor(const xmlns_map_t& map, const std::string& uri)
    {
        return (map.count(uri) > 0) ? map.at(uri) : std::set<std::string>();
    }

} //namespace xml_namespace
