// -----------------------------------------------------------------------
// MatchPoint - DKFZ translational registration framework
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See mapCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/MatchPoint/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision: 793 $ (last changed revision)
// @date    $Date: 2014-10-10 10:24:45 +0200 (Fr, 10 Okt 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
// Subversion HeadURL: $HeadURL: https://svn/sbr/Sources/SBR-Projects/MatchPoint/trunk/Code/IO/include/mapImageReader.h $
*/


#include "rttbImageWriter.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace io
	{
		namespace itk {
			ImageWriter::ImageWriter(FileNameString aFileName, ITKImageTypePointer aITKImage)
				:_fileName(aFileName), _itkImage(aITKImage){

			}

			bool ImageWriter::writeITKFile(){ 			
				WriterType::Pointer writer = WriterType::New();
				writer->SetFileName(_fileName);
				writer->SetInput(_itkImage);
				try{
					writer->Update();
				}
				catch( ::itk::ExceptionObject & excp )
				{
					std::cerr << "Error: ITK Exception caught " << excp << std::endl;
					throw rttb::core::InvalidParameterException("ITK Exception writer->Update()!");
					return false;
				}

				return true;
			}
		}//end namespace itk
	}//end namespace io
}//end namespace rttb


