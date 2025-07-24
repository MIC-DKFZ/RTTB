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

#ifndef __ITK_IMAGE_MASK_FILE_ACCESSOR_GENERATOR_H
#define __ITK_IMAGE_MASK_FILE_ACCESSOR_GENERATOR_H

#include "rttbMaskAccessorGeneratorBase.h"
#include "rttbBaseType.h"
#include "rttbITKImageMaskAccessor.h"

#include "RTTBITKIOExports.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{

			/*! @class ITKImageFileMaskAccessorGenerator
			@brief Load 3D Mask data using the itk loading methods and wraps the resulting itk image in a ITKImageMaskAccessor.
			* this is normally used if Mask distributions are stored in formats like meta image, nrrd...
			*/
			class RTTBITKIO_EXPORT ITKImageFileMaskAccessorGenerator: public core::MaskAccessorGeneratorBase
			{
			public:
				using MaskAccessorPointer = MaskAccessorGeneratorBase::MaskAccessorPointer;

			private:
				FileNameType _itkMaskFileName;
				/** @brief The mask as itkImage */
				ITKImageMaskAccessor::ITKMaskImageType::Pointer _itkDoubleImage;

				ITKImageFileMaskAccessorGenerator() = delete;

			public:
				~ITKImageFileMaskAccessorGenerator() override;

        explicit ITKImageFileMaskAccessorGenerator(const FileNameType& fileName);

				/*! @brief Generate MaskAccessor
				@return Return shared pointer of MaskAccessor.
				@exception InvalidDoseException Thrown if file could not be read
				@exception InvalidParameterException Thrown if file has imageDimension !=3 or image component type != SCALAR
				@details is always converted into a itkImage<DoseTypeGy,3> by using a CastImageFilter
				@sa doCasting, handleGenericImage
				*/
				MaskAccessorPointer generateMaskAccessor() override;


			};
		}//end namespace itk
	}//end namespace io
}//end namespace rttb


#endif
