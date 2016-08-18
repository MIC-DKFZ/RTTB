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

#include "rttbModelXMLWriter.h"

/*boost includes*/
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "rttbInvalidParameterException.h"



namespace rttb
{
	namespace io
	{
		namespace models
		{
			ModelXMLWriter::ModelXMLWriter(std::string& const filename, rttb::models::BioModel& const model) : _filename(filename), _model(model){
			}

			void ModelXMLWriter::setFilename(FileNameString filename){
				_filename = filename;
			}

			FileNameString ModelXMLWriter::getFilename() const{
				return _filename;
			}

			void ModelXMLWriter::setModel(rttb::models::BioModel& model){
				_model = model;
			}

			rttb::models::BioModel& ModelXMLWriter::getModel() const {
				return _model;
			}

			void ModelXMLWriter::writeModel(){

				boost::property_tree::ptree pt;

				std::string xmlattrNameTag = "<xmlattr>.name";
				std::string modelTag = "BioModel";
				std::string propertyTag = "property";
				std::string configTag = "config";
				std::string resultsTag = "results";
				std::string valueTag = "value";

				boost::property_tree::ptree propertynode;
				boost::property_tree::ptree confignode;

				confignode.put("BioModelType", _model.getModelType());
				confignode.put("StructureID", _model.getDVH()->getStructureID());
				confignode.put("DoseID", _model.getDVH()->getDoseID());
				pt.add_child(modelTag + "." + configTag, confignode);

				propertynode.put("", _model.getValue());
				propertynode.put(xmlattrNameTag, valueTag);
				pt.add_child(modelTag + "."+ resultsTag + "." + propertyTag, propertynode);
				
				std::map<std::string, double> parameterMap = _model.getParameterMap();
			
				for (std::map<std::string, double>::const_iterator it = parameterMap.begin(); it != parameterMap.end(); it++){
					propertynode.put("", it->second);
					propertynode.put(xmlattrNameTag, it->first);
					pt.add_child(modelTag + "." + resultsTag + "." + propertyTag, propertynode);
				}
				try
				{
					boost::property_tree::xml_parser::xml_writer_settings<std::string> settings =
					boost::property_tree::xml_writer_make_settings<std::string>('\t', 1);
					boost::property_tree::xml_parser::write_xml(_filename, pt, std::locale(), settings);
				}
				catch (boost::property_tree::xml_parser_error& const e)
				{
					std::cout << e.what();
					throw core::InvalidParameterException("Write xml failed: xml_parser_error!");
				}
				

			}

		}
	}
}
