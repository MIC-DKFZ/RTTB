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
#ifndef __DICOM_FILE_READER_HELPER_H
#define __DICOM_FILE_READER_HELPER_H

#include <vector>

#include "osconfig.h"    /* make sure OS specific configuration is included first */
#include "drtdose.h"

#include "rttbBaseType.h"
#include "boost/shared_ptr.hpp"


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

			typedef boost::shared_ptr<DRTDoseIOD> DRTDoseIODPtr;
			typedef boost::shared_ptr<DcmItem> DcmDataSetPtr;


			/*! Return the vector of all files with the same UID in the given directory, the UID is defined by the first file with the modality.
			@exception InvalidParameterException thrown if the file/directory does not exist or the modality is invalid
			@exception DcmrtException thrown if load/read file failed
			*/
			std::vector<FileNameType> getFileNamesWithSameUID(FileNameType aDirName, Modality aModality);

			/*! Return the vector of all files with the same UID in the directory of the given file
			@exception InvalidParameterException thrown if the file does not exist
			@exception DcmrtException thrown if load/read file failed
			*/
			std::vector<FileNameType> getFileNames(FileNameType aFileName);

			/*! Return if the given name is a file
			*/
			bool isFile(FileNameType aName);

			/*! Return if the given name is a directory
			*/
			bool isDirectory(FileNameType aName);

			/*! Return modality DcmTagKey(0x0008, 0x0060)
			@exception DcmrtException thrown if reading modality failed*/
			OFString getModality(DcmDataSetPtr aDcmDataSet);

			/*! Return uid DcmTagKey(0x0020, 0x000e)
			@exception DcmrtException thrown if reading uid failed*/
			OFString getUID(DcmDataSetPtr aDcmDataSet);


		};
	}
}


#endif
