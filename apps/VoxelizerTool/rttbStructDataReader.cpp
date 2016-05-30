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
// @version $Revision: 1334 $ (last changed revision)
// @date    $Date: 2016-04-22 11:13:22 +0200 (Fr, 22 Apr 2016) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "rttbStructDataReader.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbITKImageFileAccessorGenerator.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizer
		{
			StructDataReader::StructDataReader(const std::string& structFileName,
			                                   const std::string& referenceFileName,
			                                   const std::vector<std::string>& referenceFileLoadingStyle) : _referenceFilename(referenceFileName),
				_structFilename(structFileName), _referenceFileLoadingStyle(referenceFileLoadingStyle)
			{
			}

			void StructDataReader::read()
			{
				_doseAccessor = readReferenceFile(_referenceFilename, _referenceFileLoadingStyle);
				_rtStructureSet = readStructFile(_structFilename);
			}

			std::vector<std::string> StructDataReader::getAllLabels() const
			{
				std::vector<std::string> allLabels;

				if (_rtStructureSet != NULL)
				{
					for (size_t j = 0; j < _rtStructureSet->getNumberOfStructures(); j++)
					{
						allLabels.push_back(_rtStructureSet->getStructure(j)->getLabel());
					}
				}

				return allLabels;
			}

			StructDataReader::StructureSetPointer StructDataReader::getStructureSetPointer() const
			{
				return _rtStructureSet;
			}

			StructDataReader::DoseAccessorPointer StructDataReader::getDoseAccessorPointer() const
			{
				return _doseAccessor;
			}

			StructDataReader::DoseAccessorPointer StructDataReader::readReferenceFile(
			    const std::string& filename, const std::vector<std::string>& fileLoadingStyle) const
			{
				if (fileLoadingStyle.at(0) == "dicom")
				{
					return readDicomFile(filename);
				}
				else if (fileLoadingStyle.at(0) == "itk")
				{
					return readITKFile(filename);
				}
				else
				{
					return NULL;
				}

			}

			StructDataReader::DoseAccessorPointer StructDataReader::readDicomFile(
			    const std::string& filename) const
			{
				rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator(filename.c_str());
				return doseAccessorGenerator.generateDoseAccessor();
			}

			StructDataReader::DoseAccessorPointer StructDataReader::readITKFile(const std::string& filename)
			const
			{
				rttb::io::itk::ITKImageFileAccessorGenerator generator(filename);
				return generator.generateDoseAccessor();
			}

			StructDataReader::StructureSetPointer StructDataReader::readStructFile(
			    const std::string& filename) const
			{
				StructureSetPointer rtStructureSet = rttb::io::dicom::DicomFileStructureSetGenerator(
				        filename.c_str()).generateStructureSet();
				return rtStructureSet;
			}





		}
	}
}
