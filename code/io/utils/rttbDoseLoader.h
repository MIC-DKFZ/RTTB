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

#ifndef __RTTB_DOSE_LOADER_H
#define __RTTB_DOSE_LOADER_H

#include "rttbDoseIteratorInterface.h"

namespace rttb
{
    namespace io
    {
        namespace utils 
        {
            /*! @brief loads a dicom dose from a file.
                @exception Throws an rttb::Exception if loading fails
                @sa DicomFileDoseAccessorGenerator
            */
			rttb::core::DoseAccessorInterface::Pointer loadDicomDose(const std::string& fileName);

            /*! @brief loads a helax dose from a file.
                @exception Throws an rttb::Exception if loading fails
                @sa DicomHelaxFileDoseAccessorGenerator
            */
			rttb::core::DoseAccessorInterface::Pointer loadHelaxDose(const std::string& path);     

            /*! @brief loads an itk dose from a file.
                @exception Throws an rttb::Exception if loading fails.
                @details Might be of all formats that ITK know (*.mhd, *.nrrd, ...). The absolute image values are taken as dose.
                @sa ITKImageFileAccessorGenerator
            */
			rttb::core::DoseAccessorInterface::Pointer loadITKDose(const std::string& fileName);
            
			rttb::core::DoseAccessorInterface::Pointer loadITKDicomDose(const std::string& fileName);

            /*! @brief loads a dose from a file based on the loadingStyle.
                @params args[0]: determines the loadingStyle
                @exception Throws an rttb::Exception if loading fails
            */
			rttb::core::DoseAccessorInterface::Pointer loadDose(const std::string& fileName, const std::string& loadStyle = "dicom");
        }
    }
}
#endif
