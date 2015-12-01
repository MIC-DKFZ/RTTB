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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>

#include "../../code/core/rttbBaseType_NEW.h"
#include "../../code/core/rttbClinicalTrial.h"
#include "../../code/core/rttbDoseAccessorInterface_NEW.h"
#include "../../code/core/rttbDVH.h"
#include "../../code/core/rttbDVHCalculator.h"
#include "../../code/core/rttbDVHGeneratorInterface.h"
#include "../../code/core/rttbDVHSet.h"
#include "../../code/core/rttbException.h"
#include "../../code/core/rttbField.h"
#include "../../code/core/rttbFraction.h"
#include "../../code/core/rttbFractionManager.h"
#include "../../code/core/rttbGenericDoseIterator_NEW.h"
#include "../../code/core/rttbGenericMaskedDoseIterator_NEW.h"
#include "../../code/core/rttbGeometricInfo_NEW.h"
#include "../../code/core/rttbImage.h"
#include "../../code/core/rttbIndexConversionInterface_NEW.h"
#include "../../code/core/rttbIndexOutOfBoundsException.h"
#include "../../code/core/rttbInvalidDoseException.h"
#include "../../code/core/rttbInvalidParameterException.h"
#include "../../code/core/rttbMaskAccessorInterface_NEW.h"
#include "../../code/core/rttbMaskedDoseIteratorInterface_NEW.h"
#include "../../code/core/rttbMaskVoxel_NEW.h"
#include "../../code/core/rttbNullPointerException.h"
#include "../../code/core/rttbPatient.h"
#include "../../code/core/rttbPhysicalInfo.h"
#include "../../code/core/rttbPlan.h"
#include "../../code/core/rttbSelfIntersectingStructureException.h"
#include "../../code/core/rttbStructure.h"
#include "../../code/core/rttbStructureSetInterface.h"
#include "../../code/core/rttbTherapy.h"
#include "../../code/core/rttbTreatmentCourse.h"


int main(int , char*)
{

	return EXIT_SUCCESS;
}

