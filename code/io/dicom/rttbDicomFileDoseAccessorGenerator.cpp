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

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbDcmrtException.h"
#include "rttbIndexOutOfBoundsException.h"

namespace rttb{
	namespace io{
		namespace dicom{

			DicomFileDoseAccessorGenerator::~DicomFileDoseAccessorGenerator(){}


			DicomFileDoseAccessorGenerator::DicomFileDoseAccessorGenerator(FileNameType aDICOMRTDoseFileName){
				_dicomDoseFileName=aDICOMRTDoseFileName;
				
			}

			core::DoseAccessorGeneratorInterface::DoseAccessorPointer DicomFileDoseAccessorGenerator::generateDoseAccessor() {

				OFCondition status;

				DcmFileFormat fileformat;

				DRTDoseIODPtr dose= boost::make_shared<DRTDoseIOD>();
				status = fileformat.loadFile(_dicomDoseFileName.c_str());
				if (!status.good())
				{
					std::cerr << "Error: load rtdose loadFile("<<_dicomDoseFileName.c_str()<<") failed!"<<std::endl;
					throw core::InvalidDoseException("Invalid dicom dose!");
				}

				DcmItemPtr dataSet =  boost::make_shared<DcmDataset>(*fileformat.getDataset());
				status = dose->read(*dataSet);
				if(!status.good())
				{
					std::cerr << "Error: read DRTDoseIOD failed!"<<std::endl;
					throw core::InvalidDoseException("Invalid dicom dose!");
				}

				_doseAccessor=boost::make_shared<io::dicom::DicomDoseAccessor>(dose, dataSet);
				return _doseAccessor;

				
			}

			

		}
	}
}
