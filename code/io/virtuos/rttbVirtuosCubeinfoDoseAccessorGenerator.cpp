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
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "rttbVirtuosCubeinfoDoseAccessorGenerator.h"
#include "rttbVirtuosDoseAccessor.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"
#include "rttbIndexOutOfBoundsException.h"


//VIRTUOS
#include "pln1file.h"
#include "plt_type.h"
#include "rtp_type.h"


namespace rttb{
	namespace io{
		namespace virtuos{


			VirtuosCubeinfoDoseAccessorGenerator::~VirtuosCubeinfoDoseAccessorGenerator(){}


			VirtuosCubeinfoDoseAccessorGenerator::VirtuosCubeinfoDoseAccessorGenerator(Cubeinfo *aPointerOnVirtuosCube, DoseTypeGy normalizationDose, DoseTypeGy prescribedDose):
			_pPointerOnVirtuosCube(new Cubeinfo*)
			{
				//initialize cube pointer
				*_pPointerOnVirtuosCube = create_cubeinfo(0);
				*_pPointerOnVirtuosCube=aPointerOnVirtuosCube;
				_normalizationDose=normalizationDose;
				_prescribedDose=prescribedDose;

			}

			VirtuosCubeinfoDoseAccessorGenerator::DoseAccessorPointer VirtuosCubeinfoDoseAccessorGenerator::generateDoseAccessor() {
				_doseAccessor=boost::make_shared<io::virtuos::VirtuosDoseAccessor>(*_pPointerOnVirtuosCube, false,_normalizationDose,_prescribedDose);
				return _doseAccessor;
			}



		}
	}
}

