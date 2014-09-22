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


#include "rttbStrVectorStructureSetGenerator.h"


namespace rttb{
	namespace core{		


		StrVectorStructureSetGenerator::StrVectorStructureSetGenerator(std::vector<StructTypePointer>& aStructureVector,IDType aPatientUID)
		{
			
			_patientUID= aPatientUID;
			_strVector=aStructureVector;
			
		}

		StrVectorStructureSetGenerator::StructureSetPointer StrVectorStructureSetGenerator::generateStructureSet(){
			return boost::make_shared<core::StructureSet>(_strVector,_patientUID);
		}
	}
}//end namespace rttb
