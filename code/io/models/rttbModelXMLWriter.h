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
// @version $Revision: 1250 $ (last changed revision)
// @date    $Date: 2016-02-18 15:25:55 +0100 (Do, 18 Feb 2016) $ (last change date)
// @author  $Author: zhangl $ (last changed by)
*/
#ifndef __MODELS_XML_WRITER_H
#define __MODELS_XML_WRITER_H



#include "rttbBioModel.h"



namespace rttb
{
	namespace io
	{
		namespace models
		{
            /*! @class ModelXMLWriter
            @brief Writes a model (TCP, NTCP, ...) to an xml file
            */
			class ModelXMLWriter
			{
			private:
				std::string _filename;
				rttb::models::BioModel::Pointer  _model;
				bool _printDVH;

			public:
				ModelXMLWriter(const std::string& filename, rttb::models::BioModel::Pointer  model,
				               bool printDVH = true);

				void setFilename(std::string filename);
				std::string getFilename() const;

				void setModel(rttb::models::BioModel::Pointer  model);
        rttb::models::BioModel::Pointer  getModel() const;

				void setPrintDVH(bool printDVH);
				bool getPrintDVH() const;

				void writeModel();

			};

		}
	}
}
#endif
