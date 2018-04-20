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

#ifndef __RTTB_STRUCT_LOADER_H
#define __RTTB_STRUCT_LOADER_H

#include "rttbDicomFileStructureSetGenerator.h"

#include "rttbExceptionMacros.h"

namespace rttb
{
    namespace io
    {
        namespace utils 
        {
            /*! @brief loads a dicom struct from a file.
            You may pass a structure name regex. If is not empty, it will be used to filter structure in the
            loading process. Only structures with a name matching the reg ex will be loaded. This speeds up the
            loading process significantly if you need only one structure out of a structure set.
            @exception Throws an rttb::Exception if loading fails
            @sa DicomFileStructureSetGenerator
            */
            rttb::core::StructureSetGeneratorInterface::StructureSetPointer
                loadDicomStruct(
                    const std::string& fileName, const std::string& structNameRegex = "")
            {
                rttb::io::dicom::DicomFileStructureSetGenerator generator(fileName);

                if (!structNameRegex.empty())
                {
                    generator.setStructureLabelFilterActive(true);
                    generator.setFilterRegEx(structNameRegex);
                }

                return generator.generateStructureSet();
            }

            /*! @brief loads a struct from a file based on the loadingStyle.
            You may pass a structure name regex. If is not empty, it will be used to filter structure in the
            loading process. Only structures with a name matching the reg ex will be loaded. This speeds up the
            loading process significantly if you need only one structure out of a structure set.
            @exception Throws an rttb::Exception if loading fails
            @details voxelized itk images are read in generateMask() directly
            */
            rttb::core::StructureSetGeneratorInterface::StructureSetPointer loadStruct(
                const std::string& fileName, const std::string& loadStyle, const std::string& structNameRegex = "")
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
#endif
