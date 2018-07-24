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

#include "rttbStrVectorStructureSetGenerator.h"

#include <regex>
#include "boost/make_shared.hpp"

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

				std::regex e(this->getFilterRegEx());

				std::vector<StructTypePointer>::iterator it;
				for(it= _strVector.begin();it!=_strVector.end();++it)
				{
					if (std::regex_match((*it)->getLabel(), e))
					{
						_filteredStructs.push_back((*it));
					}
				}

			}

			return boost::make_shared<core::StructureSet>(_filteredStructs, _patientUID);
		}
	}
}//end namespace rttb
