// Copyright (c) 2022 Ultimaker B.V.
// libSavitar is released under the terms of the LGPLv3 or higher.

#include "Savitar/Namespace.h"

#include <filesystem>
#include <fstream>
#include <pugixml.hpp>
#include <string>

#include <gtest/gtest.h>

namespace xml_namespace
{
class NamespaceTest : public testing::Test
{
public:
    pugi::xml_document document;
    pugi::xml_node main_xml_node;

    void SetUp()
    {
        std::ifstream test_model_file(std::filesystem::path(__FILE__).parent_path().append("namespaces.xml").string());
        if (test_model_file.is_open())
        {
            const std::string xml_string(std::istreambuf_iterator<char>{ test_model_file }, {});

            pugi::xml_parse_result result = document.load_string(xml_string.c_str());
            main_xml_node = document.child("main");
        }
    }
};

TEST_F(NamespaceTest, getAncestralNamespaces)
{
    ASSERT_TRUE(main_xml_node);

    pugi::xml_node node;
    xml_namespace::xmlns_map_t result;

    node = main_xml_node.child("simple");
    result = xml_namespace::getAncestralNamespaces(node);
    ASSERT_EQ(result.size(), 2); // <-- including default namespace
    ASSERT_EQ(result["_a_"].size(), 1);
    ASSERT_EQ(result["_a_"].count("a"), 1);

    node = main_xml_node.child("multiple");
    result = xml_namespace::getAncestralNamespaces(node);
    ASSERT_EQ(result["_a_"].count("a"), 1);
    ASSERT_EQ(result["_b_"].count("b"), 1);
    ASSERT_EQ(result["_c_"].count("c"), 1);

    node = main_xml_node.child("overwrite"); // _before_ overwrite
    result = xml_namespace::getAncestralNamespaces(node);
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result["_b_"].size(), 1);
    ASSERT_EQ(result["_b_"].count("b"), 1);

    node = main_xml_node.child("overwrite").child("sub"); // _after_ overwrite
    result = xml_namespace::getAncestralNamespaces(node);
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result.count("_b_"), 0);
    ASSERT_EQ(result["_c_"].size(), 1);
    ASSERT_EQ(result["_c_"].count("b"), 1);

    node = main_xml_node.child("combine").child("sub");
    result = xml_namespace::getAncestralNamespaces(node);
    ASSERT_EQ(result.size(), 2);
    ASSERT_EQ(result["_s_"].size(), 2);
    ASSERT_EQ(result["_s_"].count("s"), 1);
    ASSERT_EQ(result["_s_"].count("n"), 1);

    node = main_xml_node.child("deep").child("sub");
    result = xml_namespace::getAncestralNamespaces(node);
    ASSERT_EQ(result.size(), 3);
    ASSERT_EQ(result.count("_w_"), 0);
    ASSERT_EQ(result.count("_x_"), 1);

    node = main_xml_node.child("deep").child("sub").child("subber").child("subbest");
    result = xml_namespace::getAncestralNamespaces(node);
    ASSERT_EQ(result.size(), 6);
    ASSERT_EQ(result.count("_w_"), 1);
    ASSERT_EQ(result.count("_x_"), 1);

    node = main_xml_node.child("multideep").child("sub").child("subber").child("subbest");
    result = xml_namespace::getAncestralNamespaces(node);
    ASSERT_EQ(result.size(), 6);
    ASSERT_EQ(result.count("_w_"), 1);
    ASSERT_EQ(result.count("_x_"), 1);

    node = main_xml_node.child("multideep").child("subber").child("subbest");
    result = xml_namespace::getAncestralNamespaces(node);
    ASSERT_EQ(result.size(), 5);
    ASSERT_EQ(result.count("_w_"), 0);
    ASSERT_EQ(result.count("_e_"), 1);
    ASSERT_EQ(result.count("_q_"), 1);
    ASSERT_EQ(result.count("_r_"), 1);

    node = main_xml_node.child("multideep").child("subbest");
    result = xml_namespace::getAncestralNamespaces(node);
    ASSERT_EQ(result.size(), 3);
    ASSERT_EQ(result.count("_e_"), 0);
    ASSERT_EQ(result.count("_q_"), 1);
}

// std::set<std::string> getNamesFor(const xmlns_map_t& map, const std::string& uri);
TEST_F(NamespaceTest, getNamesFor)
{
    ASSERT_TRUE(main_xml_node);
    pugi::xml_node node = main_xml_node.child("multideep").child("sub").child("subber").child("subbest");
    xml_namespace::xmlns_map_t map = xml_namespace::getAncestralNamespaces(node);
    ASSERT_EQ(xml_namespace::getNamesFor(map, "_w_").count("w"), 1);
    ASSERT_EQ(xml_namespace::getNamesFor(map, "_p_").size(), 0);
}

} // namespace xml_namespace
