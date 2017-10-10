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


#include "rttbDVHXMLFileWriter.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbException.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

namespace rttb
{
	namespace io
	{
		namespace other
		{

			DVHXMLFileWriter::DVHXMLFileWriter(FileNameString aFileName, DVHType aDVHType)
			{
				this->setFileName(aFileName);
				this->setDVHType(aDVHType);
			}

			void DVHXMLFileWriter::setDVHType(DVHType aDVHType)
			{
				_dvhType = aDVHType;
			}

			FileNameString DVHXMLFileWriter::getFileName() const
			{
				return _fileName;
			}

			void DVHXMLFileWriter::setFileName(FileNameString aFileName)
			{
				_fileName = aFileName;
			}

			DVHType DVHXMLFileWriter::getDVHType() const
			{
				return _dvhType;
			}

			void DVHXMLFileWriter::writeDVH(DVHPointer aDvh, bool normalized)
			{
				if (aDvh == nullptr)
				{
					throw core::NullPointerException("aDvh must not be nullptr! ");
				}

				using boost::property_tree::ptree;
				ptree pt;
				DataDifferentialType data;
				std::map <DoseTypeGy, PercentType> normalizedData;

				if (_dvhType.Type == DVHType::Differential)
				{
					pt.put("dvh.type", "DIFFERENTIAL");
					if (normalized) {
						normalizedData = aDvh->getNormalizedDVH({ DVHType::Cumulative });
					} 
					else 
					{
						data = aDvh->getDataDifferential();
					}					
				}
				else if (_dvhType.Type == DVHType::Cumulative)
				{
					pt.put("dvh.type", "CUMULATIVE");
					if (normalized) {
						normalizedData = aDvh->getNormalizedDVH();
					}
					else
					{
						data = aDvh->getDataCumulative();
					}
				}
				else
				{
					throw core::InvalidParameterException("DVH Type not acceptable: Only: DIFFERENTIAL/CUMULATIVE!");
				}

				pt.put("dvh.deltaD", aDvh->getDeltaD());
				pt.put("dvh.deltaV", aDvh->getDeltaV());
				pt.put("dvh.structureID", aDvh->getStructureID());
				pt.put("dvh.doseID", aDvh->getDoseID());
				pt.put("dvh.normalized", normalized);

				if (normalized) {
					for (auto elem : normalizedData) 
					{
						boost::property_tree::ptree node;
						node.put("", elem.second);
						node.put("<xmlattr>.doseGy", elem.first);

						pt.add_child("dvh.data.volumeInCcm", node);
					}
				}
				else
				{
					for (size_t i = 0; i < data.size(); i++)
					{
						boost::property_tree::ptree node;
						node.put("", data[i]);
						node.put("<xmlattr>.dosebin", i);

						pt.add_child("dvh.data.volume", node);
					}
				}
				
				try
				{
					boost::property_tree::xml_parser::xml_writer_settings<std::string> settings =
					    boost::property_tree::xml_writer_make_settings<std::string> ('\t', 1);
					boost::property_tree::xml_parser::write_xml(_fileName, pt, std::locale(), settings);
				}
				catch (boost::property_tree::xml_parser_error& /*e*/)
				{
					throw core::InvalidParameterException("Write xml failed: xml_parser_error!");
				}

			}

		}
	}
}
