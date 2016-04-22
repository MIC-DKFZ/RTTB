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

#include <boost/regex.hpp>

namespace rttb
{
	namespace core
	{


		StrVectorStructureSetGenerator::StrVectorStructureSetGenerator(std::vector<StructTypePointer>&
		        aStructureVector, IDType aPatientUID)
		{

			_patientUID = aPatientUID;
			_strVector = aStructureVector;

		}

		StrVectorStructureSetGenerator::StructureSetPointer
		StrVectorStructureSetGenerator::generateStructureSet()
		{
      std::vector<StructTypePointer> _filteredStructs = _strVector;

      if (this->getStructureLabelFilterActive())
      {
        _filteredStructs.clear();

			  ::boost::regex e(this->getFilterRegEx());

        for(auto aStruct : _strVector)
        {
  				if (::boost::regex_match(aStruct->getLabel(), e))
	  			{
		  			_filteredStructs.push_back(aStruct);
			  	}
        }

      }
      
      return boost::make_shared<core::StructureSet>(_filteredStructs, _patientUID);
		}
	}
}//end namespace rttb
