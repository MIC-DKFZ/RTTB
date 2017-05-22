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
				/**Vector of arguments used to specify the loading style (always the first argument)
				 * and, if needed, additional arguments for the specified loading style.
				 */
				typedef std::vector<std::string> LoadingStyleArgType;

				core::DoseAccessorInterface::DoseAccessorPointer _dose;
                std::deque<core::AccessorInterface::AccessorPointer> _modelParameterMaps;
				std::string  _doseFileName;
				LoadingStyleArgType _doseLoadStyle;
                LoadingStyleArgType _parameterMapsLoadStyle;
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
