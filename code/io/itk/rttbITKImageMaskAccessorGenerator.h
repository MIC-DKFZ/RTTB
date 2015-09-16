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
// @version $Revision: 793 $ (last changed revision)
// @date    $Date: 2014-10-10 10:24:45 +0200 (Fr, 10 Okt 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/
#ifndef __ITK_IMAGE_MASK_ACCESSOR_GENERATOR_H
#define __ITK_IMAGE_MASK_ACCESSOR_GENERATOR_H

#include "rttbBaseType.h"
#include "rttbITKImageMaskAccessor.h"
#include "rttbMaskAccessorGeneratorBase.h"

#include "itkImage.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			
			class ITKImageMaskAccessorGenerator: public core::MaskAccessorGeneratorBase
			{
			public:
				typedef MaskAccessorGeneratorBase::MaskAccessorPointer MaskAccessorPointer;

			private:
				/** @brief The Mask as itkImage */
				ITKImageMaskAccessor::ITKMaskImageType::ConstPointer _maskPtr;

				ITKImageMaskAccessorGenerator();

			public:
				virtual ~ITKImageMaskAccessorGenerator() {};

				/*!
				@pre aMaskImage must point to a valid instance.
				@exception InvalidDoseException Thrown if aMaskImage is invalid.
				*/
				ITKImageMaskAccessorGenerator(const ITKImageMaskAccessor::ITKMaskImageType* aMaskImage);

				/*! @brief Generate MaskAccessor
				@return Return shared pointer of MaskAccessor.
				*/
				MaskAccessorPointer generateMaskAccessor() ;

			};
		}
	}
}

#endif
