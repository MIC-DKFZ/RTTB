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
#include "rttbStructure.h"
#include "rttbInvalidParameterException.h"
#include "DummyStructure.h"
#include "DummyDoseAccessor.h"

namespace rttb
{
	namespace testing
	{

		/*! @brief StructureTest - tests the API for Structure
			1) constructors
			2) get/setXX
		*/
		int StructureTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			boost::shared_ptr<DummyDoseAccessor> spTestDoseAccessor =
			    boost::make_shared<DummyDoseAccessor>();

			DummyStructure myStructGenerator(spTestDoseAccessor->getGeometricInfo());

			//1) constructors
			CHECK_NO_THROW(core::Structure());

			core::Structure emptyTestStruct;
			CHECK_EQUAL("None", emptyTestStruct.getLabel());
			CHECK_NO_THROW(emptyTestStruct.getUID());

			GridIndexType zPlane = 4;
			core::Structure rect = myStructGenerator.CreateRectangularStructureCentered(zPlane);

			CHECK_NO_THROW(core::Structure(rect.getStructureVector()));
			core::Structure rect2 = core::Structure(rect.getStructureVector());
			CHECK_EQUAL(rect.getLabel(), rect2.getLabel());
			CHECK(rect.getUID() != rect2.getUID());

			PolygonSequenceType rectVec = rect.getStructureVector();
			PolygonSequenceType rect2Vec = rect2.getStructureVector();
			CHECK_EQUAL(rectVec.size(), rect2Vec.size());

			PolygonSequenceType::iterator it = rectVec.begin();
			PolygonSequenceType::iterator it2 = rect2Vec.begin();

			for (; it != rectVec.end(); ++it)
			{
				CHECK_EQUAL(it->size(), it2->size());
				PolygonType::iterator pit = it->begin();
				PolygonType::iterator pit2 = it2->begin();

				for (; pit != it->end(); ++pit)
				{
					CHECK_EQUAL(*(pit), *(pit2));
					++pit2;
				}

				++it2;
			}

			CHECK_NO_THROW(core::Structure rect3 = rect);
			core::Structure rect3 = rect;
			CHECK_EQUAL(rect.getLabel(), rect3.getLabel());
			CHECK_EQUAL(rect.getUID(), rect3.getUID());

			PolygonSequenceType rect3Vec = rect3.getStructureVector();
			CHECK_EQUAL(rectVec.size(), rect3Vec.size());

			it = rectVec.begin();
			PolygonSequenceType::iterator it3 = rect3Vec.begin();

			for (; it != rectVec.end(); ++it)
			{
				CHECK_EQUAL(it->size(), it3->size());
				PolygonType::iterator pit = it->begin();
				PolygonType::iterator pit3 = it3->begin();

				for (; pit != it->end(); ++pit)
				{
					CHECK_EQUAL(*(pit), *(pit3));
					++pit3;
				}

				++it3;
			}

			//2) get/setXX
			CHECK_EQUAL("None", emptyTestStruct.getLabel());
			CHECK_NO_THROW(emptyTestStruct.setLabel("NEW Label"));
			CHECK_EQUAL("NEW Label", emptyTestStruct.getLabel());
			CHECK_NO_THROW(emptyTestStruct.getUID());
			CHECK_NO_THROW(emptyTestStruct.setUID("1.2.345.67.8.9"));
			CHECK_EQUAL("1.2.345.67.8.9", emptyTestStruct.getUID());

			CHECK((emptyTestStruct.getStructureVector()).empty());
			CHECK_EQUAL(0, emptyTestStruct.getNumberOfEndpoints());

			CHECK_EQUAL(4, rect.getNumberOfEndpoints());
			CHECK_EQUAL(rect.getNumberOfEndpoints(), rect2.getNumberOfEndpoints());

			core::Structure circ = myStructGenerator.CreateTestStructureCircle(zPlane);
			CHECK_EQUAL(4004, circ.getNumberOfEndpoints());

			core::Structure multiPoly =
			    myStructGenerator.CreateTestStructureIntersectingTwoPolygonsInDifferentSlices(zPlane, zPlane + 1);
			CHECK_EQUAL(8, multiPoly.getNumberOfEndpoints());

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb