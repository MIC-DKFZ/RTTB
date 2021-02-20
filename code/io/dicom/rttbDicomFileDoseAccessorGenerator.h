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

#ifndef __DICOM_FILE_DOSE_ACCESSOR_GENERATOR_H
#define __DICOM_FILE_DOSE_ACCESSOR_GENERATOR_H

#include <dcmtk/config/osconfig.h>    /* make sure OS specific configuration is included first */
#include <dcmtk/dcmrt/drtdose.h>

#include "rttbDoseAccessorGeneratorBase.h"
#include "rttbBaseType.h"

#include "RTTBDicomIOExports.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{
			/*! @class DicomFileDoseAccessorGenerator
			@brief Load dose data from dicom file and generate DicomDoseAccessor.
			*/
			class RTTBDicomIO_EXPORT DicomFileDoseAccessorGenerator: public core::DoseAccessorGeneratorBase
			{
			public:
				using DRTDoseIODPtr = boost::shared_ptr<DRTDoseIOD>;
				using DcmItemPtr = boost::shared_ptr<DcmItem>;

			private:
				FileNameType _dicomDoseFileName;

				DicomFileDoseAccessorGenerator() = delete;


			protected:


			public:
				~DicomFileDoseAccessorGenerator() override;

				/*! @brief Constructor. Initialization with a DICOM-RT dose file or a directory name
				@param aDICOMRTDoseFileName a DICOM-RT dose file name or a directory name
				@exception InvalidParameterException thrown if the file does not exist or the directory has no dicom dose file
				@exception DcmrtException thrown if load and read file failed
				*/
        explicit DicomFileDoseAccessorGenerator(FileNameType aDICOMRTDoseFileName);

				/*! @brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
				@exception InvalidDoseException Thrown if the loaded dose is invalid: one of column/row/numberOfFrames/doseGridScaling/pixelSpacing=0
				@exception DcmrtException Throw if dcmrt error
				*/
				DoseAccessorPointer generateDoseAccessor() override ;

			};
		}
	}
}

#endif
