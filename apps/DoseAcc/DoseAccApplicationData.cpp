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

#include "DoseAccApplicationData.h"

#include "DoseAccCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace doseAcc
		{

			ApplicationData::
			ApplicationData()
			{
				this->Reset();
			}

			void
			ApplicationData::
			Reset()
			{
				_dose1FileName = "";
				_dose2FileName = "";
				_outputFileName = "";
				_regFileName = "";

				_interpolatorName = "linear";

				_weightDose1 = 1.0;
				_weightDose2 = 1.0;

				_operator = "+";
			}

            void populateAppData(boost::shared_ptr<DoseAccCmdLineParser> argParser, ApplicationData& appData)
            {
                appData._dose1FileName = argParser->get<std::string>(argParser->OPTION_DOSE1_FILENAME);
                appData._dose1LoadStyle = argParser->get<std::string>
                    (argParser->OPTION_LOAD_STYLE_DOSE1);

                appData._dose2FileName = argParser->get<std::string>(argParser->OPTION_DOSE2_FILENAME);
                appData._dose2LoadStyle = argParser->get<std::string>
                    (argParser->OPTION_LOAD_STYLE_DOSE2);

                appData._outputFileName = argParser->get<std::string>(argParser->OPTION_OUTPUT_FILENAME);
                appData._interpolatorName = argParser->get<std::string>(argParser->OPTION_INTERPOLATOR);
                appData._weightDose1 = argParser->get<double>(argParser->OPTION_WEIGHT1);
                appData._weightDose2 = argParser->get<double>(argParser->OPTION_WEIGHT2);
                appData._regFileName = argParser->get<std::string>(argParser->OPTION_REGISTRATION_FILENAME);
                appData._operator = argParser->get<std::string>(argParser->OPTION_OPERATOR);
            }

		}
	}
}
