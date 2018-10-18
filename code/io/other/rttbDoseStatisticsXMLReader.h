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

#ifndef __DOSE_STATISTICS_XML_READER_H
#define __DOSE_STATISTICS_XML_READER_H

#include "rttbDoseStatistics.h"

namespace rttb
{
	namespace io
	{
		namespace other
		{
            /*! @class DoseStatisticsXMLReader
            @brief Reads a dose statistics XML into a DoseStatistics object
            */
			class DoseStatisticsXMLReader{
			public:
        explicit DoseStatisticsXMLReader(const std::string& filename);
				~DoseStatisticsXMLReader();


				void setFilename(const std::string& filename);

				/*! @brief Generate a Model, createModel() will be called
				@return Return new shared pointer of a Model.
				@exception InvalidParameterException Thrown if _filename invalid
				*/
        algorithms::DoseStatistics::Pointer generateDoseStatistic();
			private:
				std::string _filename;

				bool _newFile;

        algorithms::DoseStatistics::Pointer _doseStatistic;

				/*! @brief Create new Model object using the info from model xml file
				@exception InvalidParameterException Thrown if _filename invalid
				*/
				void createDoseStatistic();
			};
		}
	}
}
#endif
