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
/*!
// @file
// @version $Revision$ (last changed revision)
// @date    $Date$ (last change date)
// @author  $Author$ (last changed by)
// Subversion HeadURL: $HeadURL: https://svn/sbr/Sources/SBR-Projects/MatchPoint/trunk/Code/Core/source/mapFileDispatch.cpp $
*/

#include "rttbFileDispatch.h"

#include "itksys/SystemTools.hxx"

namespace rttb
{
	namespace core
	{
		FileNameString
		FileDispatch::
		getName(const FileNameString& sFilePath)
		{
			FileNameString result = itksys::SystemTools::GetFilenameWithoutLastExtension(sFilePath);
			return result;
		};

		FileNameString
		FileDispatch::
		getExtension(const FileNameString& sFilePath)
		{
			FileNameString result = itksys::SystemTools::GetFilenameLastExtension(sFilePath);
			return result;
		};

		FileNameString
		FileDispatch::
		getFullName(const FileNameString& sFilePath)
		{
			FileNameString result = itksys::SystemTools::GetFilenameName(sFilePath);
			return result;
		};

		FileNameString
		FileDispatch::
		getPath(const FileNameString& sFilePath)
		{
			FileNameString result = itksys::SystemTools::GetFilenamePath(sFilePath);
			return ensureCorrectOSPathSeparator(result);

		};

		FileNameString
		FileDispatch::
		getName()
		{
			return getName(_fileName);
		};

		FileNameString
		FileDispatch::
		getExtension()
		{
			return getExtension(_fileName);
		};

		FileNameString
		FileDispatch::
		getFullName()
		{
			return getFullName(_fileName);
		};

		FileNameString
		FileDispatch::
		getPath()
		{
			return getPath(_fileName);
		};

		FileDispatch::
		FileDispatch(const FileNameString& filePath)
		{
			_fileName = filePath;
		};

		/** Convertes all path seperators in the seperators used in the current OS.*/
		FileNameString
		FileDispatch::
		ensureCorrectOSPathSeparator(const FileNameString& path)
		{
			FileNameString ret = path;

			#ifdef _WIN32
			const FileNameString curSep = "\\";
			const char wrongSep = '/';
			#else
			const FileNameString curSep = "/";
			const char wrongSep = '\\';
			#endif

			FileNameString::size_type pos = ret.find_first_of(wrongSep);

			while (pos != FileNameString::npos)
			{
				ret.replace(pos, 1, curSep);

				pos = ret.find_first_of(wrongSep);
			}

			return ret;
		}


	}
}