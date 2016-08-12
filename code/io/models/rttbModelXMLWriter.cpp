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
#include "rttbInvalidParameterException.h"

#include <exception>

namespace rttb
{
	namespace io
	{
		namespace models
		{
			ModelXMLWriter::ModelXMLWriter(std::string fileName, rttb::models::BioModel*  model){
				this->setFileName(fileName);
				this->setModel(model);
			}

			void ModelXMLWriter::setFileName(FileNameString fileName){
				_fileName = fileName;
			}

			FileNameString ModelXMLWriter::getFileName() const{
				return _fileName;
			}

			void ModelXMLWriter::setModel(rttb::models::BioModel* model){
				_model = model;
			}

			rttb::models::BioModel* ModelXMLWriter::getModel() const {
				return _model;
			}

			void ModelXMLWriter::writeModel(){

				using boost::property_tree::ptree;
				ptree pt;

				static const std::string xmlattrNameTag = "<xmlattr>.name";
				static const std::string modelTag = "BioModel";
				static const std::string propertyTag = "property";
				static const std::string configTag = "config";
				static const std::string resultsTag = "results";

				ptree propertynode;
				ptree confignode;

				confignode.put("BioModelType", getModel()->getModelType());
				confignode.put("StructureID", getModel()->getDVH()->getStructureID());
				confignode.put("DoseID", getModel()->getDVH()->getDoseID());
				pt.add_child(modelTag + "." + configTag, confignode);

				propertynode.put("", getModel()->getValue());
				propertynode.put(xmlattrNameTag, "Value");
				pt.add_child(modelTag + "."+ resultsTag + "." + propertyTag, propertynode);
				
				std::map<std::string, double> parameterMap = _model->getParameterMap();
			
				for (std::map<std::string, double>::iterator it = parameterMap.begin(); it != parameterMap.end(); ++it){
					propertynode.put("", it->second);
					propertynode.put(xmlattrNameTag, it->first);
					pt.add_child(modelTag + "." + resultsTag + "." + propertyTag, propertynode);
				}
				try
				{
					boost::property_tree::xml_parser::xml_writer_settings<std::string> settings =
					boost::property_tree::xml_writer_make_settings<std::string>('\t', 1);
					boost::property_tree::xml_parser::write_xml(_fileName, pt, std::locale(), settings);
				}
				catch (boost::property_tree::xml_parser_error& e)
				{
					std::cout << e.what();
					throw core::InvalidParameterException("Write xml failed: xml_parser_error!");
				}
				

			}

		}
	}
}
