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


#ifndef __DOSEMAP_APPLICATION_DATA_H
#define __DOSEMAP_APPLICATION_DATA_H

#include "mapRegistration.h"

#include "rttbDoseAccessorInterface.h"


namespace rttb
{
	namespace apps
	{
		namespace doseMap
		{
            class DoseMapCmdLineParser;
			class ApplicationData
			{
			public:
				typedef map::core::Registration<3, 3> RegistrationType;

				/**Vector of arguments used to specify the loading style (always the first argument)
				* and, if needed, additional arguments for the specified loading style (e.g. location of the
				* Virtuos plan file for the Virtuos IO style).
				*/
				typedef std::vector<std::string> LoadingStyleArgType;
				/** Loaded Dose.*/
				core::DoseAccessorInterface::DoseAccessorPointer _inputDose;
				std::string  _inputDoseFileName;
				LoadingStyleArgType  _inputDoseLoadStyle;
				core::DoseAccessorInterface::DoseAccessorPointer _refDose;
				std::string  _refDoseFileName;
				LoadingStyleArgType  _refDoseLoadStyle;
				RegistrationType::Pointer _spReg;
				std::string  _regFileName;

				std::string  _outputFileName;

				std::string  _interpolatorName;

				void reset();

				ApplicationData();
			};


			/*! @brief Reads the necessary arguments from the DoseToolCmdLineParser and writes them in the respective variables of ApplicationData.
			*/
			void populateAppData(boost::shared_ptr<DoseMapCmdLineParser> argParser, ApplicationData& appData);

		}
	}
}
#endif
