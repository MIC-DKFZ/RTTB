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
#ifndef __DVH_XML_FILE_READER_H
#define __DVH_XML_FILE_READER_H

#include "rttbBaseType.h"
#include "rttbDVH.h"
#include "rttbDVHGeneratorInterface.h"

namespace rttb{
	namespace io{
		namespace other{

			/*! @class DVHXMLFileReader
			@brief Reads DVH data from xml files.
			*/
			class DVHXMLFileReader: public core::DVHGeneratorInterface
			{
			private: 
				FileNameString _fileName;
				bool _resetFile;

				/*! @brief Create new DVH object using the info from dvh txt file
				@exception InvalidParameterException Thrown if _fileName invalid
				*/
				void createDVH();

			public: 
				/*! @brief Constructor. 
				*/
				DVHXMLFileReader(FileNameString aFileName);

				~DVHXMLFileReader();

				/*! @brief Change file name. 
				*/
				void resetFileName(FileNameString aFileName);

				/*! @brief Generate DVH, createDVH() will be called 
					@return Return new shared pointer of DVH. 
					@exception InvalidParameterException Thrown if _fileName invalid
				*/
				DVHPointer generateDVH();
			};
		}
	}
}

#endif
