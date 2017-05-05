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
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
*/

#include "rttbDVHDicomFileReader.h"
#include "rttbException.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{
			DVHDicomFileReader::DVHDicomFileReader(FileNameString aFileName)
			{
				this->setFileName(aFileName);
				this->createDVH();
			}

			void DVHDicomFileReader::setFileName(FileNameString aFileName)
			{
				_fileName = aFileName;
			}

			void DVHDicomFileReader::createDVH()
			{
				assert(false);
				throw rttb::core::Exception("DICOM DVH reader currently not implemented.");
			};

		}
	}
}

