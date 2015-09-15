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
// @version $Revision: 5603 $ (last changed revision)
// @date    $Date: 2015-08-24 15:17:30 +0200 (Mo, 24 Aug 2015) $ (last change date)
// @author  $Author: strubel $ (last changed by)
*/

#include "rttbStructDataReader.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizer
		{
			StructDataReader::StructDataReader(const std::string& structfile, const std::string& reference)
			{
				_doseAccessor = readReferenceFile(reference);
				_rtStructureSet = readStructFile(structfile);
			}

			std::vector<std::string> StructDataReader::getAllLabels() const
			{
				std::vector<std::string> allLabels;

				if (_rtStructureSet != NULL)
				{
					for (int j = 0; j < _rtStructureSet->getNumberOfStructures(); j++)
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
			    const std::string& referencefile) const
			{
				rttb::io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(referencefile.c_str());
				DoseAccessorPointer doseAccessor(doseAccessorGenerator1.generateDoseAccessor());
				return doseAccessor;
			}

			StructDataReader::StructureSetPointer StructDataReader::readStructFile(
			    const std::string& structfile) const
			{
				StructureSetPointer rtStructureSet = rttb::io::dicom::DicomFileStructureSetGenerator(
				        structfile.c_str()).generateStructureSet();
				return rtStructureSet;
			}

		}
	}
}