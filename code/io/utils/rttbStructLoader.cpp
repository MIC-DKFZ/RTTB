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

#include "rttbStructLoader.h"

#include "rttbExceptionMacros.h"

namespace rttb
{
    namespace io
    {
        namespace utils 
        {
            rttb::core::StructureSetGeneratorInterface::StructureSetPointer
                loadDicomStruct(
                    const std::string& fileName, const std::string& structNameRegex)
            {
                rttb::io::dicom::DicomFileStructureSetGenerator generator(fileName);

                if (!structNameRegex.empty())
                {
                    generator.setStructureLabelFilterActive(true);
                    generator.setFilterRegEx(structNameRegex);
                }

                return generator.generateStructureSet();
            }

            rttb::core::StructureSetGeneratorInterface::StructureSetPointer loadStruct(
                const std::string& fileName, const std::string& loadStyle, const std::string& structNameRegex)
            {
                rttb::core::StructureSetGeneratorInterface::StructureSetPointer result;
                
                if (loadStyle == "" || loadStyle == "dicom")
                {
                    result = rttb::io::utils::loadDicomStruct(fileName, structNameRegex);
                }
                else
                {
                    rttbDefaultExceptionStaticMacro(<< "Unknown io style selected. Cannot load data. Selected style: " << loadStyle);
                }

                return result;
            }
        }
    }
}
