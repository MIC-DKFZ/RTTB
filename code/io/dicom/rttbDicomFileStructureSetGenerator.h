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

/* Changes in Architecture:
The DICOM specific classes will be removed and transfered to the corresponding IO classes.
This class should only provide general structure functionality.
*/

#ifndef __DICOM_FILE_STRUCTURE_SET_GENERATOR_H
#define __DICOM_FILE_STRUCTURE_SET_GENERATOR_H


#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "drtstrct.h"

#include "rttbBaseType.h"
#include "rttbStrVectorStructureSetGenerator.h"

namespace rttb{
	namespace io{
		namespace dicom{

			/*! @class DicomFileStructureSetGenerator
			@brief Generate a structure set from a corresponding dicomRT file.
			*/
			class DicomFileStructureSetGenerator: public core::StrVectorStructureSetGenerator
			{
			public:
				typedef core::StructureSet::StructTypePointer StructTypePointer;
				typedef StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;
				typedef boost::shared_ptr<DRTStructureSetIOD> DRTStrSetIODPtr;

			private:
				IDType _UID;
				DICOMRTFileNameString _fileName;

				DicomFileStructureSetGenerator(){};

			public:
				
				/*! @brief Constructor 
				@param aDICOMRTStrSetFileName a DICOM-RT Structure set file name  
				
				*/
				DicomFileStructureSetGenerator(DICOMRTFileNameString aDICOMRTStrSetFileName);

				/*! @brief Destructor 
				*/
				~DicomFileStructureSetGenerator();

				/*! @brief generate structure set
				@return return shared pointer of StructureSet
				@exception DcmrtException Thrown if loadFile and read failed
				@exception InvalidParameterException throw if the imported header tags are not numerical.
				*/
				StructureSetPointer generateStructureSet();
			};
		}
	}
}

#endif
