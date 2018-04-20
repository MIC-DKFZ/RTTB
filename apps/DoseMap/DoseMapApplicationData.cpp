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

#include "DoseMapApplicationData.h"

#include "DoseMapCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace doseMap
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
				_inputDoseFileName = "";
				_refDoseFileName = "";
				_outputFileName = "";
				_regFileName = "";
				_interpolatorName = "linear";
			}

			void populateAppData(boost::shared_ptr<rttb::apps::doseMap::DoseMapCmdLineParser> argParser, ApplicationData& appData)
			{
				appData.reset();

				appData._inputDoseFileName = argParser->get<std::string>(argParser->OPTION_INPUT_DOSE_FILE_NAME);
				appData._outputFileName = argParser->get<std::string>(argParser->OPTION_OUTPUT_FILE_NAME);
				appData._interpolatorName = argParser->get<std::string>(argParser->OPTION_INTERPOLATOR);
				appData._regFileName = argParser->get<std::string>(argParser->OPTION_REG_FILE_NAME);
				appData._inputDoseLoadStyle = argParser->get<std::string>(argParser->OPTION_INPUT_DOSE_LOAD_STYLE);
				

				if (!argParser->isSet(argParser->OPTION_REF_DOSE_FILE)){
					appData._refDoseFileName = argParser->get<std::string>(argParser->OPTION_INPUT_DOSE_FILE_NAME);
					appData._refDoseLoadStyle = argParser->get<std::string>(argParser->OPTION_INPUT_DOSE_LOAD_STYLE);
				} else {
					appData._refDoseFileName = argParser->get<std::string>(argParser->OPTION_REF_DOSE_FILE);
					appData._refDoseLoadStyle = argParser->get<std::string>(argParser->OPTION_REF_DOSE_LOAD_STYLE);
				}
			
			}
		}
	}
}