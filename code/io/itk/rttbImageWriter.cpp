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

#include "rttbImageWriter.h"
#include "rttbITKException.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			ImageWriter::ImageWriter(const FileNameString& aFileName, ITKImageTypeConstPointer aITKImage)
				: _fileName(aFileName), _itkImage(aITKImage)
			{

			}

			bool ImageWriter::writeFile()
			{
				WriterType::Pointer writer = WriterType::New();
				writer->SetFileName(_fileName);
                writer->SetUseCompression(true);
				writer->SetInput(_itkImage);

				try
				{
					writer->Update();
				}
				catch (::itk::ExceptionObject& excp)
				{
					std::cerr << "Error: ITK Exception caught " << excp << std::endl;
					throw rttb::io::itk::ITKException("ITK Exception in writing image: writer->Update()!");
					return false;
				}

				return true;
			}
		}//end namespace itk
	}//end namespace io
}//end namespace rttb


