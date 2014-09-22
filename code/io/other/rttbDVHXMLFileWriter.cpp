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

			DVHXMLFileWriter::DVHXMLFileWriter(FileNameString aFileName, DVHType aDVHType){
				this->setFileName(aFileName); 
				this->setDVHType(aDVHType);
			}

			void DVHXMLFileWriter::setDVHType(DVHType aDVHType){
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

			DVHType DVHXMLFileWriter::getDVHType() const{
				return _dvhType;
			}

			void DVHXMLFileWriter::writeDVH(DVHPointer aDvh)
			{
				if(!aDvh)
				{
					throw core::NullPointerException("aDvh must not be NULL! ");
				}


				using boost::property_tree::ptree;
				ptree pt;

				if(_dvhType.Type == DVHType::Differential)
				{
					pt.put("dvh.type", "DIFFERENTIAL");
				}
				else if( _dvhType.Type == DVHType::Cumulative)
				{
					pt.put("dvh.type", "CUMULATIVE");
				}
				else{
					throw core::InvalidParameterException("DVH Type not acceptable: Only: DIFFERENTIAL/CUMULATIVE!");
				}

				
				DataDifferentialType dataDifferential = aDvh->getDataDifferential();
				int numberOfBins = dataDifferential.size();
				pt.put("dvh.deltaD", aDvh->getDeltaD());
				pt.put("dvh.deltaV", aDvh->getDeltaV());
				pt.put("dvh.structureID", aDvh->getStructureID());
				pt.put("dvh.doseID", aDvh->getDoseID());

				
				if(_dvhType.Type == DVHType::Differential)
				{

					for(int i=0; i<numberOfBins; i++)
					{				
						pt.add("dvh.data.dosebin", i);
						pt.add("dvh.data.volume", dataDifferential[i]);
					}
				}
				else if(_dvhType.Type == DVHType::Cumulative)
				{

					DataDifferentialType dataCumulative = aDvh->calcCumulativeDVH();

					for(int i=0; i<numberOfBins; i++)
					{
						pt.add("dvh.data.dosebin", i);
						pt.add("dvh.data.volume", dataCumulative[i]);
					}
				}
				
				
				try
				{
					boost::property_tree::xml_parser::xml_writer_settings<char> settings('\t', 1);
					boost::property_tree::xml_parser::write_xml(_fileName, pt, std::locale(), settings);
				}
				catch (boost::property_tree::xml_parser_error& e)
				{
					throw core::InvalidParameterException("Write xml failed: xml_parser_error!");
				}

			}

		}
	}
}