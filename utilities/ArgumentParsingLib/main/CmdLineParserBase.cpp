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
//------------------------------------------------------------------------

#include "CmdLineParserBase.h"
#include "parseXML.h"
#include "boost/any.hpp"

namespace po = boost::program_options;

namespace cmdlineparsing
{

    CmdLineParserBase::CmdLineParserBase(const std::string& programName, const std::string& version, const std::string& programDescription,
        const std::string& programContributor, const std::string& programCategory) :
        m_programCategory(programCategory), m_programName(programName), m_programDescription(programDescription), m_programVersion(version), m_programContributor(programContributor), m_success(false)
	{
		addOption(OPTION_HELP, OPTION_GROUP_GENERIC, "Display this help", 'h');
        addOption(OPTION_VERSION, OPTION_GROUP_GENERIC, "Display the program version", 'v');
        addOption(OPTION_XML, OPTION_GROUP_GENERIC, "Outputs a xml description of all options", 'x');
		addOption<std::string>(OPTION_CONFIG_FILE, OPTION_GROUP_GENERIC, 
			"A configuration file containing the parameters to this "
            "program. Can be XML or simple text format.", 'c');
    }

	void CmdLineParserBase::addOption(const std::string& name,
		const std::string& optionGroupName,
        const std::string& description, char shorthand)
	{
		std::string fullName = name;

		if (shorthand != 0)
		{
			fullName += "," + std::string(1, shorthand);
		}

		getOrCreateOptionGroup(optionGroupName).add_options()(fullName.c_str(),
		        description.c_str());
        std::tuple<char, std::string, std::string, std::string> arguments = make_tuple(shorthand, description, optionGroupName, "" );
		
		findDuplicate(getOptionsAsStringMap(), name, shorthand);
		
        m_baseOptionsMap[name] = arguments;
	}

    void CmdLineParserBase::findDuplicate(const std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string> >& optionsMap,
		const std::string& name, char shorthand) const
	{
		for (const auto& option : optionsMap)
		{
			if (shorthand == 0)
			{
                if (option.first == name)
				{
					throw boost::program_options::duplicate_option_error("Duplicated shorthand or option name");
				}
			}
			else
			{
				if (option.first == name || std::get<0>(option.second) == shorthand)
				{
					throw boost::program_options::duplicate_option_error("Duplicated shorthand or option name");
				}
			}	
		}
	}

	void CmdLineParserBase::addPositionalOption(const std::string& name, int numArgs)
	{
		m_positionalOptions.add(name.c_str(), numArgs);
	}

	void CmdLineParserBase::parse(int argc, const char* argv[])
	{
		// Relocate the "Generic Arguments" option group so it's always displayed last in
		// the help
		m_optionGroupMap.relocate(m_optionGroupMap.get<orderTag>().end(),
		                          m_optionGroupMap.get<orderTag>().begin());

		// Add all option groups in the same order they have been defined
		for (const auto& optionGroup : m_optionGroupMap.get<orderTag>())
		{
			m_cmdLineOptions.add(optionGroup.second);

			// The arguments from the generic option group aren't parsable by the config
			// file.
			if (optionGroup.first != OPTION_GROUP_GENERIC)
			{
				m_configFileOptions.add(optionGroup.second);
			}
		}

		store(po::command_line_parser(argc, argv)
		      .options(m_cmdLineOptions)
		      .positional(m_positionalOptions)
		      .run(),
		      m_varMap);

		// Check the help or version flag before the auto-validation so other options are ignored.
		if (m_varMap.count(OPTION_HELP))
		{
			printHelp();
			return;
		}
		else if (m_varMap.count(OPTION_VERSION))
		{
			printVersion();
			return;
		}
		else if (m_varMap.count(OPTION_XML))
		{
			printXML();
			return;
		}

		if (m_varMap.count(OPTION_CONFIG_FILE))
		{
			std::string configFile = m_varMap[OPTION_CONFIG_FILE].as<std::string>();

			if (boost::algorithm::iends_with(boost::algorithm::to_lower_copy(configFile),
			                                 ".xml"))
			{
				store(parseXML(configFile, m_configFileOptions), m_varMap);
			}
			else
			{
				store(po::parse_config_file<char>(configFile.c_str(), m_configFileOptions),
				      m_varMap);
			}
		}

		// Auto-validation
		notify(m_varMap);

		// The manual validation still lies ahead but if we got here without exceptions,
		// it means arguments can be accessed via get().
		m_success = true;

		// Manual validation
		validateInput();
	}

	void CmdLineParserBase::printHelp() const
	{
		std::cout << "Usage: " << m_programName << " [options]";

		for (unsigned int i = 0; i < m_positionalOptions.max_total_count(); ++i)
		{
			std::string name =
			    boost::algorithm::to_upper_copy(m_positionalOptions.name_for_position(i));
			std::cout << " " << name;
		}

		std::cout << std::endl;
		std::cout << m_cmdLineOptions << std::endl;
	}


	void CmdLineParserBase::printVersion() const
	{
		std::cout << m_programName << " Version: " << m_programVersion << std::endl;
	}

	void CmdLineParserBase::printXML() const
	{
        cmdlineparsing::XMLGenerator generator(m_programCategory, m_programName, m_programDescription, m_programVersion, m_programContributor);
        //Generic options doesn't have a corresponding XML entry
        if (m_baseOptionsMap.size() - getOptions(OPTION_GROUP_GENERIC).size() == m_additionalOptionsMap.size()){
            generator.setBaseParameters(m_baseOptionsMap);
            generator.setAdditionalParameters(m_additionalOptionsMap);
            generator.write(std::cout);
        }
        else {
            std::cout << "no information given to create xml" << std::endl;
        }
	}

    std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string> > CmdLineParserBase::getOptionsAsStringMap() const
	{
        return m_baseOptionsMap;
	}


	bool CmdLineParserBase::isSet(const std::string& name) const
	{
		// Help gets special treatment because if the help flag is set, m_success is never
		// true since parse() returns prematurely.
		if (name == OPTION_HELP)
		{
			return (m_varMap.count(OPTION_HELP) > 0);
		}
		else if (name == OPTION_VERSION)
		{
			return (m_varMap.count(OPTION_VERSION) > 0);
		}
		else if (name == OPTION_XML)
		{
			return (m_varMap.count(OPTION_XML) > 0);
		}

		if (!m_success)
		{
			throw AccessException("Parsing wasn't complete. Cannot access arguments via "
			                      "isSet. Make sure there were no exceptions and the help "
			                      "parameter wasn't set.");
		}

		return (m_varMap.count(name.c_str()) > 0);
	}

	po::options_description& CmdLineParserBase::getOrCreateOptionGroup(
	    const std::string& optionGroupName)
	{
		auto iter = m_optionGroupMap.get<nameTag>().find(optionGroupName);

		if (iter == m_optionGroupMap.get<nameTag>().end())
		{
			po::options_description newOptionGroup(optionGroupName);
			// Can't simply return the newly created option group directly since it's in
			// local scope.
			// Instead, insert it in the option group map and return its value which is a
			// copy of the newly created option group.
			return m_optionGroupMap.push_back(OptionGroup(optionGroupName, newOptionGroup))
			       .first.get_node()->value().second;
		}

		return iter.get_node()->value().second;
	}

    void CmdLineParserBase::addInformationForXML(const std::string& name, XMLGenerator::paramType aType, double min /*= 0.0*/, double max /*= 100.0*/, double step /*= 1.0*/)
    {
        m_additionalOptionsMap.emplace(name, std::make_tuple(aType, min, max, step, std::vector<std::string>()));
    }

    void CmdLineParserBase::addInformationForXML(const std::string& name, XMLGenerator::paramType aType, const std::vector<std::string>& vectorData)
    {
        m_additionalOptionsMap.emplace(name, std::make_tuple(aType, 0, 0, 0, vectorData));
    }

    std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string> > CmdLineParserBase::getOptions(const std::string& name) const
    {
        std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string> > genericOptionsMap;
        for (const auto& keyValue : m_baseOptionsMap){
            if (std::get<2>(keyValue.second) == name){
                genericOptionsMap.insert(keyValue);
            }
        }
        return genericOptionsMap;
    }

}