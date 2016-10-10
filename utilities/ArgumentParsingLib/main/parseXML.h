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

#ifndef __parseXML_h
#define __parseXML_h

#include <boost/program_options.hpp>

namespace cmdlineparsing
{
	/*!
	@brief	Parses the given XML file with the given parameter rule set.
	@param ruleSet	Option container (possibly containing multiple option groups) defining
					the valid arguments and their expected values.
	@throw boost::program_options::error	If the rule set contains an option that has no
											name or long name (i.e. only the one-letter
											shortcut).
	@throw boost::program_options::reading_file	If the XML file could not be read.
	*/
	boost::program_options::parsed_options parseXML(const std::string& fileName,
	        boost::program_options::options_description& ruleSet);
}

#endif