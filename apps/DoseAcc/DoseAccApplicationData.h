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
// @version $Revision: 1107 $ (last changed revision)
// @date    $Date: 2015-09-17 12:47:41 +0200 (Do, 17 Sep 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#ifndef __DOSE_ACC_APPLICATION_DATA_H
#define __DOSE_ACC_APPLICATION_DATA_H

#include "mapRegistration.h"

#include "rttbDoseAccessorInterface.h"

namespace rttb
{
	namespace apps
	{
		namespace doseAcc
		{
            class DoseAccCmdLineParser;
            /*! @class ApplicationData
            @brief Class for storing all relevant variables needed in DoseAcc
            */
			class ApplicationData
			{
			public:
				typedef map::core::Registration<3, 3> RegistrationType;
				/**Vector of arguments used to specify the loading style (always the first argument)
				 * and, if needed, additional arguments for the specified loading style.
				 */
				typedef std::vector<std::string> LoadingStyleArgType;
				/** Loaded Dose.*/
				core::DoseAccessorInterface::DoseAccessorPointer _dose1;
				std::string  _dose1FileName;
				LoadingStyleArgType _dose1LoadStyle;
				core::DoseAccessorInterface::DoseAccessorPointer _dose2;
				std::string  _dose2FileName;
				LoadingStyleArgType _dose2LoadStyle;
				RegistrationType::Pointer _spReg;
				std::string  _regFileName;
				std::string _operator;

				std::string  _outputFileName;

				double _weightDose1;
				double _weightDose2;

				std::string  _interpolatorName;

				void Reset();

				ApplicationData();
			};

            /*! @brief Reads the necessary arguments from the DoseToolCmdLineParser and writes them in the respective variables of ApplicationData.
            */
            void populateAppData(boost::shared_ptr<DoseAccCmdLineParser> argParser, ApplicationData& appData);

		}
	}
}
#endif
