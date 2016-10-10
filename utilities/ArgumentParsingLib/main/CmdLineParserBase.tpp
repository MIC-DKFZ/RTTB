#ifndef __CmdLineParserBase_tpp
#define __CmdLineParserBase_tpp

#include "CmdLineParserBase.h"

namespace cmdlineparsing
{
	template<typename T>
	void CmdLineParserBase::addOption(const std::string& name,
	                                  const std::string& optionGroupName,
                                      const std::string& description, char shorthand,
	                                  bool required, bool multitoken, bool composing)
	{
		addOption<T>(name, optionGroupName, description, shorthand,
		             generateTypedValue<T>(required, multitoken, composing));
		auto arguments = std::make_tuple(shorthand, description, optionGroupName,"" );

		findDuplicate(getOptionsAsStringMap(), name, shorthand);
		
        m_baseOptionsMap[name] = arguments;
	}

	template<typename T>
	void CmdLineParserBase::addOption(const std::string& name,
	                                  const std::string& optionGroupName, 
	                                  const std::string& description, char shorthand,
	                                  const boost::program_options::typed_value<T>* value)
	{
		std::string fullName = name;

		if (shorthand != 0)
		{
			fullName += "," + std::string(1, shorthand);
		}

		getOrCreateOptionGroup(optionGroupName).add_options()(fullName.c_str(), value,
		        description.c_str());
	}

	template<typename T>
	void CmdLineParserBase::addOptionWithDefaultValue(const std::string& name,
	        const std::string& optionGroupName, const std::string& description, 
            const T& defaultValue, const std::string& defaultValueRepresentation, char shorthand, bool required, bool multitoken,
	        bool composing)
	{
		addOption<T>(name, optionGroupName, description, shorthand,
		             generateTypedValue<T>(required, multitoken,
		                                   composing)->default_value(defaultValue, defaultValueRepresentation));

        auto arguments = std::make_tuple(shorthand, description, optionGroupName, defaultValueRepresentation);

		findDuplicate(getOptionsAsStringMap(), name, shorthand);

        m_baseOptionsMap[name] = arguments;
	}

	template<typename T>
	boost::program_options::typed_value<T>* CmdLineParserBase::generateTypedValue(
	    bool required, bool multitoken, bool composing) const
	{
		boost::program_options::typed_value<T>* value = boost::program_options::value<T>();

		if (required)
		{
			value = value->required();
		}

		if (multitoken)
		{
			value = value->multitoken();
		}

		if (composing)
		{
			value = value->composing();
		}

		return value;
	}

	template<typename T>
	T CmdLineParserBase::get(const std::string& name) const
	{
		if (!m_success)
		{
			throw AccessException("Parsing wasn't complete. Cannot access arguments via "
			                      "get. Make sure there were no exceptions and the help "
			                      "parameter wasn't set.");
		}

		if (!m_varMap.count(name))
		{
			throw AccessException("Option " + name + " not found in parsed arguments.");
		}

		return m_varMap[name].as<T>();
	}
}

#endif