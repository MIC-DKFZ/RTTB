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
#ifndef __DICOM_FILE_DOSE_ACCESSOR_WRITER_H
#define __DICOM_FILE_DOSE_ACCESSOR_WRITER_H


#include "../itk/rttbDoseAccessorProcessorBase.h"
#include "../itk/rttbDoseAccessorConversionSettingInterface.h"
#include "rttbDicomDoseAccessor.h"

//pixel data max value
#define PixelDataMaxValue UINT16_MAX

namespace rttb
{
	namespace io
	{
		namespace dicom
		{
			/*! @class DicomFileDoseAccessorWriter
				@brief Class converts/dumps the processed accessor into an dicom file
				@remark DoseAccessorConversionInterface defines how the converter should react on non valid dose values.
			*/
			class DicomFileDoseAccessorWriter: public core::DoseAccessorProcessorBase,
				public core::DoseAccessorConversionSettingInterface
			{
			public:
				typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;
				typedef DicomDoseAccessor::DRTDoseIODPtr DRTDoseIODPointer;

				/*! @brief Standard Constructor.
				*/
				DicomFileDoseAccessorWriter();

				virtual ~DicomFileDoseAccessorWriter() {};

				/*! Set a file name to write the dose
				@param aFileName a file name to write the dose 
				*/
				void setFileName(DICOMRTFileNameString aFileName);

				/*! @brief Convert the accessor into dicom dataset and write dicom dataset to a file
				@exception InvalidDoseException thrown if put and insert pixel data into dicom dataset failed
				*/
				bool process();

				
				
			private:
				DicomFileDoseAccessorWriter(const
				                              DicomFileDoseAccessorWriter&); //not implemented on purpose -> non-copyable
				DicomFileDoseAccessorWriter& operator=(const
				        DicomFileDoseAccessorWriter&);//not implemented on purpose -> non-copyable

				DRTDoseIODPointer _doseIOD;
				DICOMRTFileNameString _fileName;
				DcmFileFormat _fileformat;
				DcmDataset *_dataset;

			};
		}
	}
}
#endif
