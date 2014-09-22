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
#ifndef __ITK_IMAGE_DOSE_ACCESSOR_CONVERTER_H
#define __ITK_IMAGE_DOSE_ACCESSOR_CONVERTER_H

#include <boost/shared_ptr.hpp>

#include "rttbDoseAccessorProcessorBase.h"
#include "rttbDoseAccessorConversionSettingInterface.h"
#include "rttbITKImageDoseAccessor.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			/*! @class ITKImageDoseAccessorConverter
				@brief Class converts/dumps the processed accessor into an itk image
				@remark DoseAccessorConversionInterface defines how the converter should react on non valid dose values.
			*/
			class ITKImageDoseAccessorConverter: public core::DoseAccessorProcessorBase,
				public core::DoseAccessorConversionSettingInterface
			{
			public:
				typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;

				bool process();

				ITKDoseImageType::Pointer getITKImage()
				{
					return _itkImage;
				}

				ITKImageDoseAccessorConverter(DoseAccessorPointer accessor);
				virtual ~ITKImageDoseAccessorConverter() {};

			private:
				ITKImageDoseAccessorConverter(const
				                              ITKImageDoseAccessorConverter&); //not implemented on purpose -> non-copyable
				ITKImageDoseAccessorConverter& operator=(const
				        ITKImageDoseAccessorConverter&);//not implemented on purpose -> non-copyable

				ITKDoseImageType::Pointer _itkImage;

			};
		}
	}
}
#endif
