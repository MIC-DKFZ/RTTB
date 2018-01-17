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
#ifndef __DICOM_HELAX_IOD_VEC_DOSE_ACCESSOR_GENERATOR_H
#define __DICOM_HELAX_IOD_VEC_DOSE_ACCESSOR_GENERATOR_H

#include <vector>

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */
#include "drtdose.h"

#include "rttbDoseAccessorGeneratorBase.h"


namespace rttb
{
	namespace io
	{
		namespace helax
		{

			/*! @class DicomHelaxIODVecDoseAccessorGenerator
			@brief Generate DoseAccessor with a vector of DRTDoseIOD.
			*/
			class DicomHelaxIODVecDoseAccessorGenerator: public core::DoseAccessorGeneratorBase
			{
			public:
				using DRTDoseIODPtr = boost::shared_ptr<DRTDoseIOD>;


			protected:
			private:
				std::vector<DRTDoseIODPtr> _dosePtrVector;

				DicomHelaxIODVecDoseAccessorGenerator();

			public:
				~DicomHelaxIODVecDoseAccessorGenerator() override;

				/*! @brief Constructor. Initialisation with a vector of DRTDoseIOD pointers

				*/
				DicomHelaxIODVecDoseAccessorGenerator(std::vector<DRTDoseIODPtr>& aDICOMRTDoseVector);
				/*! @brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
				@exception InvalidDoseException Thrown if any DRTDoseIOD pointer of _doseVector is invalid: one of column/row/numberOfFrames/doseGridScaling/pixelSpacing=0
				@exception DcmrtException Throw if dcmrt error
				*/
				DoseAccessorPointer generateDoseAccessor() override ;


			};
		}
	}
}

#endif
