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
// @version $Revision: 5603 $ (last changed revision)
// @date    $Date: 2015-08-24 15:17:30 +0200 (Mo, 24 Aug 2015) $ (last change date)
// @author  $Author: strubel $ (last changed by)
*/

#include <string>
#include <vector>

namespace rttb
{
	namespace apps
	{
		namespace voxelizer
		{
			/**@brief ListofExpression contains input expression
			@return a vector of found labels
			*/
			std::vector<int> filterForExpression(const std::vector<std::string>& listOfExpressions,
			                                     const std::string& inputExpression);
			/**@brief Search the label with the position from index
			@return a label from the list as string
			*/
			void removeSpecialCharacters(std::string& label);
			std::string getFilenameWithoutEnding(const std::string& outfilename);
			std::string getFileEnding(const std::string& outfilename);
		}
	}
}