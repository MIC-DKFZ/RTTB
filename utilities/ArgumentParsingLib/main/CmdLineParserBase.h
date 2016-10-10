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

#ifndef __CmdLineParserBase_h
#define __CmdLineParserBase_h

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>

#include "XMLGenerator.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <tuple>

namespace cmdlineparsing
{
	/*!
	@brief	Thrown if accessing parsed parameters fails.
	*/
	class AccessException : public std::runtime_error
	{
	public:
		AccessException(const std::string& what) : std::runtime_error(what.c_str()) {}
	};

	/*!
	@brief	Should be thrown if CmdLineParserBase::validateInput() fails.
	*/
	class InvalidConstraintException : public std::runtime_error
	{
	public:
		InvalidConstraintException(const std::string& what) :
			std::runtime_error(what.c_str()) {}
	};

	/*!
	@brief		Base class for command line parsing including parsing from XML or INI-style
				config files.
	@details	Derive from this class, define your own arguments with
				CmdLineParserBase::addOption(),
				CmdLineParserBase::addOptionWithDefaultValue() and
				CmdLineParserBase::addPositionalOption() and call
				CmdLineParserBase::parse() after that to validate them.

				If the parsing completes without exceptions, you can access the parsed
				arguments with CmdLineParserBase::isSet() and CmdLineParserBase::get().
	@note		You can override CmdLineParserBase::validateInput() to further add custom
				validation (e.g. one int parameter mustn't be greater than the other one).
	@note		You can override CmdLineParserBase::printHelp() to control how the help is
				displayed.
	@note		The default parameters defined in this base class are "--help" (or "-h")
				and "--config-file <file>" (or "-c <file>"). If the given config file name
				ends in ".xml" it is parsed using cmdlineparsing::parseXML(), otherwise
				boost::parse_config_file() is used which expects INI-style parameter
				definitions.
	@note		There is a sample file for each format (.xml and .cfg) in the test data
				folder.
	*/


	class CmdLineParserBase
	{
	public:
		
		
		/*!
		@brief	Adds the four default parameters "--help" (or "-h"),
				"--config-file <file>" (or "-c <file>"),
                "--version" (or "-v"),
				"--xml" (or "-x") under the group
				"Generic Arguments".
		*/
        CmdLineParserBase(const std::string& programName, const std::string& version = "1.0", const std::string& programDescription="",
            const std::string& programContributor="DKFZ", const std::string& programCategory="Generic");
		virtual ~CmdLineParserBase() {}



		/*!
		@brief	Adds a new option to the option group with the given name. Adds a new
				option group if it doesn't exist yet.
		@param description	The description for this parameter that's shown in the help
							text.
		@param shorthand	The shorthand character for this option (e.g. 'h' for
							"--help").
							Make sure you don't set the same character twice.
		@param required		Whether this parameter is mandatory or optional.
		@param multitoken	Whether this parameter supports an unlimited number of values
							(e.g. "-n 1 2 3").
		@param composing	Whether this parameter can be composed of the values given in
							the command line and those given in the config file (usually
							the command line overwrites config file parameter values).
		*/
		template<typename T>
		void addOption(const std::string& name, const std::string& optionGroupName,
            const std::string& description, char shorthand = 0,
					   bool required = false, bool multitoken = false, 
		               bool composing = false);

		// Despite the similar signature this function needs its own name (instead of
		// addOption). Otherwise the default value might be implicitly cast to boolean and
		// match one of the other addOption signatures.
		/*!
		@brief	Add option with default value
		@param defaultValueRepresentation a string representation of the default value for the output in help
				(e.g. std::vector is not streamable, thus defaultValue can't be used in some cases)
		@see	CmdLineParserBase::addOption()
		*/
		template<typename T>
		void addOptionWithDefaultValue(const std::string& name,
		                               const std::string& optionGroupName,
		                               const std::string& description,
		                               const T& defaultValue,
									   const std::string& defaultValueRepresentation = "",
		                               char shorthand = 0,
									   bool required = false,
		                               bool multitoken = false, bool composing = false);

        void addInformationForXML(const std::string& name, XMLGenerator::paramType aType, double min = 0.0, double max = 100.0, double step = 1.0);
        void addInformationForXML(const std::string& name, XMLGenerator::paramType aType, const std::vector<std::string>& vectorData);

		/*!
		@brief	Non-generic version for arguments without values (e.g. "help").
		@see	CmdLineParserBase::addOption()
		*/
		void addOption(const std::string& name, const std::string& optionGroupName,
            const std::string& description, char shorthand = 0);

		/*!
		@brief	Adds a positional option with the number of expected values.
		@note	In order for a positional option to work you need to also add a regular
				option of the same name (using CmdLineParserBase::addOption() or
				CmdLineParserBase::addOptionWithDefaultValue()). This isn't done for you so
				you can decide which group it belongs to.
		@note	The "usage" line in the "help" text is generated from the positional
				arguments.
		@param numArgs	-1 means an unlimited number of values and only the very last
						positional option can have it.
		*/
		void addPositionalOption(const std::string& name, int numValues);

		/*!
		@brief	Starts parsing the given command line arguments.
		@details Beforehand all custom options must have been defined.
				Afterwards, the parsed parameters can be accessed with
				CmdLineParserBase::isSet() and CmdLineParserBase::get().
				Also calls CmdLineParserBase::validateInput().
		*/
		void parse(int argc, const char* argv[]);

		/*!
		@brief	Prints the default help to the standard output. The help text is generated
				from the option groups, regular options and positional options.
		@note	Override this function to print your own help text.
		*/
		virtual void printHelp() const;

		/*!
		@brief	Prints the default version information to the standard output.
		@note	Override this function to print your own version information.
		*/
		virtual void printVersion() const;

		/*!
		@brief	Prints the default xml information of parameters to the standard output.
		@note	Override this function to print your own xml information.
		*/
		virtual void printXML() const;

		/*!
		@brief	Gets called at the end of CmdLineParserBase::parse().
		@note	Override this function to implement your own input validation. E.g. if one
				integer parameter mustn't be greater than the other.
		*/
		virtual void validateInput() const = 0;

        std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string> > getOptionsAsStringMap() const;

		/*!
		@brief	Access the parsed parameters by name.
		@note	CmdLineParserBase::parse() must complete successfully before calling this.
		@throws cmdlineparsing::AccessException If used before parsing has completed. E.g.
												because CmdLineParserBase::parse() threw an
												exception or because the help parameter was
												set or because CmdLineParserBase::parse()
												wasn't called at all.
		@throws cmdlineparsing::AccessException If parameter doesn't exist in parameter
												map.
		*/
		template<typename T>
		T get(const std::string& name) const;

		/*!
		@brief	Check if the parsed parameter with the given name was given.
		@note	CmdLineParserBase::parse() must complete sucessfully before calling this.
		*/
		bool isSet(const std::string& name) const;

		const std::string OPTION_GROUP_GENERIC = "Generic Arguments";
		const std::string OPTION_HELP = "help";
		const std::string OPTION_VERSION = "version";
		const std::string OPTION_XML = "xml";
		const std::string OPTION_CONFIG_FILE = "config-file";

	protected:
		// These option groups are protected so you can access them when overriding
		// printHelp()
		boost::program_options::options_description m_cmdLineOptions, m_configFileOptions;
		boost::program_options::positional_options_description m_positionalOptions;

        //Stores basic options for each argument (key is longFlag) (shortFlag, description, optionGroup, default value)
        std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string> > m_baseOptionsMap;
        //Stores additional options for each argument (key is longFlag) (parameter type, minimum constraint, maximum constraint, step constraint, vector for string vector data)
        std::unordered_map<std::string, std::tuple<XMLGenerator::paramType, double, double, double, std::vector<std::string> > > m_additionalOptionsMap;

		std::string m_programCategory;
		std::string m_programName;
		std::string m_programDescription;
		std::string m_programVersion;
		std::string m_programContributor;

	private:
		CmdLineParserBase(const CmdLineParserBase&) = delete;
		void operator=(const CmdLineParserBase&) = delete;

		// Add option with typed value. Helper function to reduce duplicate code.
		template<typename T>
		void addOption(const std::string& name, const std::string& optionGroupName,
			const std::string& description, char shorthand,
		               const boost::program_options::typed_value<T>* value);

		// Returns the option group with the given name or creates a new one and returns it
		// if it already exists.
		boost::program_options::options_description& getOrCreateOptionGroup(
		    const std::string& optionGroupName);

		// Generate typed value. Helper function to reduce duplicate code.
		template<typename T>
		boost::program_options::typed_value<T>* generateTypedValue(bool required = false,
		        bool multitoken = false, bool composing = false) const;

        void CmdLineParserBase::findDuplicate(const std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string> >& optionsMap,
			const std::string& name, char shorthand) const;
        std::unordered_map<std::string, std::tuple<char, std::string, std::string, std::string> > getOptions(const std::string& name) const;
		boost::program_options::variables_map m_varMap;

		// Stores whether the parsing was successful and arguments can be read via get().
		bool m_success;
		
		// Multi-index container for the option groups.
		// This offers two different indices to the same collection:
		//  - nameTag: sorted by name (necessary for finding existing option groups)
		//  - orderTag: sorted by the order they were inserted in (necessary for displaying
		//				them in the help in the order they were defined)
		// Boost's options_description already has a container for option groups, however,
		// it doesn't offer enough access to add options to existing groups.
		struct nameTag {};
		struct orderTag {};
		typedef std::pair<std::string, boost::program_options::options_description>
		OptionGroup;
		boost::multi_index_container <
		OptionGroup,
		boost::multi_index::indexed_by <
		boost::multi_index::random_access<boost::multi_index::tag<orderTag> >,
		boost::multi_index::ordered_unique <
		boost::multi_index::tag<nameTag>,
		BOOST_MULTI_INDEX_MEMBER(OptionGroup, std::string, first)
		>
		>
		>
		m_optionGroupMap;
	};
}

#ifndef CMDLINEPARSING_MANUAL_INSTANTIATION
#include "CmdLineParserBase.tpp"
#endif

#endif