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
#ifndef __VIRTUOS_CUBEINFO_DOSE_ACCESSOR_GENERATOR_H
#define __VIRTUOS_CUBEINFO_DOSE_ACCESSOR_GENERATOR_H

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

			/*! @class VirtuosCubeinfoDoseAccessorGenerator
			@brief Generate DoseAccessor with Virtuos Cubeinfo
			*/
			class VirtuosCubeinfoDoseAccessorGenerator: public core::DoseAccessorGeneratorBase
			{
			public:
				typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;

			private:
				Cubeinfo** _pPointerOnVirtuosCube;
				DoseTypeGy _normalizationDose;
				DoseTypeGy _prescribedDose;


				VirtuosCubeinfoDoseAccessorGenerator();

			protected:



			public:
				~VirtuosCubeinfoDoseAccessorGenerator();

				/*! @brief Constructor. Initialisation with a Cubeinfo*.
				@param normalizationDose is defined as (prescribedDose*1000)/maxDoseInGy. Default is 1 Gy.
				@param prescribedDose the does that was planned in the reference point in Gy. Default is 1 Gy.
				*/
				VirtuosCubeinfoDoseAccessorGenerator(Cubeinfo* aPointerOnVirtuosCube,
				                                     DoseTypeGy normalizationDose = 1,
				                                     DoseTypeGy prescribedDose = 1);
				/*! @brief Generate DoseAccessor
				@return Return shared pointer of DoseAccessor.
				*/
				DoseAccessorPointer generateDoseAccessor() ;

			};
		}
	}
}

#endif
