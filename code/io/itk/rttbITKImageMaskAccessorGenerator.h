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

#ifndef __ITK_IMAGE_MASK_ACCESSOR_GENERATOR_H
#define __ITK_IMAGE_MASK_ACCESSOR_GENERATOR_H

#include "rttbITKImageMaskAccessor.h"
#include "rttbMaskAccessorGeneratorBase.h"

#include "RTTBITKIOExports.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
            /*! @class ITKImageMaskAccessorGenerator
            @brief Generate ITKImageMaskAccessor wrapping an itk image as object (not as file).
            */
            class RTTBITKIO_EXPORT ITKImageMaskAccessorGenerator : public core::MaskAccessorGeneratorBase
			{
			public:
				using MaskAccessorPointer = MaskAccessorGeneratorBase::MaskAccessorPointer;

			private:
				/** @brief The Mask as itkImage */
				ITKImageMaskAccessor::ITKMaskImageType::ConstPointer _maskPtr;

				ITKImageMaskAccessorGenerator() = delete;

			public:
				~ITKImageMaskAccessorGenerator() override = default;

				/*!
				@pre aMaskImage must point to a valid instance.
				@exception InvalidDoseException Thrown if aMaskImage is invalid.
				*/
        explicit ITKImageMaskAccessorGenerator(const ITKImageMaskAccessor::ITKMaskImageType* aMaskImage);

				/*! @brief Generate MaskAccessor
				@return Return shared pointer of MaskAccessor.
				*/
				MaskAccessorPointer generateMaskAccessor() override ;

			};
		}
	}
}

#endif
