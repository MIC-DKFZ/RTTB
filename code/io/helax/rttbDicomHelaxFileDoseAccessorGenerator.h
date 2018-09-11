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

#ifndef __DICOM_HELAX_FILE_DOSE_ACCESSOR_GENERATOR_H
#define __DICOM_HELAX_FILE_DOSE_ACCESSOR_GENERATOR_H

#include <string>
#include <vector>
#include <exception>

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */
#include "drtdose.h"

#include "rttbDoseAccessorGeneratorBase.h"
#include "rttbBaseType.h"

namespace rttb
{
	namespace io
	{
		namespace helax
		{

			/*! @class DicomHelaxFileDoseAccessorGenerator
			@brief Load dose data from dicom helax files and generate DoseAccessor
			*/
			class DicomHelaxFileDoseAccessorGenerator: public core::DoseAccessorGeneratorBase
			{
			public:
				using DRTDoseIODPtr = boost::shared_ptr<DRTDoseIOD>;


			protected:

			private:
				FileNameType _doseDirName;

				DicomHelaxFileDoseAccessorGenerator() = delete;

			public:
				~DicomHelaxFileDoseAccessorGenerator() override;
				/*! @brief Constructor. Initialisation with a directory name

				*/
        explicit DicomHelaxFileDoseAccessorGenerator(FileNameType aDICOMRTDoseDirName);

				/*! @brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
				@exception InvalidParameterException Thrown if aDICOMRTDoseDirName is not found
				@exception InvalidDoseException Thrown if any loaded dose is invalid: one of column/row/numberOfFrames/doseGridScaling/pixelSpacing=0
				@exception DcmrtException Throw if dcmrt error
				*/
				DoseAccessorPointer generateDoseAccessor() override ;


			};
		}
	}
}

#endif
