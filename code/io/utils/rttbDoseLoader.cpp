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
#include "rttbExceptionMacros.h"

#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomHelaxFileDoseAccessorGenerator.h"
#include "rttbITKImageFileAccessorGenerator.h"

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
            rttb::core::DoseAccessorInterface::DoseAccessorPointer
                loadDicomDose(const std::string& fileName)
            {
                rttb::io::dicom::DicomFileDoseAccessorGenerator generator(fileName);
                return generator.generateDoseAccessor();
            }

            /*! @brief loads a helax dose from a file.
                @exception Throws an rttb::Exception if loading fails
                @sa DicomHelaxFileDoseAccessorGenerator
            */
            rttb::core::DoseAccessorInterface::DoseAccessorPointer
                loadHelaxDose(const std::string& path)
            {
                rttb::io::helax::DicomHelaxFileDoseAccessorGenerator generator(path);
                return generator.generateDoseAccessor();
            }

            /*! @brief loads an itk dose from a file.
                @exception Throws an rttb::Exception if loading fails.
                @details Might be of all formats that ITK know (*.mhd, *.nrrd, ...). The absolute image values are taken as dose.
                @sa ITKImageFileAccessorGenerator
            */
            rttb::core::DoseAccessorInterface::DoseAccessorPointer
                loadITKDose(const std::string& fileName)
            {
                rttb::io::itk::ITKImageFileAccessorGenerator generator(fileName);
                return generator.generateDoseAccessor();
            }

			rttb::core::DoseAccessorInterface::DoseAccessorPointer loadITKDicomDose(const std::string& fileName) {
				rttb::io::itk::ITKImageFileAccessorGenerator generator(fileName);
				return generator.generateDoseAccessor(true);
			}

            /*! @brief loads a dose from a file based on the loadingStyle.
                @params args[0]: determines the loadingStyle
                @exception Throws an rttb::Exception if loading fails
            */
            rttb::core::DoseAccessorInterface::DoseAccessorPointer
                loadDose(const std::string& fileName,
                    const std::vector<std::string>& args)
            {
                rttb::core::DoseAccessorInterface::DoseAccessorPointer result;

                if (args.empty() || args[0] == "dicom")
                {
                    result = loadDicomDose(fileName);
                }
                else if (args[0] == "helax")
                {
                    result = loadHelaxDose(fileName);
                }
                else if (args[0] == "itk")
                {
                    result = loadITKDose(fileName);
                }
				else if (args[0] == "itkDicom")
				{
					result = loadITKDicomDose(fileName);
				}
                else
                {
                    rttbDefaultExceptionStaticMacro(<< "Unknown io style selected. Cannot load data. Selected style: "
                        << args[0]);
                }

                return result;
            }
        }
    }
}
#endif
