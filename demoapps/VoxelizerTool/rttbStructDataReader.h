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

				/*!@brief Constructor
				@details calls readStructFile and readReferenceFile method and save the result in _rtStructureSet and _doseAccessor
				@param structfile
				@param referencefile
				*/
				StructDataReader(const std::string& structfile, const std::string& referencefile);
				/**@brief read all labels an save it in a vector.
				@return a vector of all labels
				*/
				std::vector<std::string> getAllLabels();
				/**@brief
				@return the objekt _rtStructureSet
				*/
				StructureSetPointer getStructureSetPointer();
				/**@brief
				@return the objekt _doseAccessor
				*/
				DoseAccessorPointer getDoseAccessorPointer();

			private:

				/**@brief read a referencefile
				@return the result as object
				*/
				DoseAccessorPointer readReferenceFile(const std::string& referencefile);
				/**@brief read a structfile
				@return the result as object
				*/
				StructureSetPointer readStructFile(const std::string& structfile);

				StructureSetPointer _rtStructureSet;
				DoseAccessorPointer _doseAccessor;
			};
		}
	}
}
#endif