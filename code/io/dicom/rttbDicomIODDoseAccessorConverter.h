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
#ifndef __DICOM_IOD_DOSE_ACCESSOR_CONVERTER_H
#define __DICOM_IOD_DOSE_ACCESSOR_CONVERTER_H


#include "../itk/rttbDoseAccessorProcessorBase.h"
#include "../itk/rttbDoseAccessorConversionSettingInterface.h"
#include "rttbDicomDoseAccessor.h"

#include <cstdint>

//pixel data max value
#define PixelDataMaxValue UINT16_MAX

namespace rttb
{
	namespace io
	{
		namespace dicom
		{

			/*! @class DicomIODDoseAccessorConverter
				@brief Class converts/dumps the processed accessor into an dicom dose iod
				@remark DoseAccessorConversionInterface defines how the converter should react on non valid dose values.
				@remark Not implemented because of no usage now, please use DicomFileDoseAccessorConverter
			*/
			class DicomIODDoseAccessorConverter: public core::DoseAccessorProcessorBase,
				public core::DoseAccessorConversionSettingInterface
			{

			public:
				typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;
				typedef DicomDoseAccessor::DRTDoseIODPtr DRTDoseIODPointer;
				typedef boost::shared_ptr<DcmItem> DcmItemPtr;

				DicomIODDoseAccessorConverter(DoseAccessorPointer accessor);
				virtual ~DicomIODDoseAccessorConverter() {};

				bool process();//not implemented because of no usage now, please use DicomFileDoseAccessorConverter

				DRTDoseIODPointer getDicomDoseIOD()
				{
					return _doseIOD;
				}

			private:
				DicomIODDoseAccessorConverter(const
				                              DicomIODDoseAccessorConverter&); //not implemented on purpose -> non-copyable
				DicomIODDoseAccessorConverter& operator=(const
				        DicomIODDoseAccessorConverter&);//not implemented on purpose -> non-copyable


				DRTDoseIODPointer _doseIOD;
			};
		}
	}
}
#endif
