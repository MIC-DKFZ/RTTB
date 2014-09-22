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

#include <iostream>
#include <sstream>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "rttbStructureSet.h"
#include "rttbInvalidParameterException.h"

namespace rttb{
	namespace core{	

		StructureSet::StructureSet(){}

		StructureSet::StructureSet(std::vector<StructTypePointer> aStructureVector, 
			IDType aPatientUID, IDType aUID){
				_structureSetVector = aStructureVector;				
				_patientUID = aPatientUID;
				_UID = aUID;

				if(_UID==""){
					boost::uuids::uuid id;
					boost::uuids::random_generator generator;
					id = generator();
					std::stringstream ss;
					ss << id;			
					_UID = ss.str();
				}

		}

		StructureSet::StructTypePointer StructureSet::getStructure(int aStructureNo) const{
			int size = this->getNumberOfStructures()-1;
			if(aStructureNo < 0 || aStructureNo > size){
				std::stringstream sstr;
				sstr << "aStructureNo must between 0 and "<<size;
				throw InvalidParameterException(sstr.str());
			}
			return _structureSetVector.at(aStructureNo);
		}

		StructureSet::NumberOfStructuresType StructureSet::getNumberOfStructures() const{
			return _structureSetVector.size();
		}

		IDType StructureSet::getUID(){
			return _UID;
		}

		IDType StructureSet::getPatientUID(){

			return _patientUID;
		}

	}//end namespace core
}//end namespace rttb
