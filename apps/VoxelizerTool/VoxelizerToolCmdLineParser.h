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

#ifndef __VoxelizerCmdLineParser_h
#define __VoxelizerCmdLineParser_h

#include "CmdLineParserBase.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizerTool
		{
			/*! @class VoxelizerCmdLineParser
			@brief Argument parsing is parametrized here based on ArgParserLib
			@see cmdlineparsing::CmdLineParserBase
			*/
			class VoxelizerCmdLineParser : public cmdlineparsing::CmdLineParserBase
			{
			public:
                VoxelizerCmdLineParser(int argc, const char** argv, const std::string& name, const std::string& version,
                    const std::string& description, const std::string& contributor, const std::string& category);
				void validateInput() const override;
				void printHelp() const override;

				// Option groups
				const std::string OPTION_GROUP_REQUIRED = "Required Arguments";
				const std::string OPTION_GROUP_OPTIONAL = "Optional Arguments";

				// Parameters
				const std::string OPTION_STRUCT_FILE = "structFile";
				const std::string OPTION_REFERENCE_FILE = "referenceFile";
				const std::string OPTION_REFERENCE_FILE_LOAD_STYLE = "referenceFileLoadStyle";
				const std::string OPTION_OUTPUT_FILE_NAME = "output";
				const std::string OPTION_REGEX = "struct";
				const std::string OPTION_MULTIPLE_STRUCTS = "multipleStructs";
				const std::string OPTION_BOOLEAN_VOXELIZATION = "booleanVoxelization";
				const std::string OPTION_ADDSTRUCTURES = "addStructures";
				const std::string OPTION_NO_STRICT_VOXELIZATION = "noStrictVoxelization";

			};
		}
	}
}

#endif