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

#ifndef __DICOM_IOD_DOSE_ACCESSOR_GENERATOR_H
#define __DICOM_IOD_DOSE_ACCESSOR_GENERATOR_H

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */
#include "drtdose.h"

#include "rttbDoseAccessorGeneratorBase.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{

			/*! @class DicomIODDoseAccessorGenerator
			@brief Generate DicomDoseAccessor with a DRTDoseIOD.
			*/
			class DicomIODDoseAccessorGenerator: public core::DoseAccessorGeneratorBase
			{
			public:
				using DRTDoseIODPtr = boost::shared_ptr<DRTDoseIOD>;
				using DcmItemPtr = boost::shared_ptr<DcmItem>;


			protected:

			private:
				DRTDoseIODPtr _doseIODPtr;

				DicomIODDoseAccessorGenerator() = delete;


			public:
				~DicomIODDoseAccessorGenerator() override;

				/*! @brief Constructor. Initialization with a boost shared pointer of DRTDoseIOD

				*/
				DicomIODDoseAccessorGenerator(DRTDoseIODPtr aDRTDoseIODP);

				/*! @brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
				@exception InvalidDoseException Thrown if aDRTDoseIODP is invalid: one of column/row/numberOfFrames/doseGridScaling/pixelSpacing=0
				@exception DcmrtException Throw if dcmrt error
				*/
				DoseAccessorPointer generateDoseAccessor() override ;

			};
		}
	}
}

#endif
