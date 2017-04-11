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
// @version $Revision: 1127 $ (last changed revision)
// @date    $Date: 2015-10-01 13:33:33 +0200 (Do, 01 Okt 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/

#include "VoxelizerToolHelper.h"

#include <regex>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>

namespace rttb
{
	namespace apps
	{
		namespace voxelizerTool
		{

			std::vector<unsigned int> filterForExpression(const std::vector<std::string>& listOfExpressions,
			                                     const std::string& inputExpression)
			{
				std::vector<unsigned int> listOfFoundElements;

				for (unsigned int j = 0; j < listOfExpressions.size(); j++)
				{
					std::regex e(boost::algorithm::to_lower_copy(inputExpression));
					std::string s = boost::algorithm::to_lower_copy(listOfExpressions.at(j));

					if (std::regex_match(s, e))
					{
						listOfFoundElements.push_back(j);
					}
				}

				return listOfFoundElements;
			}

			void removeSpecialCharacters(std::string& label)
			{

				//Replace / to avoid problems with directories (struct "Magen/DD" --> Magen/DD.mhd), delete trailing . to avoid filenames with two trailing points (Niere re. --> Niere re..mhd)
				while (label.find("/") != std::string::npos)
				{
					label.replace(label.find("/"), 1, "_");
				}

				if (*label.rbegin() == '.')
				{
					label.replace(label.size() - 1, 1, "");
				}
			}

			std::string getFilenameWithoutEnding(const std::string& outfilename)
			{
				boost::filesystem::path p(outfilename);
				return p.replace_extension("").string();
			}

			std::string getFileEnding(const std::string& outfilename)
			{
				boost::filesystem::path p(outfilename);
				return p.extension().string();
			}
		}
	}
}