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
// @version $Revision: 1374 $ (last changed revision)
// @date    $Date: 2016-05-30 14:15:42 +0200 (Mo, 30 Mai 2016) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
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
			/*! @class DoseToolCmdLineParser
			@brief Argument parsing is parametrized here based on ArgParserLib
			@see cmdlineparsing::CmdLineParserBase
			*/
			class DoseToolCmdLineParser : public cmdlineparsing::CmdLineParserBase
			{
			public:
				DoseToolCmdLineParser(int argc, const char** argv, const std::string& name, const std::string& version,
					const std::string& description, const std::string& contributor, const std::string& category, bool virtuosSupport = false);
				void validateInput() const;
				void printHelp() const;

				// Option groups
				const std::string OPTION_GROUP_REQUIRED = "Required Arguments";
				const std::string OPTION_GROUP_OPTIONAL = "Optional Arguments";

				// Parameters
				const std::string OPTION_DOSE_FILE = "doseFile";
				const std::string OPTION_STRUCT_FILE = "structFile";
				const std::string OPTION_STRUCT_NAME = "structName";
				const std::string OPTION_DOSE_STATISTICS = "doseStatistics";
				const std::string OPTION_DVH = "DVH";
				const std::string OPTION_DOSE_LOAD_STYLE = "doseLoadStyle";
				const std::string OPTION_STRUCT_LOAD_STYLE = "structLoadStyle";
				const std::string OPTION_COMPLEX_STATISTICS = "complexStatistics";
				const std::string OPTION_PRESCRIBED_DOSE = "prescribedDose";
				const std::string OPTION_ALLOW_SELF_INTERSECTION_STRUCT = "allowSelfIntersection";
				const std::string OPTION_MULTIPLE_STRUCTS_MODE = "multipleStructsMode";

				bool _virtuosSupport;
			};

		}
	}
}

#endif