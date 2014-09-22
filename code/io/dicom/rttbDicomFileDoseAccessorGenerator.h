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
#ifndef __DICOM_FILE_DOSE_ACCESSOR_GENERATOR_H
#define __DICOM_FILE_DOSE_ACCESSOR_GENERATOR_H

#include <string>
#include <vector>
#include <exception>

#include "osconfig.h"    /* make sure OS specific configuration is included first */
#include "drtdose.h"

#include "rttbDoseAccessorGeneratorBase.h"
#include "rttbBaseType.h"


namespace rttb{
	namespace io{
		namespace dicom{

			/*! @class DicomFileDoseAccessorGenerator         
			@brief Load dose data from dicom file and generate DicomDoseAccessor.
			*/
			class DicomFileDoseAccessorGenerator: public core::DoseAccessorGeneratorBase
			{
			public: 							
				typedef boost::shared_ptr<DRTDoseIOD> DRTDoseIODPtr;
				
			private:
				FileNameType _dicomDoseFileName;
				
				DicomFileDoseAccessorGenerator(); 


			protected:


			public: 
				~DicomFileDoseAccessorGenerator();

				/*! @brief Constructor. Initialisation with a DICOM-RT dose file with name aDICOMRTDoseFileName
				
				*/
				DicomFileDoseAccessorGenerator(FileNameType aDICOMRTDoseFileName);

				/*! @brief Generate DoseAccessor 
				@return Return shared pointer of DoseAccessor. 
				@exception InvalidDoseException Thrown if the loaded dose is invalid: one of column/row/numberOfFrames/doseGridScaling/pixelSpacing=0
				@exception DcmrtException Throw if dcmrt error
				*/
				DoseAccessorPointer generateDoseAccessor() ;

			};
		}
	}
}

#endif
