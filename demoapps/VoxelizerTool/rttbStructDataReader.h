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
#ifndef __StructDataReader_h
#define __StructDataReader_h

#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbOTBMaskAccessor.h"

namespace rttb
{
	namespace apps
	{
		namespace voxelizer
		{
			/*! @class StructDataReader
			@brief StructDataReader read struct and reference file
			*/
			class StructDataReader
			{

			public:
				typedef rttb::core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
				typedef rttb::core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

				StructDataReader(const std::string& structFileName, const std::string& referenceFileName,
				                 const std::vector<std::string>& referenceFileLoadingStyle);

				/*!@brief Reads structure and reference file and saves the result in variables */
				void read();

				/**@brief read all labels an save it in a vector.
				@return a vector of all labels
				*/
				std::vector<std::string> getAllLabels() const;
				/**@brief
				@return the object _rtStructureSet
				*/
				StructureSetPointer getStructureSetPointer() const;
				/**@brief
				@return the object _doseAccessor
				*/
				DoseAccessorPointer getDoseAccessorPointer() const;

			private:

				/**@brief read a reference file
				@return the result as object
				*/
				DoseAccessorPointer readReferenceFile(const std::string& filename,
				                                      const std::vector<std::string>& fileLoadingStyle) const;

				DoseAccessorPointer readDicomFile(const std::string& filename) const;
				DoseAccessorPointer readITKFile(const std::string& filename) const;

				/**@brief read a struct file
				@return the result as object
				*/
				StructureSetPointer readStructFile(const std::string& filename) const;

				StructureSetPointer _rtStructureSet;
				DoseAccessorPointer _doseAccessor;

				std::string _structFilename;
				std::string _referenceFilename;
				std::vector<std::string> _referenceFileLoadingStyle;
			};
		}
	}
}
#endif