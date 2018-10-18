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

#ifndef __ITK_IMAGE_ACCESSOR_CONVERTER_H
#define __ITK_IMAGE_ACCESSOR_CONVERTER_H

#include "rttbDoseAccessorProcessorBase.h"
#include "rttbDoseAccessorConversionSettingInterface.h"

#include "itkImage.h"

#include "RTTBITKIOExports.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			/*! @class ITKImageAccessorConverter
				@brief Class converts/dumps the processed accessor into an itk image
				@remark DoseAccessorConversionInterface defines how the converter should react on non valid dose values.
			*/
            class RTTBITKIO_EXPORT ITKImageAccessorConverter : public core::DoseAccessorProcessorBase,
				public core::DoseAccessorConversionSettingInterface
			{
			public:
				typedef ::itk::Image<GenericValueType, 3> ITKImageType;
				using DoseAccessorPointer = core::DoseAccessorInterface::Pointer;

				bool process() override;

				ITKImageType::Pointer getITKImage()
				{
					return _itkImage;
				}

        explicit ITKImageAccessorConverter(DoseAccessorPointer accessor);
				~ITKImageAccessorConverter() override = default;

			private:
				ITKImageAccessorConverter(const
				                          ITKImageAccessorConverter&) = delete; //not implemented on purpose -> non-copyable
				ITKImageAccessorConverter& operator=(const
				                                     ITKImageAccessorConverter&) = delete;//not implemented on purpose -> non-copyable

				ITKImageType::Pointer _itkImage;

			};
		}
	}
}
#endif
