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

#include "XMLGenerator.h"

namespace cmdlineparsing
{

    XMLGenerator::XMLGenerator(const std::string& programCategory, const std::string& programName,
        const std::string& programDescription, const std::string& programVersion, const std::string& programContributor) : m_programCategory(programCategory), m_programName(programName), m_programDescription(programDescription), m_programVersion(programVersion), m_programContributor(programContributor)
	{
	}

    void XMLGenerator::write(std::ostream& stream) const
	{
		boost::property_tree::ptree root;
		addProgrammDesc(root, m_programCategory, m_programName, m_programDescription, m_programVersion, m_programContributor);

		std::vector<std::string> groups = getOptionGroups();

		for (const auto& aGroup : groups)
		{
			boost::property_tree::ptree& option = root.add("executable.parameters", "");
            addGroupTag(option, aGroup);
            for (const auto& keyValue : m_baseParameters){
                if (aGroup == std::get<2>(keyValue.second))
				{
                    if (m_additionalParameters.find(keyValue.first) != m_additionalParameters.end()){
                        addParameterTags(option, keyValue.first, keyValue.second, m_additionalParameters.at(keyValue.first));
                    }
				}
			}
		}
		boost::property_tree::write_xml(stream, root, boost::property_tree::xml_writer_make_settings<std::string>(' ', 4));
	}

	std::vector<std::string> XMLGenerator::getOptionGroups() const
	{
		std::vector<std::string> optionGroup; 

		for (const auto& keyValue : m_baseParameters)
		{
			if (std::get<2>(keyValue.second) != "Generic Arguments")
			{
                optionGroup.push_back(std::get<2>(keyValue.second));
			}
		}
        //remove duplicates
        optionGroup.erase(unique(optionGroup.begin(), optionGroup.end()), optionGroup.end());
		return optionGroup;
	}

    void XMLGenerator::addParameterTags(boost::property_tree::ptree& node, const std::string longFlag, const std::tuple<char, std::string, std::string, std::string>& baseParameterInfo, const std::tuple<XMLGenerator::paramType, double, double, double, std::vector<std::string> >& additionalParameterInfo) const
	{
        char shortFlag;
        std::string description;
        std::string optionGroup;
        std::string defaultValue;
        paramType aType;
        double minConstraint;
        double maxConstraint;
        double stepConstraint;
        std::vector<std::string> vectorData;
        std::tie(shortFlag, description, optionGroup, defaultValue) = baseParameterInfo;
        std::tie(aType, minConstraint, maxConstraint, stepConstraint, vectorData) = additionalParameterInfo;

        boost::property_tree::ptree subnode;
        switch (aType){
        case paramType::BOOLEAN:
            subnode = createParameterTagWithDefaultValue(longFlag, shortFlag, description, defaultValue);
            node.add_child("boolean", subnode);
            break;
        case paramType::DOUBLE:
            subnode = createParameterTagWithDefaultValue(longFlag, shortFlag, description, defaultValue);
            addConstraints(subnode, minConstraint, maxConstraint, stepConstraint);
            node.add_child("double", subnode);
            break;
        case paramType::INPUT:
            subnode = createParameterTagWithDefaultValue(longFlag, shortFlag, description, defaultValue);
            addIOTag(subnode, "input", vectorData);
            node.add_child("file", subnode);
            break;
        case paramType::INTEGER:
            subnode = createParameterTagWithDefaultValue(longFlag, shortFlag, description, defaultValue);
            addConstraints(subnode, minConstraint, maxConstraint, stepConstraint, true);
            node.add_child("integer", subnode);
            break;
        case paramType::OUTPUT:
            subnode = createParameterTagWithDefaultValue(longFlag, shortFlag, description, defaultValue);
            addIOTag(subnode, "output", vectorData);
            node.add_child("file", subnode);
            break;
        case paramType::STRING:
            subnode = createParameterTagWithDefaultValue( longFlag, shortFlag, description, defaultValue);
            node.add_child("string", subnode);
            break;
        case paramType::STRINGENUMERATION:
            subnode = createParameterTagWithDefaultValue(longFlag, shortFlag, description, defaultValue);
            addEnums(subnode, vectorData);
            node.add_child("string-enumeration", subnode);
            break;
        case paramType::STRINGVECTOR:
            subnode = createParameterTagWithDefaultValue(longFlag, shortFlag, description, defaultValue);
            node.add_child("string-vector", subnode);
            break;
        default:
            throw std::runtime_error("unknown parameter type");
            break;
        }
	}

    void XMLGenerator::addProgrammDesc(boost::property_tree::ptree& node, const std::string& category, const std::string& name,
        const std::string& description, const std::string& version, const std::string& contributor) const
	{
		node.add("executable.category", category);
		node.add("executable.title", name);
		node.add("executable.description", description);
		node.add("executable.version", version);
		node.add("executable.contributor", contributor);
	}

    void XMLGenerator::addGroupTag(boost::property_tree::ptree& node, const std::string& label) const
	{
		node.add("label", label);
		node.add("description", label);
	}

    boost::property_tree::ptree XMLGenerator::createParameterTagWithDefaultValue(const std::string& longflag,
        char flag, const std::string& description, const std::string& defaultValue) const
	{
        boost::property_tree::ptree subnode;
        subnode.add("name", longflag);
        subnode.add("flag", flag);
        subnode.add("longflag", longflag);
        subnode.add("description", description);
        subnode.add("label", longflag);
        if (!defaultValue.empty()){
            subnode.put("default", defaultValue);
		}
        return subnode;
	}

    void XMLGenerator::addIOTag(boost::property_tree::ptree& node, const std::string& channel, const std::vector<std::string>& desiredFileExtensions) const
	{
        std::string extensions;
        for (const auto& extension : desiredFileExtensions){
            extensions += "." + extension;
            extensions += ",";
        }
        extensions.pop_back();

        node.add("<xmlattr>.fileExtensions", extensions);
		node.add("channel", channel);
	}

    void XMLGenerator::addConstraints(boost::property_tree::ptree& node, double minimum, double maximum, double step, bool castToInt) const
    {
        boost::property_tree::ptree& constraints = node.add("constraints", "");
        if (castToInt){
            constraints.add("minimum", static_cast<int>(minimum));
            constraints.add("maximum", static_cast<int>(maximum));
            constraints.add("step", static_cast<int>(step));
        }
        else {
            constraints.add("minimum", minimum);
            constraints.add("maximum", maximum);
            constraints.add("step", step);
        }
    }

    void XMLGenerator::addEnums(boost::property_tree::ptree& node, std::vector<std::string> data) const
    {
        for (const auto& element : data){
            node.add("element", element);
        }
    }

    void XMLGenerator::setBaseParameters(const std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string> >& parameters)
    {
        m_baseParameters = parameters;
    }

    void XMLGenerator::setAdditionalParameters(const std::unordered_map<std::string, std::tuple<paramType, double, double, double, std::vector<std::string> > >& parameters)
    {
        m_additionalParameters = parameters;
    }

}
