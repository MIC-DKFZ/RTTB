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
#ifndef __VIRTUOS_PLAN_FILE_DOSE_ACCESSOR_GENERATOR_H
#define __VIRTUOS_PLAN_FILE_DOSE_ACCESSOR_GENERATOR_H

#include <string>
#include <vector>
#include <exception>

#include "rttbDoseAccessorGeneratorBase.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"

#include "ncfile.h"

namespace rttb
{
	namespace io
	{
		namespace virtuos
		{

			/*! @class VirtuosPlanFileDoseAccessorGenerator         
			@brief Load dose data from a Virtuos dose file and a Virtuos plan file, generate DoseAccessor.
			@detail Through the use of the plan file, the dose file can be normalized, i.e. the DoseAccessor returns values in Gy.
			*/
			class VirtuosPlanFileDoseAccessorGenerator: public core::DoseAccessorGeneratorBase
			{
			public:
				typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;

			private: 
				FileNameType _doseFileName;
				FileNameType _planFileName;

				Cubeinfo **_pPointerOnVirtuosCube;

				/*! prepare virtuos dose cube for data import. Actual data import is performed in importPixelData()
				and assembleGeometricInfo().
				@pre filename needs to point to an existing file.
				@throw InvalidParameterException Thrown if aVirtuosDoseFileName is invalid or if virtuos cube can not be read
				*/
				void initializeVirtuosCube(FileNameType aVirtuosDoseFileName);

				VirtuosPlanFileDoseAccessorGenerator();

			protected:
				


			public: 
				~VirtuosPlanFileDoseAccessorGenerator();
				/*! @brief Constructor. Initialisation with a Virtuos dose file and a Virtuos plan file. Information for dose scaling, 
				etc. is taken from the corresponding plan file aVirtuosPlanFileName.
				
				*/
				VirtuosPlanFileDoseAccessorGenerator(const FileNameType aVirtuosDoseFileName,
				                                     const FileNameType aVirtuosPlanFileName);

				/*@brief Generate DoseAccessor 
				@return Return shared pointer of DoseAccessor. 
				@throw InvalidParameterException Thrown if one of the file names is empty.
				@throw InvalidParameterException Thrown if plan file or virtuos cube could not be read.
				*/
				DoseAccessorPointer generateDoseAccessor() ;
				
			};
		}
	}
}

#endif
