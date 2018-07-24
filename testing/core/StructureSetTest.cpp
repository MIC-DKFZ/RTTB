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

#include <boost/make_shared.hpp>

#include "litCheckMacros.h"

#include "DummyStructure.h"
#include "DummyDoseAccessor.h"
#include "rttbInvalidParameterException.h"
#include "rttbStructure.h"
#include "rttbStructureSet.h"

namespace rttb
{
	namespace testing
	{

		/*! @brief StructureSet - tests the API for StructureSet
		1) constructor
    2) getters
		*/
		int StructureSetTest(int argc, char* argv[])
		{
			typedef core::Structure::Pointer StructTypePointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor =
			    boost::make_shared<DummyDoseAccessor>();

			DummyStructure myStructGenerator(spTestDoseAccessor->getGeometricInfo());
			GridIndexType zPlane = 4;
			core::Structure rect = myStructGenerator.CreateRectangularStructureCentered(zPlane);
			StructTypePointer rectStrPtr = boost::make_shared<core::Structure>(rect);
      rectStrPtr->setLabel("test");
      StructTypePointer rectStrPtr2 = boost::make_shared<core::Structure>(rect);
      rectStrPtr2->setLabel("test2");
      const IDType patientUID("patientUID");
      const IDType UID("UID");

      //1) constructor
      CHECK_NO_THROW(core::StructureSet({  }));
      core::StructureSet structureSetDefault({  });
      CHECK_NO_THROW(core::StructureSet({ rectStrPtr, rectStrPtr2 }, patientUID, UID));
      core::StructureSet structureSet({rectStrPtr, rectStrPtr2}, patientUID, UID);

      //2) getters
      CHECK_EQUAL(structureSetDefault.getNumberOfStructures(), 0);
      CHECK_EQUAL(structureSet.getNumberOfStructures(), 2);
      CHECK_EQUAL(structureSet.getStructure(0)->getLabel(), "test");
      CHECK_EQUAL(structureSet.getStructure(1)->getLabel(), "test2");
      CHECK_THROW_EXPLICIT(structureSet.getStructure(2), core::InvalidParameterException);

      CHECK_EQUAL(structureSet.getUID(), UID);
      CHECK_EQUAL(structureSet.getPatientUID(), patientUID);
      CHECK_NO_THROW(structureSetDefault.getUID());
      CHECK_NO_THROW(structureSetDefault.getPatientUID());

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb