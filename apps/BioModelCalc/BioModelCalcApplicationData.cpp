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
// @version $Revision: 1210 $ (last changed revision)
// @date    $Date: 2015-11-24 15:52:45 +0100 (Di, 24 Nov 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "BioModelCalcApplicationData.h"

namespace rttb
{
	namespace apps
	{
		namespace bioModelCalc
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
				_doseFileName = "";
				_outputFileName = "";
			}

			void populateAppData(boost::shared_ptr<BioModelCmdLineParser> argParser, ApplicationData& appData)
			{
				appData._doseFileName = argParser->get<std::string>(argParser->OPTION_DOSE_FILE);
				appData._doseLoadStyle = argParser->get<std::vector<std::string> >(argParser->OPTION_LOAD_STYLE);
				appData._doseScaling = argParser->get<double>(argParser->OPTION_DOSE_SCALING);
				appData._outputFileName = argParser->get<std::string>(argParser->OPTION_OUTPUT_FILE);
				appData._model = argParser->get<std::string>(argParser->OPTION_MODEL);
				appData._modelParameters = argParser->get<std::vector<double> >(argParser->OPTION_MODEL_PARAMETERS);
			}

		}
	}
}
