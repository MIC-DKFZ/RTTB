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
#ifndef __ITK_IMAGE_DOSE_FILE_ACCESSOR_GENERATOR_H
#define __ITK_IMAGE_DOSE_FILE_ACCESSOR_GENERATOR_H

#include <string>
#include <exception>

#include "rttbDoseAccessorGeneratorBase.h"
#include "rttbBaseType.h"
#include "rttbITKImageDoseAccessor.h"
#include "rttbGenericImageReader.h"

#include "itkImage.h"
#include "itkCastImageFilter.h"


namespace rttb
{
	namespace io
	{
		namespace itk
		{

			/*! @class ITKImageFileDoseAccessorGenerator
			@brief Load dose data using the itk loading methods and wraps the resulting itk image in a ITKImageDoseAccessor.
			* this is normally used if dose distributions are stored in formats like meta image, nrrd...
			* @note it implies that the dose information is stored in absolute Gy values.
			*/
			class ITKImageFileDoseAccessorGenerator: public core::DoseAccessorGeneratorBase
			{
			public:
				typedef DoseAccessorGeneratorBase::DoseAccessorPointer DoseAccessorPointer;

			private:
				FileNameType _dicomDoseFileName;
				/** @brief The dose as itkImage */
				ITKDoseImageType::Pointer _itkDoubleImage;

				ITKImageFileDoseAccessorGenerator();

				
			public:
				~ITKImageFileDoseAccessorGenerator();

				ITKImageFileDoseAccessorGenerator(const FileNameType& fileName);

				/*! @brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
				@exception InvalidDoseException Thrown if file could not be read
				@exception InvalidParameterException Thrown if file has imageDimension !=3 or image component type != SCALAR
				@details is always converted into a itkImage<DoseTypeGy,3> by using a CastImageFilter
				@sa doCasting, handleGenericImage
				*/
				DoseAccessorPointer generateDoseAccessor();


			};
		}//end namespace itk
	}//end namespace io
}//end namespace rttb


#endif
