// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/ThreeMFParser.h"
#include "Savitar/Scene.h"
#include "Savitar/SceneNode.h"

#include <gtest/gtest.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <string>

namespace Savitar
{

/*
 * Fixture that loads a testing XML model and gives an instance of the parser to
 * test with.
 */
class ThreeMFParserTest : public testing::Test
{
public:
    std::string xml_string;
    ThreeMFParser* parser;

    void SetUp() override
    {
        xml_string = "";
        std::ifstream test_model_file(std::filesystem::path(__FILE__).parent_path().append("test_model.xml.test").string());
        if (test_model_file.is_open())
        {
            xml_string = std::string(std::istreambuf_iterator<char>{ test_model_file }, {});
        }

        parser = new ThreeMFParser();
    }

    void TearDown() override
    {
        delete parser;
    }
};

TEST_F(ThreeMFParserTest, parse)
{
    ASSERT_FALSE(xml_string.empty());

    Scene scene;
    ASSERT_NO_THROW(scene = parser->parse(xml_string));

    std::vector<SceneNode*> nodes = scene.getSceneNodes();
    ASSERT_FALSE(nodes.empty());
    ASSERT_EQ(nodes.size(), 4UL);

    std::array<size_t, 4> expected_verts = { 36UL, 8UL, 0UL, 0UL };
    std::array<size_t, 4> expected_tris = { 144UL, 144UL, 0UL, 0UL };
    std::array<size_t, 4> expected_child = { 0UL, 0UL, 1UL, 1UL };
    int i = -1;
    for (SceneNode* node : nodes)
    {
        ++i;
        MeshData& data = node->getMeshData();

        const std::vector<Vertex>& verts = data.getVertices();
        EXPECT_EQ(verts.size(), expected_verts[i]);

        const bytearray& tris = data.getFacesAsBytes();
        EXPECT_EQ(tris.size(), expected_tris[i]);

        const std::vector<SceneNode*>& children = node->getAllChildren();
        ASSERT_EQ(children.size(), expected_child[i]);
        for (SceneNode* child : children)
        {
            data = child->getMeshData();
            EXPECT_FALSE(data.getVertices().empty());
            EXPECT_FALSE(data.getFacesAsBytes().empty());
        }
    }
    // NOTE: To/from for content of vertices/triangles is tested in MeshDataTest.

    std::map<std::string, MetadataEntry> settings;

    settings = nodes[0]->getSettings();
    EXPECT_NE(settings.find("extruder_nr"), settings.end());
    EXPECT_EQ(settings.at("extruder_nr").value.compare("0"), 0);
    EXPECT_NE(settings.find("bottom_layers"), settings.end());
    EXPECT_EQ(settings.at("bottom_layers").value.compare("20"), 0);
    EXPECT_EQ(settings.find("infill_pattern"), settings.end());

    settings = nodes[1]->getSettings();
    EXPECT_TRUE(settings.empty());

    settings = nodes[2]->getSettings();
    EXPECT_NE(settings.find("extruder_nr"), settings.end());
    EXPECT_EQ(settings.at("extruder_nr").value.compare("1"), 0);
    EXPECT_EQ(settings.find("bottom_layers"), settings.end());
    EXPECT_NE(settings.find("infill_pattern"), settings.end());
    EXPECT_EQ(settings.at("infill_pattern").value.compare("concentric"), 0);

    EXPECT_EQ(nodes[0]->getName().compare("test_object"), 0);
    EXPECT_EQ(nodes[1]->getName().compare(""), 0);
    EXPECT_EQ(nodes[1]->getId().compare("2"), 0);
    EXPECT_FALSE(nodes[1]->getTransformation().empty());
}

TEST_F(ThreeMFParserTest, sceneToString)
{
    ASSERT_FALSE(xml_string.empty());
    Scene scene = parser->parse(xml_string);

    const std::string scene_string = parser->sceneToString(scene);
    EXPECT_FALSE(scene_string.empty());
}

TEST_F(ThreeMFParserTest, decimalSeparatorTest)
{
    // Don't accept a model that uses ','s for decimal separators.
    std::string xml_string_septest;
    std::ifstream test_model_file(std::filesystem::path(__FILE__).parent_path().append("problem_model.xml.test").string());
    if (test_model_file.is_open())
    {
        xml_string_septest = std::string(std::istreambuf_iterator<char>{ test_model_file }, {});
    }
    ThreeMFParser parser_septest;

    Scene scene;
    ASSERT_THROW(scene = parser_septest.parse(xml_string_septest), std::runtime_error);
}

} // namespace Savitar
