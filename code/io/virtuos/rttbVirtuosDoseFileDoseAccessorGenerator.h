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
#ifndef __VIRTUOS_DOSE_FILE_DOSE_ACCESSOR_GENERATOR_H
#define __VIRTUOS_DOSE_FILE_DOSE_ACCESSOR_GENERATOR_H

#include <string>
#include <vector>
#include <exception>

#include "rttbDoseAccessorGeneratorBase.h"
#include "rttbDoseAccessorInterface.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"

#include "ncfile.h"

namespace rttb
{
	namespace io
	{
		namespace virtuos
		{

			/*! @class VirtuosDoseFileDoseAccessorGenerator
			@brief Load dose data from a Virtuos dose file and generate DoseAccessor.
			@warning Manual dose normalization is done as no virtuos plan file is given! I.e., the DoseAccessor returns pixels having no physical unit (!= Gy) in case of wrong values. It's the responsibility of the user to specify the correct values. Consider using VirtuosPlanFileDoseAccessorGenerator instead!
			*/
			class VirtuosDoseFileDoseAccessorGenerator: public core::DoseAccessorGeneratorBase
			{
			public:
				typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;

			private:
				FileNameType _doseFileName;
				DoseTypeGy _normalizationDose;
				DoseTypeGy _prescribedDose;
				Cubeinfo** _pPointerOnVirtuosCube;

				/*! prepare virtuos dose cube for data import. Actual data import is performed in importPixelData()
				and assembleGeometricInfo().
				@pre filename needs to point to an existing file.
				@throw InvalidParameterException Thrown if aVirtuosDoseFileName is invalid or if virtuos cube can not be read
				*/
				void initializeVirtuosCube(FileNameType aVirtuosDoseFileName);

				VirtuosDoseFileDoseAccessorGenerator();


			protected:



			public:

				~VirtuosDoseFileDoseAccessorGenerator();

				/*! @brief Constructor. Initialisation with a Virtuos-RT dose file with name aVirtuosDoseFileName.
				@param normalizationDose is defined as (prescribedDose*1000)/maxDoseInGy.
				@param prescribedDose the does that was planned in the reference point in Gy.
				*/
				VirtuosDoseFileDoseAccessorGenerator(const FileNameType aVirtuosDoseFileName,
				                                     DoseTypeGy normalizationDose, DoseTypeGy prescribedDose);

				/*@brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
				@throw InvalidParameterException Thrown if one of the file names is empty.
				*/
				DoseAccessorPointer generateDoseAccessor() ;

			};
		}
	}
}

#endif
