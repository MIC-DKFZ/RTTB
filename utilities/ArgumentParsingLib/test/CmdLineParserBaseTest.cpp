//------------------------------------------------------------------------
// Copyright(c) German Cancer Research Center(DKFZ),
// Software Development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
// Authors: Sascha Diatschuk, Simon Strubel, Clemens Hentschke, Fabrizio Ciccarese
//			
//------------------------------------------------------------------------

#include <gtest/gtest.h>
#include "CmdLineParserBase.h"
#include "XMLGenerator.h"


// Helper function to facilitate passing a dynamic string array as const char**
std::vector<const char*> stdStringVec2CStringVec(const std::vector<std::string>&
        stringList)
{
	std::vector<const char*> pointers(stringList.size());
	std::transform(stringList.begin(), stringList.end(), pointers.begin(),
	               [](const std::string & s)
	{
		return s.c_str();
	});
	pointers.push_back(0);

	return pointers;
}

// Since CmdLineParserBase is abstract, we need to sub-class it in order to test it.
class CmdLineParserBaseTest :
	public cmdlineparsing::CmdLineParserBase,
	public testing::Test
{
public:
	CmdLineParserBaseTest() : CmdLineParserBase("CmdLineParserBaseTest","1.2","description","contributor","category")
	{
		// Capture stdout
		m_stdCout = std::cout.rdbuf();
		std::cout.rdbuf(m_strCout.rdbuf());
	}
	~CmdLineParserBaseTest()
	{
		// Change stdout back to normal
		std::cout.rdbuf(m_stdCout);
	}

	void validateInput() const {}

	std::ostringstream m_strCout; // Temporary output
	std::streambuf* m_stdCout; // stdout
};

// Since CmdLineParserBase is abstract, we need to sub-class it in order to test it.
class CmdLineParserBaseDefaultTest :
    public cmdlineparsing::CmdLineParserBase,
    public testing::Test
{
public:
    CmdLineParserBaseDefaultTest() : CmdLineParserBase("CmdLineParserBaseTest")
    {
        // Capture stdout
        m_stdCout = std::cout.rdbuf();
        std::cout.rdbuf(m_strCout.rdbuf());
    }
    ~CmdLineParserBaseDefaultTest()
    {
        // Change stdout back to normal
        std::cout.rdbuf(m_stdCout);
    }

    void validateInput() const {}

    std::ostringstream m_strCout; // Temporary output
    std::streambuf* m_stdCout; // stdout
};

TEST_F(CmdLineParserBaseTest, BeforeParse)
{
	// These functions all require parse() to be called before-hand and should all fail
	EXPECT_THROW(isSet(std::string()), cmdlineparsing::AccessException);
	EXPECT_THROW(isSet("test"), cmdlineparsing::AccessException);
	EXPECT_THROW(isSet("config-file"), cmdlineparsing::AccessException);
	EXPECT_THROW(get<std::string>(std::string()), cmdlineparsing::AccessException);
	EXPECT_THROW(get<int>("test"), cmdlineparsing::AccessException);
	EXPECT_THROW(get<bool>("help"), cmdlineparsing::AccessException);
    EXPECT_THROW(get<std::string>("version"), cmdlineparsing::AccessException);
    EXPECT_THROW(get<bool>("xml"), cmdlineparsing::AccessException);
	EXPECT_THROW(get<std::string>("config-file"), cmdlineparsing::AccessException);
}

TEST_F(CmdLineParserBaseDefaultTest, ConstructorDefault)
{
    EXPECT_EQ("CmdLineParserBaseTest", m_programName);
    EXPECT_EQ("1.0", m_programVersion);
    EXPECT_EQ("Generic", m_programCategory);
    EXPECT_EQ("", m_programDescription);
    EXPECT_EQ("DKFZ", m_programContributor);
}

TEST_F(CmdLineParserBaseTest, Constructor)
{
    EXPECT_EQ("CmdLineParserBaseTest", m_programName);
    EXPECT_EQ("1.2", m_programVersion);
    EXPECT_EQ("category", m_programCategory);
    EXPECT_EQ("description", m_programDescription);
    EXPECT_EQ("contributor", m_programContributor);
}

TEST_F(CmdLineParserBaseTest, PrintBasicHelp)
{
	printHelp();
	std::string expected = "Usage: " + m_programName + " [options]\n\n";
	EXPECT_EQ(expected, m_strCout.str());
}

TEST_F(CmdLineParserBaseTest, PrintBasicVersion)
{
	printVersion();
	std::string expected = m_programName + " Version: " + m_programVersion + "\n";
	EXPECT_EQ(expected, m_strCout.str());
}

TEST_F(CmdLineParserBaseTest, AddOption)
{
	addOption(std::string(), std::string(), std::string());
	parse(0, nullptr);
	EXPECT_EQ(5, this->m_cmdLineOptions.options().size());
}

TEST_F(CmdLineParserBaseTest, AddTemplatedOption)
{
    addOption<int>("test", "test", "test");
	parse(0, nullptr);
	EXPECT_EQ(5, this->m_cmdLineOptions.options().size());
}

TEST_F(CmdLineParserBaseTest, AddOptionWithDefaultValue)
{
    addOptionWithDefaultValue<std::string>("test", "test", "test", "test");
	parse(0, nullptr);
	EXPECT_EQ(5, this->m_cmdLineOptions.options().size());
}

TEST_F(CmdLineParserBaseTest, AddInformationForXMLSimple)
{
    addOption<int>("test", "test", "test");
    addInformationForXML("test", cmdlineparsing::XMLGenerator::paramType::STRING);
    parse(0, nullptr);
    EXPECT_EQ(1, this->m_additionalOptionsMap.size());
    EXPECT_EQ(cmdlineparsing::XMLGenerator::paramType::STRING, std::get<0>(this->m_additionalOptionsMap.at("test")));
}

TEST_F(CmdLineParserBaseTest, AddInformationForXMLMinMaxStep)
{
    addOption<int>("test", "test", "test");
    addInformationForXML("test", cmdlineparsing::XMLGenerator::paramType::DOUBLE, 1, 40,2);
    parse(0, nullptr);
    EXPECT_EQ(1, this->m_additionalOptionsMap.size());
    EXPECT_EQ(1, std::get<1>(this->m_additionalOptionsMap.at("test")));
    EXPECT_EQ(40, std::get<2>(this->m_additionalOptionsMap.at("test")));
    EXPECT_EQ(2, std::get<3>(this->m_additionalOptionsMap.at("test")));
}

TEST_F(CmdLineParserBaseTest, AddInformationForXMLVector)
{
    addOption<int>("test", "test", "test");
    addInformationForXML("test", cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, std::vector<std::string>{"eins", "zwei", "drei"});
    parse(0, nullptr);
    EXPECT_EQ(1, this->m_additionalOptionsMap.size());
    std::vector<std::string> testVector{ "eins", "zwei", "drei" };
    EXPECT_EQ(testVector, std::get<4>(this->m_additionalOptionsMap.at("test")));
}

TEST_F(CmdLineParserBaseTest, AddOptionWithDefaultValueVector)
{
	std::vector<std::string> defaultValue = { "first", "second" };
    addOptionWithDefaultValue<std::vector<std::string> >("test", "test", "test", defaultValue,
	        defaultValue.at(0) + " " + defaultValue.at(1));
	parse(0, nullptr);
	EXPECT_EQ(5, this->m_cmdLineOptions.options().size());
}

TEST_F(CmdLineParserBaseTest, ParseInvalidParameter)
{
	std::vector<std::string> argList = { "", "--test" };
	ASSERT_THROW(parse(static_cast<int>(argList.size()),
	                   &stdStringVec2CStringVec(argList)[0]),
	             boost::program_options::unknown_option);
}

TEST_F(CmdLineParserBaseTest, ParseHelp)
{
	std::vector<std::string> argList = { "", "--" + OPTION_HELP };
	ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
	                      &stdStringVec2CStringVec(argList)[0]));
}

TEST_F(CmdLineParserBaseTest, ParseVersion)
{
    std::vector<std::string> argList = { "", "--" + OPTION_VERSION };
    ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
        &stdStringVec2CStringVec(argList)[0]));
}

TEST_F(CmdLineParserBaseTest, ParseXML)
{
    addOptionWithDefaultValue<std::string>("test", "test", "test", "test");
    addInformationForXML("test", cmdlineparsing::XMLGenerator::paramType::STRING);
    std::vector<std::string> argList = { "", "--" + OPTION_XML };
    ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
        &stdStringVec2CStringVec(argList)[0]));
}

TEST_F(CmdLineParserBaseTest, ParseXMLNoInfo)
{
    addOptionWithDefaultValue<std::string>("test", "test", "test", "test");
    std::vector<std::string> argList = { "", "--" + OPTION_XML };
    ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
        &stdStringVec2CStringVec(argList)[0]));
}

TEST_F(CmdLineParserBaseTest, ParseShorthand)
{
	std::vector<std::string> argList = { "", "-h" };
	ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
	                      &stdStringVec2CStringVec(argList)[0]));
}

TEST_F(CmdLineParserBaseTest, ParseCfgFileUnary)
{
	std::vector<std::string> argList = { "", "--" + OPTION_CONFIG_FILE };
	ASSERT_THROW(parse(static_cast<int>(argList.size()),
	                   &stdStringVec2CStringVec(argList)[0]),
	             boost::program_options::invalid_command_line_syntax);
}

TEST_F(CmdLineParserBaseTest, ParseCfgFileSimple)
{
	std::vector<std::string> argList = { "", "--" + OPTION_CONFIG_FILE, "simple.xml" };
	ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
	                      &stdStringVec2CStringVec(argList)[0]));
}

TEST_F(CmdLineParserBaseTest, ParseCfgFileComplex)
{
	const std::string OPTION_MULTITOKEN_NUMBERS = "multitoken-numbers";
	const std::string OPTION_COMPOSING_NUMBERS = "composing-numbers";
	const std::string OPTION_FLAG = "flag";
	const std::string OPTION_OUTPUT_FILE = "output-file";
	const int cmdComposedNumbers[2] = { 100, 200 };

    addOption<std::vector<int> >(OPTION_MULTITOKEN_NUMBERS, "", "", 0, false, true);
    addOption<std::vector<int> >(OPTION_COMPOSING_NUMBERS, "", "", 0, false, true, true);
    addOption(OPTION_FLAG, "", "");
    addOption<std::string>(OPTION_OUTPUT_FILE, "", "");

	std::string num1 = std::to_string(cmdComposedNumbers[0]);
	std::string num2 = std::to_string(cmdComposedNumbers[1]);

	std::vector<std::string> argList =
	{
		"", "--" + OPTION_CONFIG_FILE, "sample.xml", "--" + OPTION_COMPOSING_NUMBERS,
		std::to_string(cmdComposedNumbers[0]), std::to_string(cmdComposedNumbers[1])
	};
	ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
	                      &stdStringVec2CStringVec(argList)[0]));

	std::vector<int> multitokenNumbers = get<std::vector<int> >("multitoken-numbers");
	EXPECT_EQ(2, multitokenNumbers.size());
	EXPECT_EQ(99, multitokenNumbers[0]);
	EXPECT_EQ(999, multitokenNumbers[1]);

	std::vector<int> composingNumbers = get<std::vector<int> >("composing-numbers");
	EXPECT_EQ(5, composingNumbers.size());
	EXPECT_EQ(cmdComposedNumbers[0], composingNumbers[0]);
	EXPECT_EQ(cmdComposedNumbers[1], composingNumbers[1]);
	EXPECT_EQ(10, composingNumbers[2]);
	EXPECT_EQ(20, composingNumbers[3]);
	EXPECT_EQ(30, composingNumbers[4]);

	EXPECT_TRUE(isSet("flag"));

	EXPECT_EQ("out.txt", get<std::string>("output-file"));
}

TEST_F(CmdLineParserBaseTest, ParseRequiredMissing)
{
	const std::string OPTION_REQUIRED = "input-file";

    addOption<std::string>(OPTION_REQUIRED, "", "", 0, true);

	std::vector<std::string> argList = { "" };
	ASSERT_THROW(parse(static_cast<int>(argList.size()),
	                   &stdStringVec2CStringVec(argList)[0]),
	             boost::program_options::required_option);
}

TEST_F(CmdLineParserBaseTest, ParseInt)
{
	const std::string OPTION_NUMBER = "number";
	const int number = 3;

    addOption<int>(OPTION_NUMBER, "", "");

	std::vector<std::string> argList = {"", "--" + OPTION_NUMBER, std::to_string(number)};
	ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
	                      &stdStringVec2CStringVec(argList)[0]));

	EXPECT_EQ(number, get<int>(OPTION_NUMBER));
}

TEST_F(CmdLineParserBaseTest, ParseBool)
{
	const std::string OPTION_FLAG = "flag";

    addOption(OPTION_FLAG, "", "");

	std::vector<std::string> argList = { "", "--" + OPTION_FLAG };
	ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
	                      &stdStringVec2CStringVec(argList)[0]));

	EXPECT_TRUE(isSet(OPTION_FLAG));
}

TEST_F(CmdLineParserBaseTest, ParseDouble)
{
	const std::string OPTION_DOUBLE = "double";
	const double d = 3.14159266e114;

    addOption<double>(OPTION_DOUBLE, "", "");

	std::vector<std::string> argList = { "", "--" + OPTION_DOUBLE, std::to_string(d) };
	ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
	                      &stdStringVec2CStringVec(argList)[0]));

	EXPECT_DOUBLE_EQ(d, get<double>(OPTION_DOUBLE));
}

TEST_F(CmdLineParserBaseTest, ParseString)
{
	const std::string OPTION_STRING = "string";
	const std::string s = "test";

    addOption<std::string>(OPTION_STRING, "", "");

	std::vector<std::string> argList = { "", "--" + OPTION_STRING, s };
	ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
	                      &stdStringVec2CStringVec(argList)[0]));

	EXPECT_EQ(s, get<std::string>(OPTION_STRING));
}

TEST_F(CmdLineParserBaseTest, ParseComposingNumbers)
{
	const std::string OPTION_COMPOSING_NUMBERS = "composing";
	std::vector<int> numbers = { 1, 2, 3 };

    addOption<std::vector<int> >(OPTION_COMPOSING_NUMBERS, "", "", 0, false, true);

	std::vector<std::string> argList = { "", "--" + OPTION_COMPOSING_NUMBERS };

	for (int i : numbers)
	{
		argList.push_back(std::to_string(i));
	}

	ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
	                      &stdStringVec2CStringVec(argList)[0]));

	std::vector<int> results = get<std::vector<int> >(OPTION_COMPOSING_NUMBERS);
	EXPECT_EQ(numbers.size(), results.size());

	for (int i = 0; i < numbers.size(); ++i)
	{
		EXPECT_EQ(numbers[i], results[i]);
	}
}

TEST_F(CmdLineParserBaseTest, ParseDefaultValue)
{
	const std::string OPTION_NUMBER = "number";
	const int defaultValue = 12;

    addOptionWithDefaultValue<int>(OPTION_NUMBER, "", "", defaultValue);

	std::vector<std::string> argList = { "" };
	ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
	                      &stdStringVec2CStringVec(argList)[0]));

	EXPECT_EQ(defaultValue, get<int>(OPTION_NUMBER));
}

TEST_F(CmdLineParserBaseTest, ParsePositionalOptions)
{
	const std::string OPTION_OUTPUT_FILE = "output-file";
	const std::string OPTION_INPUT_FILES = "input-files";
	const std::string outputFile = "out.txt";
	const std::vector<std::string> inputFiles = { "a.txt", "b.txt", "c.txt" };

    addOption<std::string>(OPTION_OUTPUT_FILE, "", "", 0,  true);
    addOption<std::vector<std::string> >(OPTION_INPUT_FILES, "", "", 0, true, true);

	addPositionalOption(OPTION_OUTPUT_FILE, 1);
	addPositionalOption(OPTION_INPUT_FILES, -1);

	std::vector<std::string> argList = { "", outputFile };

	for (const std::string& s : inputFiles)
	{
		argList.push_back(s);
	}

	ASSERT_NO_THROW(parse(static_cast<int>(argList.size()),
	                      &stdStringVec2CStringVec(argList)[0]));

	EXPECT_EQ(outputFile, get<std::string>(OPTION_OUTPUT_FILE));

	std::vector<std::string> results = get<std::vector<std::string> >(OPTION_INPUT_FILES);
	EXPECT_EQ(inputFiles.size(), results.size());

	for (int i = 0; i < results.size(); ++i)
	{
		EXPECT_EQ(inputFiles[i], results[i]);
	}
}

TEST_F(CmdLineParserBaseTest, AddDuplicatedOptions)
{
    ASSERT_THROW(addOption("help", "", "", 'i'), boost::program_options::duplicate_option_error);
    ASSERT_THROW(addOption("sdsa", "", "", 'h'), boost::program_options::duplicate_option_error);
    ASSERT_NO_THROW(addOption("something", "", "", 'y'));
    ASSERT_THROW(addOption("something", "", "", 's'), boost::program_options::duplicate_option_error);
    ASSERT_THROW(addOption("hhh", "", "", 'y'), boost::program_options::duplicate_option_error);
    ASSERT_NO_THROW(addOption("noShorthand", "", ""));
    ASSERT_NO_THROW(addOption("noShorthand2", "", ""));
    ASSERT_THROW(addOption<std::string>("option_name", "", "", 'y',  false, true, true), boost::program_options::duplicate_option_error);
    ASSERT_NO_THROW(addOption<std::vector<int>>("option_name2", "", "", 'm',  false, true, true));
    ASSERT_THROW(addOption<std::string>("option_name2", "", "", 'f',  false, true, true), boost::program_options::duplicate_option_error);
    ASSERT_THROW(addOption<std::string>("option_name2", "", "", false,  true, true), boost::program_options::duplicate_option_error);
    ASSERT_THROW(addOptionWithDefaultValue<std::string>("help", "", "", "",  "", 'n', false, true, true),
		boost::program_options::duplicate_option_error);
    ASSERT_THROW(addOptionWithDefaultValue<std::string>("help", "", "", "", "", false, true, true),
		boost::program_options::duplicate_option_error);
}
