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

#include <rttbUtils.h>
#include <filesystem>

namespace rttb {
	namespace core {

		bool isKey(const std::map<double, double>& values, const double value) {
			for (auto const& collectionElements : values) {
				if (std::abs(collectionElements.first - value) <= errorConstant) {
					return true;
				}
			}
			return false;
		}

		bool isKey(const std::vector<double>& values, double value) {
			for (auto const& collectionElement : values) {
				if (std::abs(collectionElement - value) <= errorConstant) {
					return true;
				}
			}
			return false;
		}

		bool valueIsClose(double value1, double value2, double specificErrorConstant) {
			return std::abs(value1 - value2) < specificErrorConstant;
		}

		bool isFile(FileNameType aName) {
			std::filesystem::path path(aName);
			return (std::filesystem::exists(path) && std::filesystem::is_regular_file(path));
		}

		bool isDirectory(FileNameType aName) {
			std::filesystem::path path(aName);
			return (std::filesystem::exists(path) && std::filesystem::is_directory(path));
		}

		std::string getFilenameWithoutEnding(const std::string& outfilename) {
			std::filesystem::path p(outfilename);
			return p.replace_extension("").string();
		}

		std::string getFileEnding(const std::string& outfilename) {
			std::filesystem::path p(outfilename);
			return p.extension().string();
		}
	}
}
