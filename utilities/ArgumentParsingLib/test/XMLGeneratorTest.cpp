//------------------------------------------------------------------------
// Copyright(c) German Cancer Research Center(DKFZ),
// Software Development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
// Authors: Simon Strubel, Clemens Hentschke
//------------------------------------------------------------------------

#include <gtest/gtest.h>

#include <fstream>
#include "XMLGenerator.h"


class XMLGeneratorTest :
	public testing::Test
{
private:
    std::ostringstream m_strStream;
    std::streambuf* m_stdStream;
public:
    std::shared_ptr<cmdlineparsing::XMLGenerator> generator;

    void SetUp() override{
        generator = std::make_shared<cmdlineparsing::XMLGenerator>("aCategory", "aName", "aDescription");
    }

    std::string readFileContents(const std::string& filename) const{
        std::ifstream fileStream(filename);
        std::string content((std::istreambuf_iterator<char>(fileStream)),(std::istreambuf_iterator<char>()));
        return content;
    }

    void redirectStreamToString(std::ostream& os){
        m_stdStream = os.rdbuf();
        os.rdbuf(m_strStream.rdbuf());
    }
    std::string getContentOfStream() const {
        return m_strStream.str();
    }

    std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string>> createBaseParametersMap(const std::string& key, char shortFlag, const std::string& description, const std::string& optionGroup, const std::string& defaultValue) const{
        std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string>> baseParameters;
        auto baseArguments = std::make_tuple(shortFlag, description, optionGroup, defaultValue);
        baseParameters.emplace(key, baseArguments);
        return baseParameters;
    }

    std::unordered_map<std::string, std::tuple<cmdlineparsing::XMLGenerator::paramType, double, double, double, std::vector<std::string> > > createAdditionalParametersMap(const std::string& key, cmdlineparsing::XMLGenerator::paramType parameterType, double minimumConstraint=0.0, double maximumConstraint=100.0, double stepConstraint=1.0, const std::vector<std::string>& aVector = std::vector<std::string>()) const{
        std::unordered_map<std::string, std::tuple<cmdlineparsing::XMLGenerator::paramType, double, double, double, std::vector<std::string> > > additionalParameters;
        auto additionalArguments = std::make_tuple(parameterType, minimumConstraint, maximumConstraint, stepConstraint, aVector);
        additionalParameters.emplace(key, additionalArguments);
        return additionalParameters;
    }

};

TEST_F(XMLGeneratorTest, TestXMLDouble)
{
    auto baseParameters = createBaseParametersMap("aDouble", 'd', "Test Description", "TestGroup", "42");
    auto additionalParameters = createAdditionalParametersMap("aDouble", cmdlineparsing::XMLGenerator::paramType::DOUBLE, 0.0, 100.0, 1.0);

    generator->setBaseParameters(baseParameters);
    generator->setAdditionalParameters(additionalParameters);

    redirectStreamToString(std::cout);
    generator->write(std::cout);
    auto expectedContent = readFileContents("XMLDouble.xml");
    auto acutalContent = getContentOfStream();

    EXPECT_EQ(expectedContent, acutalContent);
}

TEST_F(XMLGeneratorTest, TestXMLBoolean)
{
    auto baseParameters = createBaseParametersMap("aBoolean", 'b', "Test Description", "TestGroup", "true");
    auto additionalParameters = createAdditionalParametersMap("aBoolean", cmdlineparsing::XMLGenerator::paramType::BOOLEAN);

    generator->setBaseParameters(baseParameters);
    generator->setAdditionalParameters(additionalParameters);

    redirectStreamToString(std::cout);
    generator->write(std::cout);
    auto expectedContent = readFileContents("XMLBoolean.xml");
    auto acutalContent = getContentOfStream();

    EXPECT_EQ(expectedContent, acutalContent);
}

TEST_F(XMLGeneratorTest, TestXMLInput)
{
    auto baseParameters = createBaseParametersMap("anInput", 'i', "Test Description", "TestGroup", "input.txt");
    auto additionalParameters = createAdditionalParametersMap("anInput", cmdlineparsing::XMLGenerator::paramType::INPUT, 0.0, 0.0, 0.0, std::vector<std::string>{"txt", "png"});

    generator->setBaseParameters(baseParameters);
    generator->setAdditionalParameters(additionalParameters);

    redirectStreamToString(std::cout);
    generator->write(std::cout);
    auto expectedContent = readFileContents("XMLInput.xml");
    auto acutalContent = getContentOfStream();

    EXPECT_EQ(expectedContent, acutalContent);
}

TEST_F(XMLGeneratorTest, TestXMLOutput)
{
    auto baseParameters = createBaseParametersMap("anOutput", 'o', "Test Description", "TestGroup", "output.xml");
    auto additionalParameters = createAdditionalParametersMap("anOutput", cmdlineparsing::XMLGenerator::paramType::OUTPUT, 0.0, 0.0, 0.0, std::vector<std::string>{"xml"});

    generator->setBaseParameters(baseParameters);
    generator->setAdditionalParameters(additionalParameters);

    redirectStreamToString(std::cout);
    generator->write(std::cout);
    auto expectedContent = readFileContents("XMLOutput.xml");
    auto acutalContent = getContentOfStream();

    EXPECT_EQ(expectedContent, acutalContent);
}

TEST_F(XMLGeneratorTest, TestXMLString)
{
    auto baseParameters = createBaseParametersMap("aString", 's', "Test Description", "TestGroup", "blabla");
    auto additionalParameters = createAdditionalParametersMap("aString", cmdlineparsing::XMLGenerator::paramType::STRING);

    generator->setBaseParameters(baseParameters);
    generator->setAdditionalParameters(additionalParameters);

    redirectStreamToString(std::cout);
    generator->write(std::cout);
    auto expectedContent = readFileContents("XMLString.xml");
    auto acutalContent = getContentOfStream();

    EXPECT_EQ(expectedContent, acutalContent);
}

TEST_F(XMLGeneratorTest, TestXMLInteger)
{
    auto baseParameters = createBaseParametersMap("anInt", 'i', "Test Description", "TestGroup", "5");
    auto additionalParameters = createAdditionalParametersMap("anInt", cmdlineparsing::XMLGenerator::paramType::INTEGER, 0, 10, 2);

    generator->setBaseParameters(baseParameters);
    generator->setAdditionalParameters(additionalParameters);

    redirectStreamToString(std::cout);
    generator->write(std::cout);
    auto expectedContent = readFileContents("XMLInteger.xml");
    auto acutalContent = getContentOfStream();

    EXPECT_EQ(expectedContent, acutalContent);
}

TEST_F(XMLGeneratorTest, TestXMLStringEnum)
{
    auto baseParameters = createBaseParametersMap("aStringEnum", 'e', "Test Description", "TestGroup", "option1");
    auto additionalParameters = createAdditionalParametersMap("aStringEnum", cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, 0, 0, 0, std::vector<std::string>{"option1","option2","option3"});

    generator->setBaseParameters(baseParameters);
    generator->setAdditionalParameters(additionalParameters);

    redirectStreamToString(std::cout);
    generator->write(std::cout);
    auto expectedContent = readFileContents("XMLStringEnum.xml");
    auto acutalContent = getContentOfStream();

    EXPECT_EQ(expectedContent, acutalContent);
}

TEST_F(XMLGeneratorTest, TestXMLStringVector)
{
    auto baseParameters = createBaseParametersMap("aStringVector", 'v', "Test Description", "TestGroup", "foo,bar,foobar");
    auto additionalParameters = createAdditionalParametersMap("aStringVector", cmdlineparsing::XMLGenerator::paramType::STRINGVECTOR);

    generator->setBaseParameters(baseParameters);
    generator->setAdditionalParameters(additionalParameters);

    redirectStreamToString(std::cout);
    generator->write(std::cout);
    auto expectedContent = readFileContents("XMLStringVector.xml");
    auto acutalContent = getContentOfStream();

    EXPECT_EQ(expectedContent, acutalContent);
}

TEST_F(XMLGeneratorTest, TestXMLComplex)
{
    auto baseParametersInput = createBaseParametersMap("anInput", 'i', "Test Description", "TestGroup", "input.txt");
    auto additionalParametersInput = createAdditionalParametersMap("anInput", cmdlineparsing::XMLGenerator::paramType::INPUT, 0, 0, 0, std::vector<std::string>{"txt"});
    auto baseParametersOutput = createBaseParametersMap("anOutput", 'o', "Test Description", "TestGroup", "output.xml");
    auto additionalParametersOutput = createAdditionalParametersMap("anOutput", cmdlineparsing::XMLGenerator::paramType::OUTPUT, 0, 0, 0, std::vector<std::string>{"xml"});
    auto baseParametersDouble = createBaseParametersMap("aDouble", 'd', "Test Description", "TestGroup", "42");
    auto additionalParametersDouble = createAdditionalParametersMap("aDouble", cmdlineparsing::XMLGenerator::paramType::DOUBLE, 0, 100, 1);
    auto baseParametersStringVector = createBaseParametersMap("aStringVector", 'v', "Test Description", "TestGroup", "foo,bar,foobar");
    auto additionalParametersStringVector = createAdditionalParametersMap("aStringVector", cmdlineparsing::XMLGenerator::paramType::STRINGVECTOR);
    auto baseParametersStringEnum = createBaseParametersMap("aStringEnum", 'e', "Test Description", "TestGroup", "option1");
    auto additionalParametersStringEnum = createAdditionalParametersMap("aStringEnum", cmdlineparsing::XMLGenerator::paramType::STRINGENUMERATION, 0, 0, 0, std::vector<std::string>{"option1","option2", "option3"});

    auto baseParametersCombined = baseParametersInput;
    baseParametersCombined.insert(baseParametersOutput.begin(), baseParametersOutput.end());
    baseParametersCombined.insert(baseParametersDouble.begin(), baseParametersDouble.end());
    baseParametersCombined.insert(baseParametersStringVector.begin(), baseParametersStringVector.end());
    baseParametersCombined.insert(baseParametersStringEnum.begin(), baseParametersStringEnum.end());

    auto additionalParametersCombined = additionalParametersInput;
    additionalParametersCombined.insert(additionalParametersOutput.begin(), additionalParametersOutput.end());
    additionalParametersCombined.insert(additionalParametersDouble.begin(), additionalParametersDouble.end());
    additionalParametersCombined.insert(additionalParametersStringVector.begin(), additionalParametersStringVector.end());
    additionalParametersCombined.insert(additionalParametersStringEnum.begin(), additionalParametersStringEnum.end());

    generator->setBaseParameters(baseParametersCombined);
    generator->setAdditionalParameters(additionalParametersCombined);

    redirectStreamToString(std::cout);
    generator->write(std::cout);
    auto expectedContent = readFileContents("XMLComplex.xml");
    auto acutalContent = getContentOfStream();

    EXPECT_EQ(expectedContent, acutalContent);
}

