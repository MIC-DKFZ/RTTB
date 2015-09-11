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
#ifndef __ITK_IMAGE_ACCESSOR_GENERATOR_H
#define __ITK_IMAGE_ACCESSOR_GENERATOR_H

#include "rttbDoseAccessorGeneratorBase.h"
#include "rttbBaseType.h"
#include "rttbITKImageAccessorConverter.h"

#include "itkImage.h"

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
			class ITKImageAccessorGenerator: public core::DoseAccessorGeneratorBase
			{
			public:
				typedef ::itk::Image<GenericValueType, 3> ITKImageType;
				typedef DoseAccessorGeneratorBase::DoseAccessorPointer DoseAccessorPointer;

			private:
				/** @brief The dose as itkImage */
				ITKImageType::ConstPointer _dosePtr;

				ITKImageAccessorGenerator();

			public:
				virtual ~ITKImageAccessorGenerator() {};

				/*!
				@pre aDoseImage must point to a valid instance.
				@exception InvalidDoseException Thrown if aDoseImage is invalid.
				*/
				ITKImageAccessorGenerator(const ITKImageType* aDoseImage);

				/*! @brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
				*/
				DoseAccessorPointer generateDoseAccessor() ;

			};
		}
	}
}

#endif
