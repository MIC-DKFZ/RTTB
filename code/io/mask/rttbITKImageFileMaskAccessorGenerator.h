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
#ifndef __ITK_IMAGE_MASK_FILE_ACCESSOR_GENERATOR_H
#define __ITK_IMAGE_MASK_FILE_ACCESSOR_GENERATOR_H

#include <string>
#include <exception>

#include "rttbMaskAccessorGeneratorBase.h"
#include "rttbBaseType.h"
#include "rttbITKImageMaskAccessor.h"
#include "../itk/rttbGenericImageReader.h"

#include "itkImage.h"
#include "itkCastImageFilter.h"


namespace rttb
{
	namespace io
	{
		namespace mask
		{

			/*! @class ITKImageFileMaskAccessorGenerator
			@brief Load Mask data using the itk loading methods and wraps the resulting itk image in a ITKImageMaskAccessor.
			* this is normally used if Mask distributions are stored in formats like meta image, nrrd...
			* @note it implies that the Mask information is stored in absolute Gy values.
			*/
			class ITKImageFileMaskAccessorGenerator: public core::MaskAccessorGeneratorBase
			{
			public:
				typedef MaskAccessorGeneratorBase::MaskAccessorPointer MaskAccessorPointer;

			private:
				FileNameType _itkMaskFileName;
				/** @brief The dose as itkImage */
				ITKImageMaskAccessor::ITKMaskImageType::Pointer _itkDoubleImage;

				ITKImageFileMaskAccessorGenerator();

				/*! @brief Casts into itkImage<DoseTypeGy,3>
				@details result is stored into _itkDoubleImage
				*/
				template <typename TPixelType> void doCasting(rttb::io::itk::GenericImageReader::GenericOutputImageType*
				        genericImage);

				/*! @brief Converts a generic image to itkImage<DoseTypeGy,3>
				@param itkGenericImage the image read by GenericImageReader
				@param loadedComponentType the component type (used for casting later on)
				@exception InvalidParameterException if component type is not supported
				@sa GenericImageReader
				*/
				void handleGenericImage(rttb::io::itk::GenericImageReader::GenericOutputImageType* itkGenericImage,
				                        ::itk::ImageIOBase::IOComponentType& loadedComponentType);


			public:
				~ITKImageFileMaskAccessorGenerator();

				ITKImageFileMaskAccessorGenerator(const FileNameType& fileName);

				/*! @brief Generate MaskAccessor
				@return Return shared pointer of MaskAccessor.
				@exception InvalidDoseException Thrown if file could not be read
				@exception InvalidParameterException Thrown if file has imageDimension !=3 or image component type != SCALAR
				@details is always converted into a itkImage<DoseTypeGy,3> by using a CastImageFilter
				@sa doCasting, handleGenericImage
				*/
				MaskAccessorPointer generateMaskAccessor();


			};
		}//end namespace itk
	}//end namespace io
}//end namespace rttb

#include "rttbITKImageFileMaskAccessorGenerator.tpp"

#endif
