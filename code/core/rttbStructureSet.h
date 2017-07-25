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

#ifndef __STRUCTURE_SET_H
#define __STRUCTURE_SET_H

#include <vector>

#include <boost/shared_ptr.hpp>

#include "rttbBaseType.h"
#include "rttbStructure.h"

#include "RTTBCoreExports.h"

namespace rttb
{
	namespace core
	{

		/*! @class StructureSet
			@brief This is an class representing a structure set, which can be used to generate masks.
		*/
		class RTTBCore_EXPORT StructureSet
		{
		public:
			typedef Structure::StructTypePointer StructTypePointer;
			typedef size_t NumberOfStructuresType;

		protected:
			std::vector<StructTypePointer> _structureSetVector;

			IDType _UID;
			IDType _patientUID;


		public:
			virtual ~StructureSet() {};

			/*! @brief Constructor
				@param aPatientUID the patient UID.
				@param aUID the structure set UID. If it is empty, it will be calculated in the constructor
			*/
			StructureSet(const std::vector<StructTypePointer>& aStructureVector, IDType aPatientUID = "",
			             IDType aUID = "");

			/*! @brief Get the Structure with the index aStructureNo
				@return Return Structure pointer.
				@exception InvalidParameterException Thrown if structureNo not between 0 and number of structures
				of structureSet.
			*/
			StructTypePointer getStructure(size_t aStructureNo) const;

			/*! @brief Get the number of structures
				@return Return the number of structures.
			*/
			NumberOfStructuresType getNumberOfStructures() const;

			virtual IDType getUID() const;

			virtual IDType getPatientUID() const;

		};
	}
}

#endif
