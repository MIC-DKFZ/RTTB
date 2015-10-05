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

			boost::property_tree::ptree writeDoseStatistics(DoseStatisticsPtr aDoseStatistics,
			        DoseTypeGy aReferenceDose)
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

				std::vector<double> DxVxOutputX = boost::assign::list_of(0.02)(0.05)(0.1)(0.9)(0.95)(0.98);

				try
				{
					for (int i = 0; i < DxVxOutputX.size(); i++)
					{
						ptree DxNode = createNodeWithNameAndXAttribute(aDoseStatistics->getDx(absoluteVolume * DxVxOutputX.at(i)), "Dx",
						               static_cast<int>(DxVxOutputX.at(i) * 100));
						pt.add_child(statisticsTag + "." + propertyTag, DxNode);
					}
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
					for (int i = 0; i < DxVxOutputX.size(); i++)
					{
						ptree VxNode = createNodeWithNameAndXAttribute(aDoseStatistics->getVx(aReferenceDose * DxVxOutputX.at(i)), "Vx",
						               static_cast<int>(DxVxOutputX.at(i) * 100));
						pt.add_child(statisticsTag + "." + propertyTag, VxNode);
					}
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				std::vector<double> OHOCOutputX = boost::assign::list_of(0.02)(0.05)(0.1);

				try
				{
					for (int i = 0; i < OHOCOutputX.size(); i++)
					{
						ptree mohxNode = createNodeWithNameAndXAttribute(aDoseStatistics->getMOHx(absoluteVolume * OHOCOutputX.at(i)), "MOHx",
						                 static_cast<int>(OHOCOutputX.at(i) * 100));
						pt.add_child(statisticsTag + "." + propertyTag, mohxNode);
					}
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					for (int i = 0; i < OHOCOutputX.size(); i++)
					{
						ptree mocxNode = createNodeWithNameAndXAttribute(aDoseStatistics->getMOCx(absoluteVolume * OHOCOutputX.at(i)), "MOCx",
						                 static_cast<int>(OHOCOutputX.at(i) * 100));
						pt.add_child(statisticsTag + "." + propertyTag, mocxNode);
					}
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					for (int i = 0; i < OHOCOutputX.size(); i++)
					{
						ptree maxOhxNode = createNodeWithNameAndXAttribute(aDoseStatistics->getMaxOHx(absoluteVolume * OHOCOutputX.at(i)),
						                   "MaxOHx",
						                   static_cast<int>(OHOCOutputX.at(i) * 100));
						pt.add_child(statisticsTag + "." + propertyTag, maxOhxNode);
					}
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					for (int i = 0; i < OHOCOutputX.size(); i++)
					{
						ptree minOCxNode = createNodeWithNameAndXAttribute(aDoseStatistics->getMinOCx(absoluteVolume * OHOCOutputX.at(i)),
						                   "MinOCx",
						                   static_cast<int>(OHOCOutputX.at(i) * 100));
						pt.add_child(statisticsTag + "." + propertyTag, minOCxNode);
					}
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
					boost::property_tree::xml_parser::write_xml(aFileName, pt, std::locale(),
					        boost::property_tree::xml_writer_make_settings<std::string>('\t', 1));
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
					boost::property_tree::xml_parser::write_xml(sstr, pt, boost::property_tree::xml_writer_make_settings<std::string>('\t',
					        1));
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

				/*todo: x should be defined based on the user's feedback*/
				if (aReferenceDose <= 0)
				{
					throw core::InvalidParameterException("aReferenceDose should be >0!");
				}

				std::vector<double> DxVxOutputX = boost::assign::list_of(0.02)(0.05)(0.1)(0.9)(0.95)(0.98);

				double absoluteVolume = aDoseStatistics->getVolume();

				try
				{
					for (int i = 0; i < DxVxOutputX.size(); i++)
					{
						sstr << aDoseStatistics->getDx(absoluteVolume * DxVxOutputX.at(i)) << columnSeparator;
					}
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					for (int i = 0; i < DxVxOutputX.size(); i++)
					{
						// *1000 because of conversion cm3 to mm3
						sstr << aDoseStatistics->getVx(aReferenceDose * DxVxOutputX.at(i)) * 1000 << columnSeparator;
					}
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				std::vector<double> OHOCOutputX = boost::assign::list_of(0.02)(0.05)(0.1);

				try
				{
					for (int i = 0; i < OHOCOutputX.size(); i++)
					{
						sstr << aDoseStatistics->getMOHx(absoluteVolume * OHOCOutputX.at(i)) << columnSeparator;
					}
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					for (int i = 0; i < OHOCOutputX.size(); i++)
					{
						sstr << aDoseStatistics->getMOCx(absoluteVolume * OHOCOutputX.at(i)) << columnSeparator;
					}
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					for (int i = 0; i < OHOCOutputX.size(); i++)
					{
						sstr << aDoseStatistics->getMaxOHx(absoluteVolume * OHOCOutputX.at(i)) << columnSeparator;
					}
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
				}

				try
				{
					for (int i = 0; i < OHOCOutputX.size(); i++)
					{
						sstr << aDoseStatistics->getMinOCx(absoluteVolume * OHOCOutputX.at(i)) << columnSeparator;
					}
				}
				catch (core::DataNotAvailableException)
				{
					//as data is not available (was not computed by doseStatistics), it cannot be written
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