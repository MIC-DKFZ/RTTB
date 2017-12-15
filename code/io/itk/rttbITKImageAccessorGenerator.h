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
#ifndef __ITK_IMAGE_ACCESSOR_GENERATOR_H
#define __ITK_IMAGE_ACCESSOR_GENERATOR_H

#include "rttbDoseAccessorGeneratorBase.h"

#include "itkImage.h"

#include "RTTBITKIOExports.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			/*! @class ITKImageAccessorGenerator
			@brief Generate ITKImageAccessor wrapping an itk image as object (not as file).
			@note it implies that the information is stored in absolute values.
			*/
			class RTTBITKIO_EXPORT ITKImageAccessorGenerator : public core::DoseAccessorGeneratorBase
			{
			public:
				typedef ::itk::Image<GenericValueType, 3> ITKImageType;
				typedef DoseAccessorGeneratorBase::DoseAccessorPointer DoseAccessorPointer;

			private:
				/** @brief The dose as itkImage */
				ITKImageType::ConstPointer _dosePtr;

				ITKImageAccessorGenerator();

			public:
				~ITKImageAccessorGenerator() override = default;

				/*!
				@pre aDoseImage must point to a valid instance.
				@exception InvalidDoseException Thrown if aDoseImage is invalid.
				*/
				ITKImageAccessorGenerator(const ITKImageType* aDoseImage);

				/*! @brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
				*/
				DoseAccessorPointer generateDoseAccessor() override ;

			};
		}
	}
}

#endif
