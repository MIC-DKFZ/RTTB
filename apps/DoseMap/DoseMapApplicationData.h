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
			/*! @class ApplicationData
				@brief Class for storing all relevant variables needed in DoseMap
			*/
			class ApplicationData
			{
			public:
				typedef map::core::Registration<3, 3> RegistrationType;

				/** Loaded Dose.*/
				core::DoseAccessorInterface::Pointer _inputDose;
				std::string  _inputDoseFileName;
				std::string  _inputDoseLoadStyle;
				core::DoseAccessorInterface::Pointer _refDose;
				std::string  _refDoseFileName;
				std::string  _refDoseLoadStyle;
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
