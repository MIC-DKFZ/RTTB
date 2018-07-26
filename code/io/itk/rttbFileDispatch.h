// -----------------------------------------------------------------------
// MatchPoint - DKFZ translational registration framework
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See mapCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/MatchPoint/copyright.html
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the above copyright notices for more information.
//
//------------------------------------------------------------------------

#ifndef __RTTB_FILE_DISPATCH_H
#define __RTTB_FILE_DISPATCH_H

#include <string>
#include "rttbBaseType.h"

namespace rttb
{
	namespace core
	{
		/*! @class FileDispatch
		@brief Convenience functions for working with files and path
		@note code copied from MatchPoint, see documentation (http://sourceforge.net/projects/matchpoint/)
		*/

		class FileDispatch
		{
		public:
			/** Returns the name of the file (without extension).*/
			static FileNameString getName(const FileNameString& sFilePath);
			/** Returns the extansion of the file (dot included).*/
			static FileNameString getExtension(const FileNameString& sFilePath);
			/** Returns name of the file plus extension.*/
			static FileNameString getFullName(const FileNameString& sFilePath);
			/** Returns the directory the file is located in (without trailing slash).
			* @remark this function always removes the last element of the path. Thus
			* if you pass a path without a file, it will return the parent directory.*/
			static FileNameString getPath(const FileNameString& sFilePath);

			/** Convertes all path seperators in the seperators used in the current OS.*/
			static FileNameString ensureCorrectOSPathSeparator(const FileNameString& path);

			FileNameString getName();
			FileNameString getExtension();
			FileNameString getFullName();
			FileNameString getPath();

			FileDispatch(const FileNameString& filePath);

		private:
			FileNameString _fileName;
		};

	}//end namespace core
}//end namespace rttb

#endif
