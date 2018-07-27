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

#include <cmath>

#include "rttbInvalidParameterException.h"

#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbDicomIODStructureSetGenerator.h"
#include "rttbDcmrtException.h"
#include "rttbDicomFileReaderHelper.h"
#include "rttbUtils.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{


			DicomFileStructureSetGenerator::DicomFileStructureSetGenerator(DICOMRTFileNameString
			        aDICOMRTStrSetFileName)
			{

				_fileName = aDICOMRTStrSetFileName;

			}


			DicomFileStructureSetGenerator::~DicomFileStructureSetGenerator() = default;

			DicomFileStructureSetGenerator::StructureSetPointer
			DicomFileStructureSetGenerator::generateStructureSet()
			{
				std::vector<FileNameString> fileVector;

				//if a file
				if (core::isFile(_fileName))
				{
					fileVector.push_back(_fileName);
				}
				//if a directory
				else if (core::isDirectory(_fileName))
				{
					rttb::io::dicom::Modality strModality = {rttb::io::dicom::Modality::RTSTRUCT};
					fileVector = getFileNamesWithSameUID(_fileName, strModality);
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
				DRTStrSetIODPtr drtStrSetIODPtr = boost::make_shared<DRTStructureSetIOD>();


				//get the first structure set file
				status = fileformat.loadFile(fileVector.at(0).c_str());

				if (!status.good())
				{
					throw DcmrtException("Load rt structure set loadFile() failed!");
				}

				status = drtStrSetIODPtr->read(*fileformat.getDataset());

				if (!status.good())
				{
					throw DcmrtException("Read DRTStructureSetIOD DRTStructureSetIOD.read() failed!");
				}

				io::dicom::DicomIODStructureSetGenerator iodGenerator(drtStrSetIODPtr);
				iodGenerator.setStructureLabelFilterActive(this->getStructureLabelFilterActive());
				iodGenerator.setFilterRegEx(this->getFilterRegEx());
				return iodGenerator.generateStructureSet();
			}

		}//end namespace dicom
	}//end namespace io
}//end namespace rttb
