// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision: 1066 $ (last changed revision)
// @date    $Date: 2015-08-19 11:47:07 +0200 (Mi, 19 Aug 2015) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/

#ifndef __DOSETOOL_CMD_LINE_PARSER
#define __DOSETOOL_CMD_LINE_PARSER

#include "CmdLineParserBase.h"
namespace rttb
{
	namespace apps
	{
		namespace doseTool
		{
			/*! @class BioModelCmdLineParser
			@brief Argument parsing is parametrized here based on ArgParserLib
			@see cmdlineparsing::CmdLineParserBase
			*/
			class DoseToolCmdLineParser : public cmdlineparsing::CmdLineParserBase
			{
			public:
				DoseToolCmdLineParser(int argc, const char** argv, const std::string& name, const std::string& version);
				void validateInput() const;
				void printHelp() const;

				// Option groups
				const std::string OPTION_GROUP_REQUIRED = "Required Arguments";
				const std::string OPTION_GROUP_OPTIONAL = "Optional Arguments";

				// Parameters
				const std::string OPTION_DOSE_FILE = "doseFile";
				const std::string OPTION_STRUCT_FILE = "structFile";
				const std::string OPTION_STRUCT_NAME = "structName";
				const std::string OPTION_OUTPUT_FILE = "outputFile";
				const std::string OPTION_DOSE_LOAD_STYLE = "doseLoadStyle";
				const std::string OPTION_STRUCT_LOAD_STYLE = "structLoadStyle";
				const std::string OPTION_COMPLEX_STATISTICS = "complexStatistics";
				const std::string OPTION_PRESCRIBED_DOSE = "prescribedDose";
				const std::string OPTION_ALLOW_SELF_INTERSECTION_STRUCT = "allowSelfIntersection";
				const std::string OPTION_MULTIPLE_STRUCTS_MODE = "multipleStructsMode";
			};

		}
	}
}

#endif