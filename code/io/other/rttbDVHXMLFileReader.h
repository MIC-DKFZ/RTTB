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

#ifndef __DVH_XML_FILE_READER_H
#define __DVH_XML_FILE_READER_H

#include "rttbBaseType.h"
#include "rttbDVHGeneratorInterface.h"

#include "RTTBOtherIOExports.h"

namespace rttb
{
	namespace io
	{
		namespace other
		{

			/*! @class DVHXMLFileReader
			@brief Reads DVH data from xml files.
			*/
			class RTTBOtherIO_EXPORT DVHXMLFileReader: public core::DVHGeneratorInterface
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
        explicit DVHXMLFileReader(FileNameString aFileName);

				~DVHXMLFileReader();

				/*! @brief Change file name.
				*/
				void resetFileName(FileNameString aFileName);

				/*! @brief Generate DVH, createDVH() will be called
					@return Return new shared pointer of DVH.
					@exception InvalidParameterException Thrown if _fileName invalid
				*/
				core::DVH::Pointer generateDVH() override;
			};
		}
	}
}

#endif
