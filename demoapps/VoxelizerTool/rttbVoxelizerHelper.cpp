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

#include "rttbVoxelizerHelper.h"
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <iostream>

namespace rttb
{
	namespace apps
	{
		namespace voxelizer
		{

			std::vector<int> filterForExpression(const std::vector<std::string>& listOfExpressions,
			                                     const std::string& inputExpression)
			{

				std::vector<int> listOfFoundElements;

				for (int j = 0; j < listOfExpressions.size(); j++)
				{
					boost::regex e(boost::algorithm::to_lower_copy(inputExpression));
					std::string s = boost::algorithm::to_lower_copy(listOfExpressions.at(j));

					if (boost::regex_match(s, e))
					{
						listOfFoundElements.push_back(j);
					}
				}

				return listOfFoundElements;
			}

			std::string getLabelFromList(std::vector<std::string> listOfExpressions, int j)
			{
				//Replace / to avoid problems with directories (struct "Magen/DD" --> Magen/DD.mhd), delete trailing . to avoid filenames with two trailing points (Niere re. --> Niere re..mhd)
				listOfExpressions.at(j).replace(listOfExpressions.at(j).find("/"), 1, "_");

				if (listOfExpressions.at(j).substr(listOfExpressions.at(j).size() - 1) == ".")
				{
					listOfExpressions.at(j).replace(listOfExpressions.at(j).size() - 1, 1, "");
				}

				std::cout << listOfExpressions.at(j) << std::endl;
				return listOfExpressions.at(j);
			}

			std::string getFilenameWithoutEnding(const std::string& outfilename)
			{
				boost::filesystem::path p(outfilename);
				return p.stem().string();
			}

			std::string getFileEnding(const std::string& outfilename)
			{
				boost::filesystem::path p(outfilename);
				return p.extension().string();
			}
		}
	}
}