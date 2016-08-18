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
			class ModelXMLWriter{
			private:
				std::string _filename;
				rttb::models::BioModel&  _model;
			public:
				ModelXMLWriter(std::string& const filename, rttb::models::BioModel& const model);

				void setFilename(std::string filename);
				FileNameString getFilename() const;

				void setModel(rttb::models::BioModel&  model);
				rttb::models::BioModel& getModel() const;

				void writeModel();

			};

		}
	}
}
#endif
