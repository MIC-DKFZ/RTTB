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

#include "rttbModelXMLWriter.h"
#include "rttbDVHXMLFileWriter.h"
#include "rttbInvalidParameterException.h"

#include <boost/property_tree/xml_parser.hpp>

namespace rttb
{
	namespace io
	{
		namespace models
		{
			ModelXMLWriter::ModelXMLWriter(const std::string&  filename,
        rttb::models::BioModel::Pointer model, bool printDVH) : _filename(filename),
				_model(model), _printDVH(printDVH)
			{
			}

			void ModelXMLWriter::setFilename(FileNameString filename)
			{
				_filename = filename;
			}

			FileNameString ModelXMLWriter::getFilename() const
			{
				return _filename;
			}

			void ModelXMLWriter::setModel(rttb::models::BioModel::Pointer model)
			{
				_model = model;
			}

      rttb::models::BioModel::Pointer  ModelXMLWriter::getModel() const
			{
				return _model;
			}

			void ModelXMLWriter::setPrintDVH(bool printDVH)
			{
				_printDVH = printDVH;
			}
			bool ModelXMLWriter::getPrintDVH() const
			{
				return _printDVH;
			}

			void ModelXMLWriter::writeModel()
			{
				std::setprecision(2);
				boost::property_tree::ptree pt;

				std::string xmlattrNameTag = "<xmlattr>.name";
				std::string modelTag = "BioModel";
				std::string propertyTag = "property";
				std::string configTag = "config";
				std::string resultsTag = "results";
				std::string valueTag = "value";
				auto dvh = _model->getDVH();

				boost::property_tree::ptree propertynode;
				boost::property_tree::ptree confignode;
				confignode.put("BioModelType", _model->getModelType());
				confignode.put("StructureID", dvh->getStructureID());
				confignode.put("DoseID", dvh->getDoseID());

				if (_printDVH)
				{
					FileNameString filename = "dvhfor" + _model->getModelType() + ".xml";
					DVHType typeDiff = { DVHType::Differential };
					io::other::DVHXMLFileWriter dvhWriter(filename, typeDiff);
					dvhWriter.writeDVH(dvh);
					confignode.put("DVHReference", filename);
				}


				pt.add_child(modelTag + "." + configTag, confignode);

				propertynode.put("", _model->getValue());
				propertynode.put(xmlattrNameTag, valueTag);
				pt.add_child(modelTag + "." + resultsTag + "." + propertyTag, propertynode);

				std::map<std::string, double> parameterMap = _model->getParameterMap();

				for (std::map<std::string, double>::const_iterator it = parameterMap.begin();
				     it != parameterMap.end(); ++it)
				{
					double value = it->second;

					if (value - static_cast<double>(std::round(value)) < errorConstant)
					{
						propertynode.put("", static_cast<int>(value));
					}
					else
					{
						propertynode.put("", static_cast<float>(value));
					}

					propertynode.put(xmlattrNameTag, it->first);
					pt.add_child(modelTag + "." + resultsTag + "." + propertyTag, propertynode);
				}

				try
				{
					boost::property_tree::xml_parser::xml_writer_settings<std::string> settings =
					    boost::property_tree::xml_writer_make_settings<std::string>('\t', 1);

					boost::property_tree::xml_parser::write_xml(_filename, pt, std::locale(), settings);
				}
				catch (const boost::property_tree::xml_parser_error&  e)
				{
					std::cout << e.what();
					throw core::InvalidParameterException("Write xml failed: xml_parser_error!");
				}

			}

		}
	}
}
