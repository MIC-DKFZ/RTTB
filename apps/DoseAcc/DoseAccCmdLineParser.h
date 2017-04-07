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

#ifndef __DOSEACC_CMD_LINE_PARSER
#define __DOSEACC_CMD_LINE_PARSER

#include "CmdLineParserBase.h"
namespace rttb
{
	namespace apps
	{
		namespace doseAcc
		{
			/*! @class DoseAccCmdLineParser
			@brief Argument parsing is parametrized here based on ArgParserLib
			@see cmdlineparsing::CmdLineParserBase
			*/
			class DoseAccCmdLineParser : public cmdlineparsing::CmdLineParserBase
			{
			public:
                DoseAccCmdLineParser(int argc, const char** argv, const std::string& name,
				                      const std::string& version, bool virtuosSupport = false);
				DoseAccCmdLineParser(int argc, const char** argv, const std::string& name, const std::string& version,
					const std::string& description, const std::string& contributor, const std::string& category, bool virtuosSupport = false);
				void validateInput() const override;
				void printHelp() const override;

				// Option groups
				const std::string OPTION_GROUP_REQUIRED = "Required Arguments";
				const std::string OPTION_GROUP_OPTIONAL = "Optional Arguments";

				// Parameters
                const std::string OPTION_DOSE1_FILENAME = "dose1";
                const std::string OPTION_DOSE2_FILENAME = "dose2";
                const std::string OPTION_OUTPUT_FILENAME = "output";
				const std::string OPTION_INTERPOLATOR = "interpolator";
				const std::string OPTION_WEIGHT1 = "weight1";
                const std::string OPTION_WEIGHT2 = "weight2";
				const std::string OPTION_REGISTRATION_FILENAME = "registration";
                const std::string OPTION_LOAD_STYLE_DOSE1 = "loadStyle1";
                const std::string OPTION_LOAD_STYLE_DOSE2 = "loadStyle2";
				const std::string OPTION_OPERATOR = "operator";

				bool _virtuosSupport;
			};

		}
	}
}

#endif