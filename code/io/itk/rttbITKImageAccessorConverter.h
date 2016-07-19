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
#ifndef __ITK_IMAGE_ACCESSOR_CONVERTER_H
#define __ITK_IMAGE_ACCESSOR_CONVERTER_H

#include <boost/shared_ptr.hpp>

#include "rttbDoseAccessorProcessorBase.h"
#include "rttbDoseAccessorConversionSettingInterface.h"
#include "rttbITKImageAccessorConverter.h"

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
				typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;

				bool process();

				ITKImageType::Pointer getITKImage()
				{
					return _itkImage;
				}

				ITKImageAccessorConverter(DoseAccessorPointer accessor);
				virtual ~ITKImageAccessorConverter() {};

			private:
				ITKImageAccessorConverter(const
				                          ITKImageAccessorConverter&); //not implemented on purpose -> non-copyable
				ITKImageAccessorConverter& operator=(const
				                                     ITKImageAccessorConverter&);//not implemented on purpose -> non-copyable

				ITKImageType::Pointer _itkImage;

			};
		}
	}
}
#endif
