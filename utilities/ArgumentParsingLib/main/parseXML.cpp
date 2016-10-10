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

#include <boost/property_tree/xml_parser.hpp>
#include "parseXML.h"

#include<iostream>

namespace po = boost::program_options;
namespace pt = boost::property_tree;

namespace cmdlineparsing
{
	po::parsed_options parseXML(const std::string& fileName, po::options_description& desc)
	{
		// Taken from po::parse_config_file()
		std::set<std::string> allowedOptions;
		const std::vector<boost::shared_ptr<po::option_description> >& options =
		    desc.options();

		for (unsigned i = 0; i < options.size(); ++i)
		{
			const po::option_description& d = *options[i];

			if (d.long_name().empty())
			{
				boost::throw_exception(po::error("abbreviated option names are not "
				                                 "permitted in options configuration "
				                                 "files"));
			}

			allowedOptions.insert(d.long_name());
		}

		// Load file and parse XML tree
		po::parsed_options result(&desc);
		pt::ptree tree;
		std::ifstream fileStream(fileName.c_str());

		if (!fileStream)
		{
			boost::throw_exception(po::reading_file(fileName.c_str()));
		}

			pt::read_xml(fileStream, tree);

		// Assign parameters
				for (pt::ptree::value_type& paramContainer : tree.get_child("Parameters"))
				{
					std::string key;
					std::vector<std::string> value;

					for (pt::ptree::value_type& param : paramContainer.second)
					{
						if (param.first == "Name")
						{
							key = param.second.data();
						}
						else if (param.first == "Value")
						{
							value.push_back(param.second.data());
						}
					}

					if (!key.empty() && allowedOptions.find(key) != allowedOptions.end())
					{
						result.options.push_back(po::option(key, value));
					}
				}

		return result;
	}
}