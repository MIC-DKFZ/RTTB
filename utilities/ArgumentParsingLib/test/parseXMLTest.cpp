//------------------------------------------------------------------------
// Copyright(c) German Cancer Research Center(DKFZ),
// Software Development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
// Author: Sascha Diatschuk
//------------------------------------------------------------------------

#include <gtest/gtest.h>
#include <boost/property_tree/xml_parser.hpp>
#include "parseXML.h"

class ParseXMLTest : public testing::Test
{
protected:
	boost::program_options::options_description ruleSet;
};

TEST_F(ParseXMLTest, InvalidFileName)
{
	EXPECT_THROW(cmdlineparsing::parseXML(std::string(), ruleSet),
	             boost::program_options::reading_file);
}

TEST_F(ParseXMLTest, NoOptionsWithLongName)
{
	boost::shared_ptr<boost::program_options::option_description> rule(new
	        boost::program_options::option_description);
	ruleSet.add(rule);
	EXPECT_THROW(cmdlineparsing::parseXML(std::string(), ruleSet),
	             boost::program_options::error);
}

TEST_F(ParseXMLTest, NotWellFormedXML)
{
	EXPECT_THROW(cmdlineparsing::parseXML("notwellformed1.xml", ruleSet),
	             boost::property_tree::xml_parser_error);
	EXPECT_THROW(cmdlineparsing::parseXML("notwellformed2.xml", ruleSet),
	             boost::property_tree::xml_parser_error);
	EXPECT_THROW(cmdlineparsing::parseXML("notwellformed3.xml", ruleSet),
	             boost::property_tree::xml_parser_error);
}

TEST_F(ParseXMLTest, InvalidXML)
{
	EXPECT_THROW(cmdlineparsing::parseXML("invalid.xml", ruleSet),
	             boost::property_tree::ptree_bad_path);
}

TEST_F(ParseXMLTest, EmptyParameterList)
{
	EXPECT_TRUE(cmdlineparsing::parseXML("empty.xml", ruleSet).options.empty());
}

TEST_F(ParseXMLTest, NotInRuleSet)
{
	EXPECT_TRUE(cmdlineparsing::parseXML("working.xml", ruleSet).options.empty());
}

TEST_F(ParseXMLTest, WorkingFile)
{
	ruleSet.add_options()
	("multitoken-numbers", boost::program_options::value<std::vector<int> >()->multitoken())
	("flag", "")
	("output-file", boost::program_options::value<std::string>());

	boost::program_options::parsed_options results =
	    cmdlineparsing::parseXML("working.xml", ruleSet);

	// Parsed options don't have their specific type yet.
	EXPECT_EQ("multitoken-numbers", results.options[0].string_key);
	EXPECT_EQ(2, results.options[0].value.size());
	EXPECT_EQ("99", results.options[0].value[0]);
	EXPECT_EQ("999", results.options[0].value[1]);

	EXPECT_EQ("flag", results.options[1].string_key);
	EXPECT_TRUE(results.options[1].value.empty());

	EXPECT_EQ("output-file", results.options[2].string_key);
	EXPECT_EQ(1, results.options[2].value.size());
	EXPECT_EQ("out.txt", results.options[2].value[0]);
}