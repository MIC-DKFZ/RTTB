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

#include "rttbDoseLoader.h"
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
            rttb::core::DoseAccessorInterface::Pointer
                loadDicomDose(const std::string& fileName)
            {
                rttb::io::dicom::DicomFileDoseAccessorGenerator generator(fileName);
                return generator.generateDoseAccessor();
            }

            rttb::core::DoseAccessorInterface::Pointer
                loadHelaxDose(const std::string& path)
            {
                rttb::io::helax::DicomHelaxFileDoseAccessorGenerator generator(path);
                return generator.generateDoseAccessor();
            }

            rttb::core::DoseAccessorInterface::Pointer
                loadITKDose(const std::string& fileName)
            {
                rttb::io::itk::ITKImageFileAccessorGenerator generator(fileName);
                return generator.generateDoseAccessor();
            }

			rttb::core::DoseAccessorInterface::Pointer loadITKDicomDose(const std::string& fileName) {
				rttb::io::itk::ITKImageFileAccessorGenerator generator(fileName, true);
				return generator.generateDoseAccessor();
			}

            rttb::core::DoseAccessorInterface::Pointer
                loadDose(const std::string& fileName,
                    const std::string& loadStyle)
            {
                rttb::core::DoseAccessorInterface::Pointer result;

                if (loadStyle == "" || loadStyle == "dicom")
                {
                    result = loadDicomDose(fileName);
                }
                else if (loadStyle == "helax")
                {
                    result = loadHelaxDose(fileName);
                }
                else if (loadStyle == "itk")
                {
                    result = loadITKDose(fileName);
                }
				else if (loadStyle == "itkDicom")
				{
					result = loadITKDicomDose(fileName);
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
