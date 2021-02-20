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

#ifndef __DICOM_FILE_READER_HELPER_H
#define __DICOM_FILE_READER_HELPER_H

#include <vector>

#include <dcmtk/config/osconfig.h>    /* make sure OS specific configuration is included first */
#include <dcmtk/dcmrt/drtdose.h>

#include "rttbBaseType.h"
#include "boost/shared_ptr.hpp"

#include "RTTBDicomIOExports.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{
			struct Modality
			{
				enum Type
				{
					RTDOSE = 1,
					RTSTRUCT = 2,
					RTPLAN = 3,
					UserDefined = 128
				} Type;
			};

			using DRTDoseIODPtr = boost::shared_ptr<DRTDoseIOD>;
			using DcmDataSetPtr = boost::shared_ptr<DcmItem>;


			/*! Return the vector of all files with the same UID in the given directory, the UID is defined by the first file with the modality.
			@exception InvalidParameterException thrown if the file/directory does not exist or the modality is invalid
			@exception DcmrtException thrown if load/read file failed
			*/
			RTTBDicomIO_EXPORT std::vector<FileNameType> getFileNamesWithSameUID(FileNameType aDirName, Modality aModality);

			/*! Return the vector of all files with the same UID in the directory of the given file
			@exception InvalidParameterException thrown if the file does not exist
			@exception DcmrtException thrown if load/read file failed
			*/
			RTTBDicomIO_EXPORT std::vector<FileNameType> getFileNames(FileNameType aFileName);

			/*! Return modality DcmTagKey(0x0008, 0x0060)
			@exception DcmrtException thrown if reading modality failed*/
			RTTBDicomIO_EXPORT OFString getModality(DcmDataSetPtr aDcmDataSet);

			/*! Return uid DcmTagKey(0x0020, 0x000e)
			@exception DcmrtException thrown if reading uid failed*/
			RTTBDicomIO_EXPORT OFString getUID(DcmDataSetPtr aDcmDataSet);


		};
	}
}


#endif
