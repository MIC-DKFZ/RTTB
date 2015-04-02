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
#ifndef __DICOM_FILE_DOSE_ACCESSOR_CONVERTER_H
#define __DICOM_FILE_DOSE_ACCESSOR_CONVERTER_H


#include "../itk/rttbDoseAccessorProcessorBase.h"
#include "../itk/rttbDoseAccessorConversionSettingInterface.h"
#include "rttbDicomDoseAccessor.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{
			/*! @class DicomFileDoseAccessorConverter
				@brief Class converts/dumps the processed accessor into an dicom file
				@remark DoseAccessorConversionInterface defines how the converter should react on non valid dose values.
			*/
			class DicomFileDoseAccessorConverter: public core::DoseAccessorProcessorBase,
				public core::DoseAccessorConversionSettingInterface
			{
			public:
				typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;
				typedef DicomDoseAccessor::DRTDoseIODPtr DRTDoseIODPointer;

				/*! @brief Constructor. Initialisation with a dose accessor and a file name to write the dose
				@param aFileName a file name to write the dose 
				*/
				DicomFileDoseAccessorConverter(DoseAccessorPointer aAccessor, DICOMRTFileNameString aFileName);

				virtual ~DicomFileDoseAccessorConverter() {};

				/*! @brief Convert the accessor into dicom dataset
				@exception InvalidDoseException thrown if put and insert pixel data into dicom dataset failed
				*/
				bool process();

				/*! @brief Write dicom dataset to a file
				@pre process() should be true before writeDicomDoseFile() is called
				*/
				void writeDicomDoseFile();

				
			private:
				DicomFileDoseAccessorConverter(const
				                              DicomFileDoseAccessorConverter&); //not implemented on purpose -> non-copyable
				DicomFileDoseAccessorConverter& operator=(const
				        DicomFileDoseAccessorConverter&);//not implemented on purpose -> non-copyable

				/* code from plastimatch
					std::string formatting by Erik Aronesty
					http://stackoverflow.com/questions/2342162/stdstring-formating-like-sprintf
					Distributed under Attribution-ShareAlike 3.0 Unported license (CC BY-SA 3.0) 
					http://creativecommons.org/licenses/by-sa/3.0/
				*/
				std::string string_format (const char *fmt, va_list ap);
				std::string string_format (const char *fmt, ...);

				DRTDoseIODPointer _doseIOD;
				DICOMRTFileNameString _fileName;
				DcmFileFormat _fileformat;
				DcmDataset *_dataset;

			};
		}
	}
}
#endif
