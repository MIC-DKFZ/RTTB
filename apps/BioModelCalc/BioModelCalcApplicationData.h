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

#ifndef __BIO_MODEL_CALC_APPLICATION_DATA_H
#define __BIO_MODEL_CALC_APPLICATION_DATA_H

#include <deque>

#include <boost/shared_ptr.hpp>

#include "rttbDoseAccessorInterface.h"

namespace rttb
{
	namespace apps
	{
		namespace bioModelCalc
		{
            class BioModelCmdLineParser;
			/*! @class ApplicationData
				@brief Class for storing all relevant variables needed in BioModelCalc
			*/
			class ApplicationData
			{
			public:
				core::DoseAccessorInterface::DoseAccessorPointer _dose;
                std::deque<core::AccessorInterface::AccessorPointer> _modelParameterMaps;
				std::string  _doseFileName;
				std::string _doseLoadStyle;
				std::string _parameterMapsLoadStyle;
				double _doseScaling;
                unsigned int _nFractions;

				std::string  _outputFileName;

				std::string _model;
				std::vector<double> _modelParameters;
                std::vector<std::string> _modelParameterMapsFilename;

				void reset();

				ApplicationData();
			};
			/*! @brief Reads the necessary arguments from the BioModelCmdLineParser and writes them in the respective variables of ApplicationData
			*/
			void populateAppData(boost::shared_ptr<BioModelCmdLineParser> argParser, ApplicationData& appData);
		}
	}
}
#endif
