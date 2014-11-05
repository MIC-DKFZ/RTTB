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

#include "rttbDicomIODDoseAccessorGenerator.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbDcmrtException.h"
#include "rttbIndexOutOfBoundsException.h"

namespace rttb{
	namespace io{
		namespace dicom{


			DicomIODDoseAccessorGenerator::~DicomIODDoseAccessorGenerator(){}

			DicomIODDoseAccessorGenerator::DicomIODDoseAccessorGenerator(DRTDoseIODPtr aDRTDoseIODP){
				_doseIODPtr=aDRTDoseIODP;	
				
			}

			core::DoseAccessorGeneratorInterface::DoseAccessorPointer DicomIODDoseAccessorGenerator::generateDoseAccessor() {
				DcmItem doseitem;
				OFCondition status;

				status = _doseIODPtr->write(doseitem);//write DoseIOD to DcmItem to get pixel data

				if(status.good()){
					DcmItemPtr dataSetPtr = boost::make_shared<DcmItem>(doseitem);
					_doseAccessor=boost::make_shared<io::dicom::DicomDoseAccessor>(_doseIODPtr, dataSetPtr);
					return _doseAccessor;
				}
				else
				{
					throw io::dicom::DcmrtException("Write DICOM RT Dose to DcmItem failed!");
				}
			}
			

		}
	}
}
