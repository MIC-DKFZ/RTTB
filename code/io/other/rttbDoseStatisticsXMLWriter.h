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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/
#ifndef __DOSE_STATISTICS_XML_WRITER_H
#define __DOSE_STATISTICS_XML_WRITER_H


#include "rttbDoseStatistics.h"
#include "rttbBaseType.h"

/*boost includes*/
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

namespace rttb{
	namespace io{
		namespace other{

			typedef boost::shared_ptr<rttb::algorithms::DoseStatistics> DoseStatisticsPtr;
			typedef rttb::algorithms::DoseStatistics::ResultListPointer ResultListPointer;

			/*! @brief Write statistics to boost::property_tree::ptree.
			@param aReferenceDose A reference dose for the calculation of Vx
			@param aDoseStatistics DoseStatistics to write
			@pre aReferenceDose must >0
			@exception InvalidParameterException Thrown if aReferenceDose<=0 or xml_parse_error
			*/
			boost::property_tree::ptree writeDoseStatistics(DoseStatisticsPtr aDoseStatistics, DoseTypeGy aReferenceDose=100);

			/*! @brief Write statistics to String.
			@param aReferenceDose A reference dose for the calculation of Vx
			@param aDoseStatistics DoseStatistics to write
			@pre aReferenceDose must >0
			@exception InvalidParameterException Thrown if aReferenceDose<=0 or xml_parse_error
			*/
			XMLString writerDoseStatisticsToString(DoseStatisticsPtr aDoseStatistics, DoseTypeGy aReferenceDose=100);

			/*! @brief Write statistics to xml file, including 
			numberOfVoxels, 
			volume, 
			minimum, 
			maximum, 
			mean, 
			standard deviation, 
			variance, 
			D2,D5,D10,D90,D95,D98,
			V2,V5,V10,V90,V95,V98,
			MOH2,MOH5,MOH10,
			MOC2,MOC5,MOC10,
			MaxOH2,MaxOH5,MaxOH10,
			MinOC2,MinOC5,MinOC10.
			@param aReferenceDose A reference dose for the calculation of Vx
			@param aDoseStatistics DoseStatistics to write
			@param aFileName To write dose statistics
			@pre aReferenceDose must >0
			@exception InvalidParameterException Thrown if aReferenceDose<=0 or xml_parse_error
			*/
			void writeDoseStatistics(DoseStatisticsPtr aDoseStatistics, FileNameString aFileName, DoseTypeGy aReferenceDose=100);

			/*! @brief Write statistics to String to generate a table: "Volume mm3@Max@Min@Mean@Std.Dev.@Variance@V2@V5@V10@V90@V95@V98@D2@D5@D10@D90@D95@D98@MOH2@MOH5@MOH10@MOC2@MOC5@MOC10@MaxOH2@MaxOH5@MaxOH10@MinOC2@MinOC5@MinOC10"
			@param aReferenceDose A reference dose for the calculation of Vx
			@param aDoseStatistics DoseStatistics to write
			@pre aReferenceDose must >0
			@exception InvalidParameterException Thrown if aReferenceDose<=0 or xml_parse_error
			*/
			StatisticsString writerDoseStatisticsToTableString(DoseStatisticsPtr aDoseStatistics, DoseTypeGy aReferenceDose=100);
		}
	}
}
#endif
