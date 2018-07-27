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

#include "rttbDicomHelaxFileDoseAccessorGenerator.h"
#include "rttbDicomHelaxDoseAccessor.h"
#include "rttbInvalidDoseException.h"
#include "rttbDicomFileReaderHelper.h"

namespace rttb
{
	namespace io
	{
		namespace helax
		{
			DicomHelaxFileDoseAccessorGenerator::~DicomHelaxFileDoseAccessorGenerator() = default;

			DicomHelaxFileDoseAccessorGenerator::DicomHelaxFileDoseAccessorGenerator(
			    FileNameType aDICOMRTDoseDirName)
			{
				_doseDirName = aDICOMRTDoseDirName;


			}

			core::DoseAccessorGeneratorInterface::DoseAccessorPointer
			DicomHelaxFileDoseAccessorGenerator::generateDoseAccessor()
			{
				rttb::io::dicom::Modality doseModality = {rttb::io::dicom::Modality::RTDOSE};
				std::vector<FileNameType> fileVector = rttb::io::dicom::getFileNamesWithSameUID(_doseDirName,
				                                       doseModality);
				OFCondition status;
				DcmFileFormat fileformat;
				std::vector<DRTDoseIODPtr> doseVector;

				for (auto & i : fileVector)
				{
					DRTDoseIODPtr dose = boost::make_shared<DRTDoseIOD>();

					status = fileformat.loadFile(i.c_str());

					if (!status.good())
					{
						throw core::InvalidDoseException("Error: load dose fileformat.loadFile failed!");
					}

					status = dose->read(*fileformat.getDataset());

					if (!status.good())
					{
						throw core::InvalidDoseException("Error: read DRTDoseIOD failed!");
					}

					doseVector.push_back(dose);
				}

				_doseAccessor = boost::make_shared<io::helax::DicomHelaxDoseAccessor>(doseVector);
				return _doseAccessor;

			}




		}
	}
}
