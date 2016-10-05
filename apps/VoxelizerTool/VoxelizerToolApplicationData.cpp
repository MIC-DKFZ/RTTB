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

#include "VoxelizerToolApplicationData.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizerTool
		{
			ApplicationData::
				ApplicationData()
			{
				this->reset();
			}

			void
				ApplicationData::
				reset()
			{
				_structFile = "";
				_referenceFile = "";
				_outputFilename = "";
				_regEx="";
				_multipleStructs = false;
				_legacyVoxelization = false;
				_booleanVoxelization = false;
				_addStructures = false;
				_allowSelfIntersections = false;
			}

			void populateAppData(boost::shared_ptr<VoxelizerCmdLineParser> argParser, ApplicationData& appData)
			{
				appData._structFile = argParser->get<std::string>(argParser->OPTION_STRUCT_FILE);
				appData._referenceFile = argParser->get<std::string>(argParser->OPTION_REFERENCE_FILE);
				appData._outputFilename = argParser->get<std::string>(argParser->OPTION_OUTPUT_FILE_NAME);
				appData._referenceFileLoadStyle = argParser->get<std::vector<std::string>>(argParser->OPTION_REFERENCE_FILE_LOAD_STYLE);
				appData._regEx = argParser->get<std::string>(argParser->OPTION_REGEX);
				
				if (argParser->isSet(argParser->OPTION_MULTIPLE_STRUCTS))
				{
					appData._multipleStructs = true;
					if (argParser->isSet(argParser->OPTION_ADDSTRUCTURES))
					{
						appData._addStructures = true;
					}
					else
					{
						appData._addStructures = false;
					}
				}

				if (argParser->isSet(argParser->OPTION_LEGACY_VOXELIZATION))
				{
					appData._legacyVoxelization = true;
				}
                else {
                    appData._legacyVoxelization = false;
                }

				if (argParser->isSet(argParser->OPTION_BOOLEAN_VOXELIZATION))
				{
					appData._booleanVoxelization = true;
				}

				if (argParser->isSet(argParser->OPTION_ADDSTRUCTURES))
				{
					appData._multipleStructs = false;
					appData._addStructures = true;
				}

				if (argParser->isSet(argParser->OPTION_ALLOW_SELF_INTERSECTIONS))
				{
					appData._allowSelfIntersections = true;
				}
			}
		}
	}
}
