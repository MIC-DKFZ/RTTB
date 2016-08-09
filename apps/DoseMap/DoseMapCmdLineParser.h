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
// @version $Revision: 1221 $ (last changed revision)
// @date    $Date: 2015-12-01 13:43:31 +0100 (Di, 01 Dez 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#ifndef __DOSEMAP_CMD_LINE_PARSER
#define __DOSEMAP_CMD_LINE_PARSER

#include "CmdLineParserBase.h"
namespace rttb
{
	namespace apps
	{
		namespace doseMap
		{
			/*! @class BioModelCmdLineParser
			@brief Argument parsing is parametrized here based on ArgParserLib
			@see cmdlineparsing::CmdLineParserBase
			*/
			class DoseMapCmdLineParser : public cmdlineparsing::CmdLineParserBase 
			{
			public:
				DoseMapCmdLineParser(int argc, const char** argv, const std::string& name,
					const std::string& version, bool virtuosSupport = false);
				void validateInput() const;
				void printHelp() const;

				// Option groups
				const std::string OPTION_GROUP_REQUIRED = "Required Arguments";
				const std::string OPTION_GROUP_OPTIONAL = "Optional Arguments";

				// Parameters
				const std::string OPTION_INPUT_DOSE_FILE_NAME = "inputDoseFileName";
				const std::string OPTION_OUTPUT_FILE_NAME = "outputFileName";
				const std::string OPTION_INTERPOLATOR = "interpolator";
				const std::string OPTION_REG_FILE_NAME = "regFileName";
				const std::string OPTION_REF_DOSE_FILE = "refDoseFile";
				const std::string OPTION_REF_DOSE_LOAD_STYLE = "refDoseLoadStyle";
				const std::string OPTION_INPUT_DOSE_LOAD_STYLE = "inputDoseLoadStyle";


				bool _virtuosSupport;
			};

		}
	}
}

#endif