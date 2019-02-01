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

#include "VoxelizerToolApplicationData.h"

#include "VoxelizerToolCmdLineParser.h"

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
				_binaryVoxelization = false;
				_addStructures = false;
				_noStrictVoxelization = false;
				_allStructs = false;
			}

			void populateAppData(boost::shared_ptr<VoxelizerCmdLineParser> argParser, ApplicationData& appData)
			{
				appData._structFile = argParser->get<std::string>(argParser->OPTION_STRUCT_FILE);
				appData._referenceFile = argParser->get<std::string>(argParser->OPTION_REFERENCE_FILE);
				appData._outputFilename = argParser->get<std::string>(argParser->OPTION_OUTPUT_FILE_NAME);
				appData._referenceFileLoadStyle = argParser->get<std::string>(argParser->OPTION_REFERENCE_FILE_LOAD_STYLE);
				//only valid option for reading structs is dicom until now.
				appData._structFileLoadStyle = "dicom";
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

				if (argParser->isSet(argParser->OPTION_BINARY_VOXELIZATION))
				{
					appData._binaryVoxelization = true;
				}

				if (argParser->isSet(argParser->OPTION_ADDSTRUCTURES))
				{
					appData._multipleStructs = false;
					appData._addStructures = true;
				}

				if (argParser->isSet(argParser->OPTION_NO_STRICT_VOXELIZATION))
				{
					appData._noStrictVoxelization = true;
				}

				if (argParser->isSet(argParser->OPTION_ALL_STRUCTS))
				{
					appData._allStructs = true;
				}
			}
		}
	}
}
