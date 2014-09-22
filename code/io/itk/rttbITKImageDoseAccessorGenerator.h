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
#ifndef __ITK_IMAGE_DOSE_ACCESSOR_GENERATOR_H
#define __ITK_IMAGE_DOSE_ACCESSOR_GENERATOR_H

#include <string>
#include <exception>

#include "rttbDoseAccessorGeneratorBase.h"
#include "rttbBaseType.h"
#include "rttbITKImageDoseAccessor.h"

#include "itkImage.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			/*! @class ITKImageDoseAccessorGenerator
			@brief Generate ITKImageDoseAccessor wrapping an itk image as object (not as file).
			@note it implies that the dose information is stored in absolute Gy values.
			*/
			class ITKImageDoseAccessorGenerator: public core::DoseAccessorGeneratorBase
			{
			public:
				typedef DoseAccessorGeneratorBase::DoseAccessorPointer DoseAccessorPointer;

			private:
				/** @brief The dose as itkImage */
				ITKDoseImageType::ConstPointer _dosePtr;

				ITKImageDoseAccessorGenerator();

			public:
				virtual ~ITKImageDoseAccessorGenerator() {};

				/*!
				@pre aDoseImage must point to a valid instance.
				@exception InvalidDoseException Thrown if aDoseImage is invalid.
				*/
				ITKImageDoseAccessorGenerator(const ITKDoseImageType* aDoseImage);

				/*! @brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
				*/
				DoseAccessorPointer generateDoseAccessor() ;

			};
		}
	}
}

#endif
