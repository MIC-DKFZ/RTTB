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

#ifndef __ITK_IO_HELPER_H
#define __ITK_IO_HELPER_H

#include "rttbBaseType.h"
#include "rttbGenericImageReader.h"

#include "itkImage.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			typedef ::itk::Image<GenericValueType, 3> ITKImageType;
			/*! @brief Read a itk image file into itkImage<DoseTypeGy,3>
			*/
			ITKImageType::Pointer readITKDoubleImage(FileNameType aITKImageFile, bool isDicom = false);

			/*! @brief Converts a generic image to itkImage<DoseTypeGy,3>
			@param itkGenericImage the image read by GenericImageReader
			@param loadedComponentType the component type (used for casting later on)
			@exception InvalidParameterException if component type is not supported
			@sa GenericImageReader
			*/
			ITKImageType::Pointer handleGenericImage(GenericImageReader::GenericOutputImageType*
			        itkGenericImage,
			        ::itk::IOComponentEnum& loadedComponentType);

			/*! @brief Casts into itkImage<DoseTypeGy,3>
			*/
			template <typename TPixelType> ITKImageType::Pointer doCasting(
			    GenericImageReader::GenericOutputImageType*
			    genericImage);


		}//end namespace itk
	}//end namespace io
}//end namespace rttb

#include "rttbITKIOHelper.tpp"

#endif
