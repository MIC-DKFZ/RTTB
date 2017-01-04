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

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "rttbDVHXMLFileReader.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace io
	{
		namespace other
		{

			DVHXMLFileReader::DVHXMLFileReader(FileNameString aFileName)
			{
				_fileName = aFileName;
				_resetFile = true;
			}

			DVHXMLFileReader::~DVHXMLFileReader() {}

			void DVHXMLFileReader::resetFileName(FileNameString aFileName)
			{
				_fileName = aFileName;
				_resetFile = true;
			}

			void DVHXMLFileReader::createDVH()
			{

				boost::property_tree::ptree pt;

				// Load the XML file into the property tree. If reading fails
				// (cannot open file, parse error), an exception is thrown.
				try
				{
					read_xml(_fileName, pt);
				}
				catch (boost::property_tree::xml_parser_error& /*e*/)
				{
					throw rttb::core::InvalidParameterException("DVH file name invalid: could not open the xml file!");
				}


				std::string dvhType;
				std::deque<DoseTypeGy> dataDifferential, dataCumulative;

				DoseTypeGy deltaD = 0;
				DoseVoxelVolumeType deltaV = 0;
				IDType strID;
				IDType doseID;

				dvhType = pt.get<std::string>("dvh.type");
				deltaD = pt.get<DoseTypeGy>("dvh.deltaD");
				deltaV = pt.get<DoseVoxelVolumeType>("dvh.deltaV");
				strID = pt.get<IDType>("dvh.structureID");
				doseID = pt.get<IDType>("dvh.doseID");

				if (dvhType != "DIFFERENTIAL" && dvhType != "CUMULATIVE")
				{
					throw core::InvalidParameterException("DVH Type invalid! Only: DIFFERENTIAL/CUMULATIVE!");
				}

		
				BOOST_FOREACH(boost::property_tree::ptree::value_type & v, pt.get_child("dvh.data"))
				{
					if (dvhType == "DIFFERENTIAL")
					{
						dataDifferential.push_back(boost::lexical_cast<DoseTypeGy>(v.second.data()));

					}
					else if (dvhType == "CUMULATIVE")
					{
						dataCumulative.push_back(boost::lexical_cast<DoseTypeGy>(v.second.data()));
					}

				}

				unsigned int numberOfBins = static_cast<unsigned int>(std::max(dataDifferential.size(),
				                            dataCumulative.size()));

				if (dvhType == "CUMULATIVE") //dataDifferential should be calculated
				{
					DoseCalcType differentialDVHi = 0;
					std::deque<DoseCalcType>::iterator it;

					for (it = dataCumulative.begin(); it != dataCumulative.end(); ++it)
					{
						if (dataDifferential.size() == numberOfBins - 1)
						{
							differentialDVHi = *it;
						}
						else
						{
							differentialDVHi = *it - *(it + 1);
						}

						dataDifferential.push_back(differentialDVHi);
					}
				}

				_dvh = boost::make_shared<core::DVH>(dataDifferential, deltaD, deltaV, strID, doseID);
				_resetFile = false;
			}

			DVHXMLFileReader::DVHPointer DVHXMLFileReader::generateDVH()
			{
				if (_resetFile)
				{
					this->createDVH();
				}

				return _dvh;
			}
		}//end namespace other
	}//end namespace io
}//end namespace rttb

