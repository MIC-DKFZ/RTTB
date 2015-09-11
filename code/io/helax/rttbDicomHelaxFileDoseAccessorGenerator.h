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
#ifndef __DICOM_HELAX_FILE_DOSE_ACCESSOR_GENERATOR_H
#define __DICOM_HELAX_FILE_DOSE_ACCESSOR_GENERATOR_H

#include <string>
#include <vector>
#include <exception>

#include "osconfig.h"    /* make sure OS specific configuration is included first */
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
				typedef boost::shared_ptr<DRTDoseIOD> DRTDoseIODPtr;


			protected:

			private:
				FileNameType _doseDirName;

				DicomHelaxFileDoseAccessorGenerator();

			public:
				~DicomHelaxFileDoseAccessorGenerator();
				/*! @brief Constructor. Initialisation with a directory name

				*/
				DicomHelaxFileDoseAccessorGenerator(FileNameType aDICOMRTDoseDirName);

				/*! @brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
				@exception InvalidParameterException Thrown if aDICOMRTDoseDirName is not found
				@exception InvalidDoseException Thrown if any loaded dose is invalid: one of column/row/numberOfFrames/doseGridScaling/pixelSpacing=0
				@exception DcmrtException Throw if dcmrt error
				*/
				DoseAccessorPointer generateDoseAccessor() ;


			};
		}
	}
}

#endif
