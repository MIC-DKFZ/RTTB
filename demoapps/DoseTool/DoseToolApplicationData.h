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
// @version $Revision: 1070 $ (last changed revision)
// @date    $Date: 2015-08-19 14:50:28 +0200 (Mi, 19 Aug 2015) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/


#ifndef __DOSETOOL_APPLICATION_DATA_H
#define __DOSETOOL_APPLICATION_DATA_H

#include <boost/shared_ptr.hpp>

#include "rttbDoseAccessorInterface.h"
#include "rttbStructureSetGeneratorInterface.h"
#include "DoseToolCmdLineParser.h"

namespace rttb
{
	namespace apps
	{
		namespace doseTool
		{
			/*! @class ApplicationData
				@brief Class for storing all relevant variables needed in DoseTool
			*/
			class ApplicationData
			{
			public:
				/**Vector of arguments used to specify the loading style (always the first argument)
				 * and, if needed, additional arguments for the specified loading style (e.g. location of the
				 * Virtuos plan file for the Virtuos IO style).
				 */
				typedef std::vector<std::string> LoadingStyleArgType;
				core::DoseAccessorInterface::DoseAccessorPointer _dose;
				core::StructureSetGeneratorInterface::StructureSetPointer _struct;
				std::string _structNameRegex;
				std::string _structNameActual;
				unsigned int _structIndex;
				std::string  _doseFileName;
				std::string  _structFileName;
				LoadingStyleArgType _doseLoadStyle;
				LoadingStyleArgType _structLoadStyle;
				bool _computeComplexDoseStatistics;
				DoseTypeGy _prescribedDose;
				std::string  _outputFileName;
				bool _allowSelfIntersection;

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
