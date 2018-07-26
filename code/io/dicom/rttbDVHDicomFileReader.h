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

#ifndef __DVH_DICOM_FILE_READER_H
#define __DVH_DICOM_FILE_READER_H

#include "rttbBaseType.h"
#include "rttbDVHGeneratorInterface.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{

			/*! @class DVHDicomFileReader
			@brief Read DVH data from a dicom file and create corresponding DVH object.
			*/
			class DVHDicomFileReader: public core::DVHGeneratorInterface
			{
			private:
				FileNameString _fileName;
				void createDVH();

			public:
				/*! @brief DVHDicomFileReader Constructor
				@param aFileName the dicom dvh file name
				*/
				DVHDicomFileReader(FileNameString aFileName);

				/*! @brief Set the dicom dvh file name (triggers data import)
				@param aFileName the dicom dvh file name
				*/
				void setFileName(FileNameString aFileName);
			};
		}
	}
}

#endif
