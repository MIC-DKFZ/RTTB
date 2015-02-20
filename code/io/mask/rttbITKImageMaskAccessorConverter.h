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
#ifndef __ITK_IMAGE_MASK_ACCESSOR_CONVERTER_H
#define __ITK_IMAGE_MASK_ACCESSOR_CONVERTER_H

#include <boost/shared_ptr.hpp>

#include "rttbITKImageMaskAccessor.h"
#include "rttbMaskAccessorProcessorBase.h"
#include "../itk/rttbDoseAccessorConversionSettingInterface.h"

namespace rttb
{
	namespace io
	{
		namespace mask
		{
			
			/*! @class ITKImageMaskAccessorConverter
				@brief Class converts/dumps the processed accessor into an itk image
				@remark MaskAccessorConversionInterface defines how the converter should react on non valid Mask values.
			*/
			class ITKImageMaskAccessorConverter: public core::MaskAccessorProcessorBase,
				public rttb::core::DoseAccessorConversionSettingInterface
				
			{
			public:
				typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;

				bool process();

				ITKImageMaskAccessor::ITKMaskImageType::Pointer getITKImage()
				{
					return _itkImage;
				}

				ITKImageMaskAccessorConverter(MaskAccessorPointer accessor);
				virtual ~ITKImageMaskAccessorConverter() {};

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
