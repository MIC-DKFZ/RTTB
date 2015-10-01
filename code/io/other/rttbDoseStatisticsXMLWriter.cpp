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

#include <ostream>

#include "rttbDoseStatisticsXMLWriter.h"
#include "rttbInvalidParameterException.h"
#include "rttbDataNotAvailableException.h"

namespace rttb
{
	namespace io
	{
		namespace other
		{
			static const std::string xmlattrTag = "<xmlattr>.x";
			static const std::string statisticsTag = "statistics";
			static const std::string columnSeparator = "@";

			boost::property_tree::ptree writeDoseStatistics(DoseStatisticsPtr aDoseStatistics,
			        DoseTypeGy aReferenceDose)
			{

				using boost::property_tree::ptree;
				ptree pt;

				pt.put(statisticsTag + ".numberOfVoxels", aDoseStatistics->getNumberOfVoxels());
				pt.put(statisticsTag + ".volume", aDoseStatistics->getVolume());
				pt.put(statisticsTag + ".minimum", aDoseStatistics->getMinimum());

				auto minimumPositions = aDoseStatistics->getMinimumPositions();
				std::vector<std::pair<DoseTypeGy, VoxelGridID> >::iterator pairItMin = minimumPositions->begin();
				int count = 0;

				for (; pairItMin != minimumPositions->end() && count < 100; ++pairItMin) //output max. 100 minimum
				{
					VoxelGridID voxelID = pairItMin->second;
					pt.add(statisticsTag + ".minimum.voxelGridID", voxelID);

					count++;
				}

				pt.put(statisticsTag + ".maximum", aDoseStatistics->getMaximum());

				auto maximumPositions = aDoseStatistics->getMaximumPositions();
				std::vector<std::pair<DoseTypeGy, VoxelGridID> >::iterator pairItMax = maximumPositions->begin();
				count = 0;

				for (; pairItMax != maximumPositions->end() && count < 100; ++pairItMax) //output max. 100 maximum
				{
					VoxelGridID voxelID = pairItMax->second;
					pt.add(statisticsTag + ".maximum.voxelGridID", voxelID);

					count++;
				}

				pt.put(statisticsTag + ".mean", aDoseStatistics->getMean());
				pt.put(statisticsTag + ".standardDeviation", aDoseStatistics->getStdDeviation());
				pt.put(statisticsTag + ".variance", aDoseStatistics->getVariance());

				double absoluteVolume = aDoseStatistics->getVolume();

				try
				{
					//Dx
					boost::property_tree::ptree dxNode1;
					boost::property_tree::ptree dxNode2;
					boost::property_tree::ptree dxNode3;
					boost::property_tree::ptree dxNode4;
					boost::property_tree::ptree dxNode5;
					boost::property_tree::ptree dxNode6;

					dxNode1.put("", aDoseStatistics->getDx(absoluteVolume * 0.02));
					dxNode2.put("", aDoseStatistics->getDx(absoluteVolume * 0.05));
					dxNode3.put("", aDoseStatistics->getDx(absoluteVolume * 0.10));
					dxNode4.put("", aDoseStatistics->getDx(absoluteVolume * 0.90));
					dxNode5.put("", aDoseStatistics->getDx(absoluteVolume * 0.95));
					dxNode6.put("", aDoseStatistics->getDx(absoluteVolume * 0.98));

					dxNode1.put(xmlattrTag, 2);
					dxNode2.put(xmlattrTag, 5);
					dxNode3.put(xmlattrTag, 10);
					dxNode4.put(xmlattrTag, 90);
					dxNode5.put(xmlattrTag, 95);
					dxNode6.put(xmlattrTag, 98);

					pt.add_child(statisticsTag + ".Dx", dxNode1);
					pt.add_child(statisticsTag + ".Dx", dxNode2);
					pt.add_child(statisticsTag + ".Dx", dxNode3);
					pt.add_child(statisticsTag + ".Dx", dxNode4);
					pt.add_child(statisticsTag + ".Dx", dxNode5);
					pt.add_child(statisticsTag + ".Dx", dxNode6);
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}


				if (aReferenceDose <= 0)
				{
					throw core::InvalidParameterException("aReferenceDose should be >0!");
				}

				try
				{
					//Vx
					boost::property_tree::ptree vxNode1;
					boost::property_tree::ptree vxNode2;
					boost::property_tree::ptree vxNode3;
					boost::property_tree::ptree vxNode4;
					boost::property_tree::ptree vxNode5;
					boost::property_tree::ptree vxNode6;

					vxNode1.put("", aDoseStatistics->getVx(aReferenceDose * 0.02));
					vxNode2.put("", aDoseStatistics->getVx(aReferenceDose * 0.05));
					vxNode3.put("", aDoseStatistics->getVx(aReferenceDose * 0.10));
					vxNode4.put("", aDoseStatistics->getVx(aReferenceDose * 0.90));
					vxNode5.put("", aDoseStatistics->getVx(aReferenceDose * 0.95));
					vxNode6.put("", aDoseStatistics->getVx(aReferenceDose * 0.98));

					vxNode1.put(xmlattrTag, 2);
					vxNode2.put(xmlattrTag, 5);
					vxNode3.put(xmlattrTag, 10);
					vxNode4.put(xmlattrTag, 90);
					vxNode5.put(xmlattrTag, 95);
					vxNode6.put(xmlattrTag, 98);

					pt.add_child(statisticsTag + ".Vx", vxNode1);
					pt.add_child(statisticsTag + ".Vx", vxNode2);
					pt.add_child(statisticsTag + ".Vx", vxNode3);
					pt.add_child(statisticsTag + ".Vx", vxNode4);
					pt.add_child(statisticsTag + ".Vx", vxNode5);
					pt.add_child(statisticsTag + ".Vx", vxNode6);
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					//MOHx
					boost::property_tree::ptree mohxNode1;
					boost::property_tree::ptree mohxNode2;
					boost::property_tree::ptree mohxNode3;

					mohxNode1.put("", aDoseStatistics->getMOHx(absoluteVolume * 0.02));
					mohxNode2.put("", aDoseStatistics->getMOHx(absoluteVolume * 0.05));
					mohxNode3.put("", aDoseStatistics->getMOHx(absoluteVolume * 0.10));

					mohxNode1.put(xmlattrTag, 2);
					mohxNode2.put(xmlattrTag, 5);
					mohxNode3.put(xmlattrTag, 10);

					pt.add_child(statisticsTag + ".MOHx", mohxNode1);
					pt.add_child(statisticsTag + ".MOHx", mohxNode2);
					pt.add_child(statisticsTag + ".MOHx", mohxNode3);
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					//MOCx
					boost::property_tree::ptree mocxNode1;
					boost::property_tree::ptree mocxNode2;
					boost::property_tree::ptree mocxNode3;

					mocxNode1.put("", aDoseStatistics->getMOCx(absoluteVolume * 0.02));
					mocxNode2.put("", aDoseStatistics->getMOCx(absoluteVolume * 0.05));
					mocxNode3.put("", aDoseStatistics->getMOCx(absoluteVolume * 0.10));

					mocxNode1.put(xmlattrTag, 2);
					mocxNode2.put(xmlattrTag, 5);
					mocxNode3.put(xmlattrTag, 10);

					pt.add_child(statisticsTag + ".MOCx", mocxNode1);
					pt.add_child(statisticsTag + ".MOCx", mocxNode2);
					pt.add_child(statisticsTag + ".MOCx", mocxNode3);
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					//MaxOHx
					boost::property_tree::ptree maxohxNode1;
					boost::property_tree::ptree maxohxNode2;
					boost::property_tree::ptree maxohxNode3;

					maxohxNode1.put("", aDoseStatistics->getMaxOHx(absoluteVolume * 0.02));
					maxohxNode2.put("", aDoseStatistics->getMaxOHx(absoluteVolume * 0.05));
					maxohxNode3.put("", aDoseStatistics->getMaxOHx(absoluteVolume * 0.10));

					maxohxNode1.put(xmlattrTag, 2);
					maxohxNode2.put(xmlattrTag, 5);
					maxohxNode3.put(xmlattrTag, 10);

					pt.add_child(statisticsTag + ".MaxOHx", maxohxNode1);
					pt.add_child(statisticsTag + ".MaxOHx", maxohxNode2);
					pt.add_child(statisticsTag + ".MaxOHx", maxohxNode3);
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					//MinOCx
					boost::property_tree::ptree minocxNode1;
					boost::property_tree::ptree minocxNode2;
					boost::property_tree::ptree minocxNode3;

					minocxNode1.put("", aDoseStatistics->getMinOCx(absoluteVolume * 0.02));
					minocxNode2.put("", aDoseStatistics->getMinOCx(absoluteVolume * 0.05));
					minocxNode3.put("", aDoseStatistics->getMinOCx(absoluteVolume * 0.10));

					minocxNode1.put(xmlattrTag, 2);
					minocxNode2.put(xmlattrTag, 5);
					minocxNode3.put(xmlattrTag, 10);

					pt.add_child(statisticsTag + ".MinOCx", minocxNode1);
					pt.add_child(statisticsTag + ".MinOCx", minocxNode2);
					pt.add_child(statisticsTag + ".MinOCx", minocxNode3);
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				return pt;

			}

			void writeDoseStatistics(DoseStatisticsPtr aDoseStatistics, FileNameString aFileName,
			                         DoseTypeGy aReferenceDose)
			{
				boost::property_tree::ptree pt = writeDoseStatistics(aDoseStatistics, aReferenceDose);

				try
				{
					boost::property_tree::xml_parser::write_xml(aFileName, pt);
				}
				catch (boost::property_tree::xml_parser_error& /*e*/)
				{
					throw core::InvalidParameterException("Write xml failed: xml_parser_error!");
				}
			}

			XMLString writerDoseStatisticsToString(DoseStatisticsPtr aDoseStatistics, DoseTypeGy aReferenceDose)
			{
				boost::property_tree::ptree pt = writeDoseStatistics(aDoseStatistics, aReferenceDose);
				std::stringstream sstr;

				try
				{
					boost::property_tree::xml_parser::write_xml(sstr, pt);
				}
				catch (boost::property_tree::xml_parser_error& /*e*/)
				{
					throw core::InvalidParameterException("Write xml to string failed: xml_parser_error!");
				}

				return sstr.str();
			}


			StatisticsString writerDoseStatisticsToTableString(DoseStatisticsPtr aDoseStatistics,
			        DoseTypeGy aReferenceDose)
			{

				std::stringstream sstr;

				sstr << aDoseStatistics->getVolume() * 1000 << columnSeparator; // cm3 to mm3
				sstr << aDoseStatistics->getMaximum() << columnSeparator;
				sstr << aDoseStatistics->getMinimum() << columnSeparator;

				sstr << aDoseStatistics->getMean() << columnSeparator;
				sstr << aDoseStatistics->getStdDeviation() << columnSeparator;
				sstr << aDoseStatistics->getVariance() << columnSeparator;

				if (aReferenceDose <= 0)
				{
					throw core::InvalidParameterException("aReferenceDose should be >0!");
				}

				//Vx

				try
				{
					sstr << aDoseStatistics->getVx(aReferenceDose * 0.02) * 1000 << columnSeparator; // cm3 to mm3
					sstr << aDoseStatistics->getVx(aReferenceDose * 0.05) * 1000 << columnSeparator; // cm3 to mm3
					sstr << aDoseStatistics->getVx(aReferenceDose * 0.10) * 1000 << columnSeparator; // cm3 to mm3
					sstr << aDoseStatistics->getVx(aReferenceDose * 0.90) * 1000 << columnSeparator; // cm3 to mm3
					sstr << aDoseStatistics->getVx(aReferenceDose * 0.95) * 1000 << columnSeparator; // cm3 to mm3
					sstr << aDoseStatistics->getVx(aReferenceDose * 0.98) * 1000 << columnSeparator; // cm3 to mm3
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				//Dx

				double absoluteVolume = aDoseStatistics->getVolume();

				try
				{
					sstr << aDoseStatistics->getDx(absoluteVolume * 0.02) << columnSeparator;
					sstr << aDoseStatistics->getDx(absoluteVolume * 0.05) << columnSeparator;
					sstr << aDoseStatistics->getDx(absoluteVolume * 0.10) << columnSeparator;
					sstr << aDoseStatistics->getDx(absoluteVolume * 0.90) << columnSeparator;
					sstr << aDoseStatistics->getDx(absoluteVolume * 0.95) << columnSeparator;
					sstr << aDoseStatistics->getDx(absoluteVolume * 0.98) << columnSeparator;
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					//MOHx
					sstr << aDoseStatistics->getMOHx(absoluteVolume * 0.02) << columnSeparator;
					sstr << aDoseStatistics->getMOHx(absoluteVolume * 0.05) << columnSeparator;
					sstr << aDoseStatistics->getMOHx(absoluteVolume * 0.10) << columnSeparator;
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					//MOCx
					sstr << aDoseStatistics->getMOCx(absoluteVolume * 0.02) << columnSeparator;
					sstr << aDoseStatistics->getMOCx(absoluteVolume * 0.05) << columnSeparator;
					sstr << aDoseStatistics->getMOCx(absoluteVolume * 0.10) << columnSeparator;
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					//MaxOHx
					sstr << aDoseStatistics->getMaxOHx(absoluteVolume * 0.02) << columnSeparator;
					sstr << aDoseStatistics->getMaxOHx(absoluteVolume * 0.05) << columnSeparator;
					sstr << aDoseStatistics->getMaxOHx(absoluteVolume * 0.10) << columnSeparator;
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					//MinOCx
					sstr << aDoseStatistics->getMinOCx(absoluteVolume * 0.02) << columnSeparator;
					sstr << aDoseStatistics->getMinOCx(absoluteVolume * 0.05) << columnSeparator;
					sstr << aDoseStatistics->getMinOCx(absoluteVolume * 0.10) << columnSeparator;
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}


				return sstr.str();

			}

		}
	}
}