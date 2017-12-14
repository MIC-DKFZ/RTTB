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
#ifndef __ITK_IMAGE_MASK_ACCESSOR_CONVERTER_H
#define __ITK_IMAGE_MASK_ACCESSOR_CONVERTER_H

#include "rttbITKImageMaskAccessor.h"
#include "rttbMaskAccessorProcessorBase.h"
#include "../itk/rttbDoseAccessorConversionSettingInterface.h"

#include "RTTBITKIOExports.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{

			/*! @class ITKImageMaskAccessorConverter
				@brief Class converts/dumps the processed accessor into an itk image
				@remark MaskAccessorConversionInterface defines how the converter should react on non valid Mask values.
			*/
            class RTTBITKIO_EXPORT ITKImageMaskAccessorConverter : public core::MaskAccessorProcessorBase,
				public rttb::core::DoseAccessorConversionSettingInterface

			{
			public:
				typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;

				bool process() override;

				const ITKImageMaskAccessor::ITKMaskImageType::Pointer getITKImage()
				{
					return _itkImage;
				}

				ITKImageMaskAccessorConverter(MaskAccessorPointer accessor);
				~ITKImageMaskAccessorConverter() override = default;;

			private:
				ITKImageMaskAccessorConverter(const
				                              ITKImageMaskAccessorConverter&); //not implemented on purpose -> non-copyable
				ITKImageMaskAccessorConverter& operator=(const
				        ITKImageMaskAccessorConverter&);//not implemented on purpose -> non-copyable

				ITKImageMaskAccessor::ITKMaskImageType::Pointer _itkImage;

			};
		}
	}
}
#endif
