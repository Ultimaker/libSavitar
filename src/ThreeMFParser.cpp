// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/ThreeMFParser.h"
#include "Savitar/Scene.h"

#include <clocale>
#include <sstream>

using namespace Savitar;

ThreeMFParser::ThreeMFParser()
{
    setlocale(LC_ALL, "C");
}

Scene ThreeMFParser::parse(const std::string& xml_string)
{
    pugi::xml_document document;
    document.load_string(xml_string.c_str());
    Scene scene;
    scene.fillByXMLNode(document.child("model"));

    return scene;
}

std::string ThreeMFParser::sceneToString(Scene scene)
{
    pugi::xml_document document;
    pugi::xml_node model_node = document.append_child("model");

    scene.toXmlNode(model_node);

    std::stringstream ss;
    document.save(ss);
    return ss.str();
}
