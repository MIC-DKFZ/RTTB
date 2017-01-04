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
// @version $Revision: 1250 $ (last changed revision)
// @date    $Date: 2016-02-18 15:25:55 +0100 (Do, 18 Feb 2016) $ (last change date)
// @author  $Author: zhangl $ (last changed by)
*/
#ifndef __DOSE_STATISTICS_XML_READER_H
#define __DOSE_STATISTICS_XML_READER_H

#include "rttbDoseStatistics.h"

/*boost includes*/

#include <boost/shared_ptr.hpp>

namespace rttb
{
	namespace io
	{
		namespace other
		{
			class DoseStatisticsXMLReader{
			public:
				typedef boost::shared_ptr<rttb::algorithms::DoseStatistics> DoseStatisticsPtr;

				DoseStatisticsXMLReader(const std::string& filename);
				~DoseStatisticsXMLReader();


				void setFilename(const std::string& filename);

				/*! @brief Generate a Model, createModel() will be called
				@return Return new shared pointer of a Model.
				@exception InvalidParameterException Thrown if _filename invalid
				*/
				DoseStatisticsPtr generateDoseStatistic();
			private:
				std::string _filename;

				bool _newFile;

				DoseStatisticsPtr _doseStatistic;

				/*! @brief Create new Model object using the info from model xml file
				@exception InvalidParameterException Thrown if _filename invalid
				*/
				void createDoseStatistic();
			};
		}
	}
}
#endif
