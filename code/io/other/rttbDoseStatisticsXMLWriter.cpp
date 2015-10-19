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

#include <boost/assign/list_of.hpp>

#include "rttbDoseStatisticsXMLWriter.h"
#include "rttbInvalidParameterException.h"
#include "rttbDataNotAvailableException.h"

namespace rttb
{
	namespace io
	{
		namespace other
		{
			static const std::string xmlattrXTag = "<xmlattr>.x";
			static const std::string xmlattrNameTag = "<xmlattr>.name";
			static const std::string statisticsTag = "statistics.results";
			static const std::string propertyTag = "property";
			static const std::string columnSeparator = "@";

			boost::property_tree::ptree writeDoseStatistics(DoseStatisticsPtr aDoseStatistics)
			{

				using boost::property_tree::ptree;
				ptree pt;

				ptree numberOfVoxelsNode = createNodeWithNameAttribute(aDoseStatistics->getNumberOfVoxels(), "numberOfVoxels");
				pt.add_child(statisticsTag + "." + propertyTag, numberOfVoxelsNode);

				ptree volumeNode = createNodeWithNameAttribute(aDoseStatistics->getVolume(), "volume");
				pt.add_child(statisticsTag + "." + propertyTag, volumeNode);

				ptree minimumNode = createNodeWithNameAttribute(aDoseStatistics->getMinimum(), "minimum");

				auto minimumPositions = aDoseStatistics->getMinimumPositions();
				std::vector<std::pair<DoseTypeGy, VoxelGridID> >::iterator pairItMin = minimumPositions->begin();
				int count = 0;

				for (; pairItMin != minimumPositions->end() && count < 100; ++pairItMin) //output max. 100 minimum
				{
					VoxelGridID voxelID = pairItMin->second;
					ptree voxelMinPositions;
					voxelMinPositions.add("voxelGridID", voxelID);
					minimumNode.add_child("voxel", voxelMinPositions);
					count++;
				}

				pt.add_child(statisticsTag + "." + propertyTag, minimumNode);

				ptree maximumNode = createNodeWithNameAttribute(aDoseStatistics->getMaximum(), "maximum");

				auto maximumPositions = aDoseStatistics->getMaximumPositions();
				std::vector<std::pair<DoseTypeGy, VoxelGridID> >::iterator pairItMax = maximumPositions->begin();
				count = 0;

				for (; pairItMax != maximumPositions->end() && count < 100; ++pairItMax) //output max. 100 maximum
				{
					VoxelGridID voxelID = pairItMax->second;
					ptree voxelMaxPositions;
					voxelMaxPositions.add("voxelGridID", voxelID);
					maximumNode.add_child("voxel", voxelMaxPositions);

					count++;
				}

				pt.add_child(statisticsTag + "." + propertyTag, maximumNode);

				ptree meanNode = createNodeWithNameAttribute(aDoseStatistics->getMean(), "mean");
				pt.add_child(statisticsTag + "." + propertyTag, meanNode);

				ptree sdNode = createNodeWithNameAttribute(aDoseStatistics->getStdDeviation(), "standardDeviation");
				pt.add_child(statisticsTag + "." + propertyTag, sdNode);

				ptree varianceNode = createNodeWithNameAttribute(aDoseStatistics->getVariance(), "variance");
				pt.add_child(statisticsTag + "." + propertyTag, varianceNode);

				double absoluteVolume = aDoseStatistics->getVolume();
				double referenceDose = aDoseStatistics->getReferenceDose();
				rttb::algorithms::DoseStatistics::DoseToVolumeFunctionType AllVx = aDoseStatistics->getAllVx();
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType AllDx = aDoseStatistics->getAllDx();
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType AllMOHx = aDoseStatistics->getAllMOHx();
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType AllMOCx = aDoseStatistics->getAllMOCx();
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType AllMaxOHx = aDoseStatistics->getAllMaxOHx();
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType AllMinOCx = aDoseStatistics->getAllMinOCx();


				rttb::algorithms::DoseStatistics::DoseToVolumeFunctionType::iterator vxIt;
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType::iterator it;

				for (it = AllDx.begin(); it != AllDx.end(); it++)
				{
					ptree DxNode = createNodeWithNameAndXAttribute((*it).second, "Dx",
						static_cast<int>((*it).first / absoluteVolume * 100));
					pt.add_child(statisticsTag + "." + propertyTag, DxNode);
				}

				for (vxIt = AllVx.begin(); vxIt != AllVx.end(); vxIt++)
				{
					ptree VxNode = createNodeWithNameAndXAttribute((*vxIt).second, "Vx",
						static_cast<int>((*vxIt).first / referenceDose * 100));
					pt.add_child(statisticsTag + "." + propertyTag, VxNode);
				}

				for (it = AllMOHx.begin(); it != AllMOHx.end(); it++)
				{
					ptree mohxNode = createNodeWithNameAndXAttribute((*it).second, "MOHx",
						static_cast<int>((*it).first / absoluteVolume * 100));
					pt.add_child(statisticsTag + "." + propertyTag, mohxNode);
				}

				for (it = AllMOCx.begin(); it != AllMOCx.end(); it++)
				{
					ptree mocxNode = createNodeWithNameAndXAttribute((*it).second, "MOCx",
						static_cast<int>((*it).first / absoluteVolume * 100));
					pt.add_child(statisticsTag + "." + propertyTag, mocxNode);
				}

				for (it = AllMaxOHx.begin(); it != AllMaxOHx.end(); it++)
				{
					ptree maxOhxNode = createNodeWithNameAndXAttribute((*it).second, "MaxOHx",
						static_cast<int>((*it).first / absoluteVolume * 100));
					pt.add_child(statisticsTag + "." + propertyTag, maxOhxNode);
				}

				for (it = AllMinOCx.begin(); it != AllMinOCx.end(); it++)
				{
					ptree minOCxNode = createNodeWithNameAndXAttribute((*it).second, "MinOCx",
						static_cast<int>((*it).first / absoluteVolume * 100));
					pt.add_child(statisticsTag + "." + propertyTag, minOCxNode);
				}

				return pt;

			}

			void writeDoseStatistics(DoseStatisticsPtr aDoseStatistics, FileNameString aFileName)
			{
				boost::property_tree::ptree pt = writeDoseStatistics(aDoseStatistics);

				try
				{
					boost::property_tree::xml_parser::write_xml(aFileName, pt, std::locale(),
						boost::property_tree::xml_writer_make_settings<std::string>('\t', 1));
				}
				catch (boost::property_tree::xml_parser_error& /*e*/)
				{
					throw core::InvalidParameterException("Write xml failed: xml_parser_error!");
				}
			}

			XMLString writerDoseStatisticsToString(DoseStatisticsPtr aDoseStatistics)
			{
				boost::property_tree::ptree pt = writeDoseStatistics(aDoseStatistics);
				std::stringstream sstr;

				try
				{
					boost::property_tree::xml_parser::write_xml(sstr, pt, boost::property_tree::xml_writer_make_settings<std::string>('\t',
						1));
				}
				catch (boost::property_tree::xml_parser_error& /*e*/)
				{
					throw core::InvalidParameterException("Write xml to string failed: xml_parser_error!");
				}

				return sstr.str();
			}


			StatisticsString writerDoseStatisticsToTableString(DoseStatisticsPtr aDoseStatistics)
			{

				std::stringstream sstr;

				sstr << aDoseStatistics->getVolume() * 1000 << columnSeparator; // cm3 to mm3
				sstr << aDoseStatistics->getMaximum() << columnSeparator;
				sstr << aDoseStatistics->getMinimum() << columnSeparator;

				sstr << aDoseStatistics->getMean() << columnSeparator;
				sstr << aDoseStatistics->getStdDeviation() << columnSeparator;
				sstr << aDoseStatistics->getVariance() << columnSeparator;

				double absoluteVolume = aDoseStatistics->getVolume();
				rttb::algorithms::DoseStatistics::DoseToVolumeFunctionType AllVx = aDoseStatistics->getAllVx();
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType AllDx = aDoseStatistics->getAllDx();
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType AllMOHx = aDoseStatistics->getAllMOHx();
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType AllMOCx = aDoseStatistics->getAllMOCx();
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType AllMaxOHx = aDoseStatistics->getAllMaxOHx();
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType AllMinOCx = aDoseStatistics->getAllMinOCx();


				rttb::algorithms::DoseStatistics::DoseToVolumeFunctionType::iterator vxIt;
				rttb::algorithms::DoseStatistics::VolumeToDoseFunctionType::iterator it;

				for (it = AllDx.begin(); it != AllDx.end(); it++)
				{
					sstr << (*it).second << columnSeparator;
				}

				for (vxIt = AllVx.begin(); vxIt != AllVx.end(); vxIt++)
				{
					// *1000 because of conversion cm3 to mm3
					sstr << (*vxIt).second * 1000 << columnSeparator;
				}

				for (it = AllMOHx.begin(); it != AllMOHx.end(); it++)
				{
					sstr << (*it).second << columnSeparator;
				}

				for (it = AllMOCx.begin(); it != AllMOCx.end(); it++)
				{
					sstr << (*it).second << columnSeparator;
				}

				for (it = AllMaxOHx.begin(); it != AllMaxOHx.end(); it++)
				{
					sstr << (*it).second << columnSeparator;
				}

				for (it = AllMinOCx.begin(); it != AllMinOCx.end(); it++)
				{
					sstr << (*it).second << columnSeparator;
				}

				return sstr.str();

			}

			boost::property_tree::ptree createNodeWithNameAttribute(DoseTypeGy doseValue, const std::string& attributeName)
			{
				boost::property_tree::ptree node;
				node.put("", doseValue);
				node.put(xmlattrNameTag, attributeName);
				return node;
			}

			boost::property_tree::ptree createNodeWithNameAndXAttribute(DoseTypeGy doseValue, const std::string& attributeName,
				int xValue)
			{
				boost::property_tree::ptree node;
				node.put("", doseValue);
				node.put(xmlattrNameTag, attributeName);
				node.put(xmlattrXTag, xValue);
				return node;
			}

		}//end namespace other
	}//end namespace io
}//end namespace rttb