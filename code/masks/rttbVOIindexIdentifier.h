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

namespace rttb
{
	namespace masks
	{
		class VOIindexIdentifier
		{

		public:
			typedef ::boost::shared_ptr<VOIindexIdentifier>                            Pointer;
			typedef ::rttb::core::StructureSet                                         StructSetType;
			typedef ::boost::shared_ptr<StructSetType>                                 StructSetTypePointer;
			typedef ::rttb::core::Structure                                            StructType;

		public:

			VOIindexIdentifier() {}
			virtual ~VOIindexIdentifier() {}


			/*!@brief get the index of the corresponding VOI
			* @pre name must contain a valid VOI name
			* @pre spStructSet must point to a valid structure set.
			* @param spStructSet Pointer to the structure set that should be checked for the named VOI.
			* @param name Name of the VOI
			* @exception  ::rttb::core::Exception on invalid spStructSet
			              ::rttb::core::Exception on invalid spStructSet
			* @return the index */
			static const std::vector<unsigned int> getIndicesByVoiRegex(StructSetTypePointer spStructSet,
			        const std::string& name);

			/*!@brief get the VOI of the corresponding index
			* @pre index must specify a valid index value
			* @pre spStructSet must point to a valid structure set.
			* @param spStructSet Pointer to the structure set that should be checked for the named VOI.
			* @param name Index of the VOI
			* @exception  ::rttb::core::Exception on invalid spStructSet or index>maxLabelIndex
			* @return voi name */
			static const std::string getVoiNameByIndex(StructSetTypePointer spStructSet,
			        unsigned int index);


		};

	}
}

#endif
