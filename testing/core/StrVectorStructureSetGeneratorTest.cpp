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

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbStrVectorStructureSetGenerator.h"
#include "DummyStructure.h"
#include "DummyDoseAccessor.h"
#include "rttbInvalidParameterException.h"
#include "rttbStructure.h"

namespace rttb
{
	namespace testing
	{

		/*! @brief StrVectorStructureSetGeneratorTest - tests the API for Structure
		1) empty structure vector
		2) dummy structure
    3) with regex
		*/
		int StrVectorStructureSetGeneratorTest(int argc, char* argv[])
		{
			typedef core::StructureSet::StructTypePointer StructTypePointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			//1) empty structure vector
			std::vector<StructTypePointer> strVector;

			CHECK_NO_THROW(core::StrVectorStructureSetGenerator generator1(strVector));
			CHECK_NO_THROW(core::StrVectorStructureSetGenerator(strVector).generateStructureSet());
			CHECK_EQUAL(core::StrVectorStructureSetGenerator(
			                strVector).generateStructureSet()->getNumberOfStructures(), 0);
			CHECK_THROW_EXPLICIT(core::StrVectorStructureSetGenerator(
			                         strVector).generateStructureSet()->getStructure(0), core::InvalidParameterException);


			//2) dummy structure
			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor =
			    boost::make_shared<DummyDoseAccessor>();

			DummyStructure myStructGenerator(spTestDoseAccessor->getGeometricInfo());
			GridIndexType zPlane = 4;
			core::Structure rect = myStructGenerator.CreateRectangularStructureCentered(zPlane);
			StructTypePointer rectStrPtr = boost::make_shared<core::Structure>(rect);
      rectStrPtr->setLabel("test");

			strVector.push_back(rectStrPtr);

			CHECK_NO_THROW(core::StrVectorStructureSetGenerator generator2(strVector));
			CHECK_NO_THROW(core::StrVectorStructureSetGenerator(strVector).generateStructureSet());
			CHECK_EQUAL(core::StrVectorStructureSetGenerator(
			                strVector).generateStructureSet()->getNumberOfStructures(), 1);
			CHECK_NO_THROW(core::StrVectorStructureSetGenerator(strVector).generateStructureSet()->getStructure(
			                   0));

      //3) with regex
      StructTypePointer rectStrPtr2 = boost::make_shared<core::Structure>(rect);
      rectStrPtr2->setLabel("none");
      strVector.push_back(rectStrPtr2);

      core::StrVectorStructureSetGenerator generator(strVector);
      generator.setStructureLabelFilterActive(true);
      generator.setFilterRegEx("test");
      CHECK_NO_THROW(generator.generateStructureSet());
      CHECK_EQUAL(generator.generateStructureSet()->getNumberOfStructures(), 1);
      CHECK_EQUAL(generator.generateStructureSet()->getStructure(0)->getLabel(), "test");

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb