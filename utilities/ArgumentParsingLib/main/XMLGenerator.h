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

#ifndef __generateXML_h
#define __generateXML_h

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>

namespace cmdlineparsing
{

	class XMLGenerator
	{

	public:
        enum class paramType { INPUT, OUTPUT, BOOLEAN, STRING, INTEGER, DOUBLE, STRINGVECTOR, STRINGENUMERATION };

        XMLGenerator(const std::string& programCategory, const std::string& programName,
            const std::string& programDescription, const std::string& programVersion = "1.0", const std::string& programContributor = "DKFZ");
		/*!
		@brief writes the xml to the stream 
		*/
		void write(std::ostream& stream) const;
        /*!
        @brief sets the basic information parameter map (i.e. that are mandatory)
        @param parameters (key: longFlag) (values: shortFlag, description, optionGroup, default value)
        @sa CmdLineParserBase
        */
        void setBaseParameters(const std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string> >& parameters);
        /*!
        @brief sets the additional information parameter map (i.e. only needed for XML info)
        @param parameters (key: longFlag) (value: parameter type, minimum constraint, maximum constraint, step constraint, vector for string vector data)
        @sa CmdLineParserBase
        */
        void setAdditionalParameters(const std::unordered_map<std::string, std::tuple<paramType, double, double, double, std::vector<std::string> > >& parameters);
		
	private:
		/*!
		@brief finds the different groups
		@return a vector of all found groups
		*/
		std::vector<std::string> getOptionGroups() const;
		/*!
		@brief adds a generic parameter tag to the node
		@param longflag (e.g. foo) 
		@param baseParameterInfo basic information like description and default value
		@param additionalParameterInfo information for xml like type, constraints
        @sa setBaseParameters
		*/
        void addParameterTags(boost::property_tree::ptree& node, const std::string longFlag, const std::tuple<char, std::string, std::string, std::string>& baseParameterInfo, const std::tuple<XMLGenerator::paramType, double, double, double, std::vector<std::string> >& additionalParameterInfo) const;
		/*!
		@brief adds program description to the node
		*/
        void addProgrammDesc(boost::property_tree::ptree& node, const std::string& category, const std::string& name,
            const std::string& description, const std::string& version, const std::string& contributor) const;
		/*!
		@brief adds a group tag to the node
		@detail required for different groups
		*/
        void addGroupTag(boost::property_tree::ptree& node, const std::string& label) const;
		/*!
		@brief creates specific parameter tags (int/double/string-enum etc.)
		*/
        boost::property_tree::ptree createParameterTagWithDefaultValue(const std::string& longflag,
            char flag, const std::string& description, const std::string& defaultValue) const;
		/*!
		@brief adds IO information for file parameter tags
        @param channel either input or output
        @param desiredFileExtensions a list of file extensions, e.g. {"txt", "xml", "jpg"}
		*/
        void addIOTag(boost::property_tree::ptree& node, const std::string& channel, const std::vector<std::string>& desiredFileExtensions) const;
        /*!
        @brief adds constraints to the node
        @param castToInt the double values are cast to int (for integer parameters)
        */
        void addConstraints(boost::property_tree::ptree& node, double minConstraint, double maxConstraint, double stepConstraint, bool castToInt = false) const;
        /*!
        @brief adds enums to the node
        */
        void addEnums(boost::property_tree::ptree& node, std::vector<std::string> vectorData) const;

        std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string> > m_baseParameters;
        std::unordered_map<std::string, std::tuple<paramType, double, double, double, std::vector<std::string> > > m_additionalParameters;

        std::string m_programName, m_programCategory, m_programDescription, m_programVersion, m_programContributor;
		};
}
#endif