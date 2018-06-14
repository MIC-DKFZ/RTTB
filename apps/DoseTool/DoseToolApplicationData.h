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

#ifndef __DOSETOOL_APPLICATION_DATA_H
#define __DOSETOOL_APPLICATION_DATA_H

#include <boost/shared_ptr.hpp>

#include "rttbDoseAccessorInterface.h"
#include "rttbStructureSetGeneratorInterface.h"

namespace rttb
{
	namespace apps
	{
		namespace doseTool
		{
            class DoseToolCmdLineParser;
			/*! @class ApplicationData
				@brief Class for storing all relevant variables needed in DoseTool
			*/
			class ApplicationData
			{
			public:
				core::DoseAccessorInterface::Pointer _dose;
				core::StructureSet::Pointer _struct;
				std::string _structNameRegex;
				std::vector<std::string> _structNames;
				std::string _doseFileName;
				std::string _structFileName;
				std::string _doseLoadStyle;
				std::string _structLoadStyle;
				bool _computeComplexDoseStatistics;
				DoseTypeGy _prescribedDose;
				std::string _doseStatisticOutputFileName;
				bool _allowSelfIntersection;
				bool _multipleStructsMode;
				bool _computeDVH;
				bool _computeDoseStatistics;
				std::string _dvhOutputFilename;

				/*! @brief Resets the variables. _prescribedDose is set to 1.0 because it produces no exception then (as it is not needed). Consistency checks are done in DoseToolCmdLineParser::validateInput()
				*/
				void reset();

				ApplicationData();
			};
			/*! @brief Reads the necessary arguments from the DoseToolCmdLineParser and writes them in the respective variables of ApplicationData.
			*/
			void populateAppData(boost::shared_ptr<DoseToolCmdLineParser> argParser, ApplicationData& appData);
		}
	}
}
#endif
