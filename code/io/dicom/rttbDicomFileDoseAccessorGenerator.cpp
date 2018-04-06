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

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbDcmrtException.h"
#include "rttbIndexOutOfBoundsException.h"
#include "rttbDicomFileReaderHelper.h"
#include "rttbInvalidParameterException.h"
#include "rttbUtils.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{

			DicomFileDoseAccessorGenerator::~DicomFileDoseAccessorGenerator() = default;


			DicomFileDoseAccessorGenerator::DicomFileDoseAccessorGenerator(FileNameType aDICOMRTDoseFileName)
			{
				_dicomDoseFileName = aDICOMRTDoseFileName;

			}

			core::DoseAccessorGeneratorInterface::DoseAccessorPointer
			DicomFileDoseAccessorGenerator::generateDoseAccessor()
			{
				std::vector<FileNameString> fileVector;

				//if a file
				if (core::isFile(_dicomDoseFileName))
				{
					fileVector.push_back(_dicomDoseFileName);
				}
				//if a directory
				else if (core::isDirectory(_dicomDoseFileName))
				{
					rttb::io::dicom::Modality doseModality = {rttb::io::dicom::Modality::RTDOSE};
					fileVector = getFileNamesWithSameUID(_dicomDoseFileName, doseModality);
				}
				else
				{
					throw rttb::core::InvalidParameterException("Invalid file/directory name!");
				}

				if (fileVector.size() < 1)
				{
					throw rttb::core::InvalidParameterException("There is no structure set files in the directory!");
				}

				OFCondition status;

				DcmFileFormat fileformat;

				DRTDoseIODPtr dose = boost::make_shared<DRTDoseIOD>();
				status = fileformat.loadFile(fileVector.at(0).c_str());

				if (!status.good())
				{
					std::cerr << "Error: load rtdose loadFile(" << fileVector.at(0) << ") failed!" << std::endl;
					throw core::InvalidDoseException("Invalid dicom dose!");
				}

				DcmItemPtr dataSet =  boost::make_shared<DcmDataset>(*fileformat.getDataset());
				status = dose->read(*dataSet);

				if (!status.good())
				{
					std::cerr << "Error: read DRTDoseIOD failed!" << std::endl;
					throw core::InvalidDoseException("Invalid dicom dose!");
				}

				_doseAccessor = boost::make_shared<io::dicom::DicomDoseAccessor>(dose, dataSet);
				return _doseAccessor;


			}



		}
	}
}
