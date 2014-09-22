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

#include <math.h>
#include <algorithm>
#include <sstream>
#include <stdlib.h>

//#include <boost/lexical_cast.hpp>

#include "rttbInvalidParameterException.h"
#include "rttbStructure.h"

#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbDicomIODStructureSetGenerator.h"
#include "rttbDcmrtException.h"


namespace rttb{
	namespace io{		
		namespace dicom{		


			DicomFileStructureSetGenerator::DicomFileStructureSetGenerator(DICOMRTFileNameString aDICOMRTStrSetFileName){
				
				_fileName=aDICOMRTStrSetFileName;
		
			}

			
			DicomFileStructureSetGenerator::~DicomFileStructureSetGenerator()
			{
				
			}

			DicomFileStructureSetGenerator::StructureSetPointer DicomFileStructureSetGenerator::generateStructureSet(){
				OFCondition status;

				DcmFileFormat fileformat;
				DRTStrSetIODPtr drtStrSetIODPtr=boost::make_shared<DRTStructureSetIOD>();

				status = fileformat.loadFile(_fileName.c_str());
				if (!status.good())
				{
					throw DcmrtException("Load rt structure set loadFile() failed!");
				}
				status = drtStrSetIODPtr->read(*fileformat.getDataset());
				if(!status.good())
				{
					throw DcmrtException("Read DRTStructureSetIOD DRTStructureSetIOD.read() failed!");
				}

				return (boost::make_shared<io::dicom::DicomIODStructureSetGenerator>(drtStrSetIODPtr))->generateStructureSet();
			}

		}//end namespace dicom
	}//end namespace io 
}//end namespace rttb
