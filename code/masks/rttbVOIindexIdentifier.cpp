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


#include "rttbVOIindexIdentifier.h"

#include <regex>

#include "rttbExceptionMacros.h"



namespace rttb
{
	namespace masks
	{

		const std::vector<size_t> VOIindexIdentifier::getIndicesByVoiRegex(
		    StructSetTypePointer spStructSet,
		    const std::string& nameAsRegEx)
		{
			if (spStructSet == nullptr)
			{
				rttbDefaultExceptionStaticMacro("spStructSet is nullptr");
			}

			std::vector<size_t> resultVOiIndices;
			std::regex e(nameAsRegEx);

			for (size_t i = 0; i < spStructSet->getNumberOfStructures(); i++)
			{
				std::string s = spStructSet->getStructure(i)->getLabel();

				if (std::regex_match(s, e))
				{
					resultVOiIndices.push_back(i);
				}
			}

			return resultVOiIndices;
		}


		const unsigned int VOIindexIdentifier::getIndexByVoiName(StructSetTypePointer spStructSet,
		        const std::string& name)
		{
			if (spStructSet == nullptr)
			{
				rttbDefaultExceptionStaticMacro("spStructSet is nullptr");
			}

			for (unsigned int i = 0; i < spStructSet->getNumberOfStructures(); i++)
			{
				if (spStructSet->getStructure(i)->getLabel() == name)
				{
					return i;
				}
			}

			rttbDefaultExceptionStaticMacro("no VOI was found with the given name");
		}



		const std::string VOIindexIdentifier::getVoiNameByIndex(StructSetTypePointer spStructSet,
		        unsigned int index)
		{
			if (spStructSet == nullptr)
			{
				rttbDefaultExceptionStaticMacro("spStructSet is nullptr!");
			}

			if (index >= spStructSet->getNumberOfStructures())
			{
				rttbDefaultExceptionStaticMacro("invalid index, voiLabelList out of range");
			}

			return  spStructSet->getStructure(index)->getLabel();
		}
	}
}

