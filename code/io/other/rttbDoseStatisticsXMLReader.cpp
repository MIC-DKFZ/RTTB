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
// @version $Revision: 1328 $ (last changed revision)
// @date    $Date: 2016-04-22 09:50:01 +0200 (Fr, 22 Apr 2016) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/make_shared.hpp>

#include "rttbDoseStatisticsXMLReader.h"

#include "rttbInvalidParameterException.h"

#include "rttbVolumeToDoseMeasure.h"

namespace rttb
{
    namespace io
    {
        namespace other
        {
            typedef boost::shared_ptr<rttb::algorithms::DoseStatistics> DoseStatisticsPtr;


            DoseStatisticsXMLReader::DoseStatisticsXMLReader(const std::string& filename) : _filename(filename),
                _newFile(true)
            {
            }

            DoseStatisticsXMLReader::~DoseStatisticsXMLReader()
            {
            }


            void DoseStatisticsXMLReader::setFilename(const std::string& filename)
            {
                _filename = filename;
                _newFile = true;
            }

            DoseStatisticsPtr DoseStatisticsXMLReader::generateDoseStatistic()
            {
                if (_newFile)
                {
                    this->createDoseStatistic();
                }

                return _doseStatistic;
            }

            void DoseStatisticsXMLReader::createDoseStatistic()
            {

                boost::property_tree::ptree pt;

                // Load the XML file into the property tree. If reading fails
                // (cannot open file, parse error), an exception is thrown.
                try
                {
                    read_xml(_filename, pt);
                }
                catch (boost::property_tree::xml_parser_error& /*e*/)
                {
                    throw rttb::core::InvalidParameterException("DoseStatistics file name invalid: could not open the xml file!");
                }

                // data to fill the parameter for the DoseStatistics
                std::string name;
                std::string datum;
                unsigned int x;
                std::pair<double, int> voxelid;
                std::vector < std::pair<double, int>> vec;

                //initialize all parameters for the DoseStatistics
                double minimum=-1;
                double maximum=-1;
                double numVoxels=-1;
                double volume=-1;
                double referenceDose = -1;
                double mean=-1;
                double stdDeviation=-1;
                boost::shared_ptr<std::vector<std::pair<double, int> > > minimumVoxelPositions = nullptr;
                boost::shared_ptr<std::vector<std::pair<double, int> > > maximumVoxelPositions = nullptr;
				rttb::algorithms::VolumeToDoseMeasure Dx = rttb::algorithms::VolumeToDoseMeasure(rttb::algorithms::VolumeToDoseMeasure::complexStatistics::Dx);
                std::map<DoseTypeGy, VolumeType> Vx;
                std::map<VolumeType, DoseTypeGy> MOHx;
                std::map<VolumeType, DoseTypeGy> MOCx;
                std::map<VolumeType, DoseTypeGy> MaxOHx;
                std::map<VolumeType, DoseTypeGy> MinOCx;

                BOOST_FOREACH(boost::property_tree::ptree::value_type & data, pt.get_child("statistics.results"))
                {
                    datum = data.second.data();

                    BOOST_FOREACH(boost::property_tree::ptree::value_type & middle, data.second)
                    {
                        BOOST_FOREACH(boost::property_tree::ptree::value_type & innernode, middle.second)
                        {
                            std::string mia = innernode.first;

                            if (innernode.first == "name")
                            {
                                name = innernode.second.data();
                            }
                            else if (innernode.first == "voxelGridID")
                            {
                                boost::replace_all(datum, "\r\n", "");
                                boost::replace_all(datum, "\n", "");
                                boost::trim(datum);

                                voxelid.first = boost::lexical_cast<double>(datum);
                                voxelid.second = boost::lexical_cast<unsigned int>(innernode.second.data());
                                vec.push_back(voxelid);
                            }
                            else if (innernode.first == "x")
                            {
                                x = boost::lexical_cast<unsigned int>(innernode.second.data());
                            }
                        }
                    }


                    // fill with the extracted data
                    if (name == "numberOfVoxels")
                    {
                        numVoxels = boost::lexical_cast<double>(datum);
                    }
                    else if (name == "volume")
                    {
                        volume = boost::lexical_cast<double>(datum);
						Dx.setVolume(volume);
                    }
                    else if (name == "referenceDose")
                    {
                        referenceDose = boost::lexical_cast<double>(datum);
                    }
                    else if (name == "mean")
                    {
                        mean = boost::lexical_cast<double>(datum);
                    }
                    else if (name == "standardDeviation")
                    {
                        stdDeviation = boost::lexical_cast<double>(datum);
                    }
                    else if (name == "minimum")
                    {
                        minimum = boost::lexical_cast<double>(datum);

                        if (!vec.empty())
                        {
                            minimumVoxelPositions = boost::make_shared<std::vector<std::pair<double, int>>>(vec);
                            vec.clear();
                        }
                    }
                    else if (name == "maximum")
                    {
                        maximum = boost::lexical_cast<double>(datum);

                        if (!vec.empty())
                        {
                            maximumVoxelPositions = boost::make_shared<std::vector<std::pair<double, int>>>(vec);
                            vec.clear();
                        }
                    }
                    else if (name == "Dx")
                    {
                        Dx.insertValue(std::pair<VolumeType, DoseTypeGy>(boost::lexical_cast<double>(x)*volume / 100, boost::lexical_cast<double>(datum)));
                    }
                    else if (name == "Vx")
                    {
                        Vx[boost::lexical_cast<double>(x)*referenceDose / 100] = boost::lexical_cast<double>(datum);
                    }
                    else if (name == "MOHx")
                    {
                        MOHx[boost::lexical_cast<double>(x)*volume / 100] = boost::lexical_cast<double>(datum);
                    }
                    else if (name == "MOCx")
                    {
                        MOCx[boost::lexical_cast<double>(x)*volume / 100] = boost::lexical_cast<double>(datum);
                    }
                    else if (name == "MaxOHx")
                    {
                        MaxOHx[boost::lexical_cast<double>(x)*volume / 100] = boost::lexical_cast<double>(datum);
                    }
                    else if (name == "MinOCx")
                    {
                        MinOCx[boost::lexical_cast<double>(x)*volume / 100] = boost::lexical_cast<double>(datum);
                    }
                }

                // make DoseStatistcs
                _doseStatistic = boost::make_shared<rttb::algorithms::DoseStatistics>(
                    minimum, maximum, mean, stdDeviation, numVoxels, volume, minimumVoxelPositions,
                    maximumVoxelPositions , Dx, Vx, MOHx, MOCx, MaxOHx, MinOCx, referenceDose);
            }
        }//end namespace other
    }//end namespace io
}//end namespace rttb

