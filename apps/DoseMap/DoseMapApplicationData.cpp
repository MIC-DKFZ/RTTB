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
// @version $Revision: 1145 $ (last changed revision)
// @date    $Date: 2015-10-12 17:06:10 +0200 (Mo, 12 Okt 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "DoseMapApplicationData.h"


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
				appData._inputDoseLoadStyle = argParser->get<std::vector<std::string> > (argParser->OPTION_INPUT_DOSE_LOAD_STYLE);
				appData._refDoseLoadStyle = argParser->get<std::vector<std::string> >(argParser->OPTION_REF_DOSE_LOAD_STYLE);

				if (!argParser->isSet(argParser->OPTION_REF_DOSE_FILE)){
					appData._refDoseFileName = argParser->get<std::string>(argParser->OPTION_INPUT_DOSE_FILE_NAME);
				}
				else{
					appData._refDoseFileName = argParser->get<std::string>(argParser->OPTION_REF_DOSE_FILE);
				}
			
			}
		}
	}
}