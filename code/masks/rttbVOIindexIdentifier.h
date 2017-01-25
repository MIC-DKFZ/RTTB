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
// @version $Revision:
// @date    $Date:
// @author  $Author:
*/

#ifndef __VOI_INDEX_IDENTIFIER_H
#define __VOI_INDEX_IDENTIFIER_H

#include "rttbStructureSet.h"
#include "rttbStructure.h"

#include "RTTBMasksExports.h"

namespace rttb
{
	namespace masks
	{
        /*! @class VOIindexIdentifier
        @brief Collection of functions for the identification of structs in RTSTRUCT files.
        */
		class RTTBMasks_EXPORT VOIindexIdentifier
		{

		public:
			typedef ::boost::shared_ptr<VOIindexIdentifier>                            Pointer;
			typedef ::rttb::core::StructureSet                                         StructSetType;
			typedef ::boost::shared_ptr<StructSetType>                                 StructSetTypePointer;
			typedef ::rttb::core::Structure                                            StructType;

		public:

			VOIindexIdentifier() {}
			virtual ~VOIindexIdentifier() {}


			/*!@brief get indices of all VOI that agree with the regex
			* @details if the regex does not agree with any VOI, the returning vector is empty.
			* @pre spStructSet must point to a valid structure set.
			* @param spStructSet Pointer to the structure set that should be checked for the named VOI.
			* @param name Regular expression of the VOI
			* @exception  ::rttb::core::Exception on invalid spStructSet
			* @return a vector of all found indices */
			static const std::vector<unsigned int> getIndicesByVoiRegex(StructSetTypePointer spStructSet,
			        const std::string& name);

			/*!@brief get the index of the corresponding VOI name
			* @details only if the exact name is found, the index will be given.
			* @pre name must contain a valid VOI name
			* @pre spStructSet must point to a valid structure set.
			* @param spStructSet Pointer to the structure set that should be checked for the named VOI.
			* @param name Name of the VOI
			* @exception  ::rttb::core::Exception on invalid spStructSet
			* @exception  ::rttb::core::Exception on invalid name (not found in structure set)
			* @return the index */
			static const unsigned int getIndexByVoiName(StructSetTypePointer spStructSet,
			        const std::string& name);

			/*!@brief get the VOI of the corresponding index
			* @pre index must specify a valid index value
			* @pre spStructSet must point to a valid structure set.
			* @param spStructSet Pointer to the structure set that should be checked for the named VOI.
			* @param index Index of the VOI
			* @exception  ::rttb::core::Exception on invalid spStructSet or index>maxLabelIndex
			* @return voi name */
			static const std::string getVoiNameByIndex(StructSetTypePointer spStructSet,
			        unsigned int index);


		};

	}
}

#endif
