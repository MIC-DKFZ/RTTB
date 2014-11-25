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

#include <deque>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbDVH.h"
#include "rttbDVHSet.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"

#include "DummyDVHGenerator.h"

namespace rttb{
	namespace testing{

		typedef core::DVHSet::DVHSetType DVHSetType;

		/*! @brief DVHTest - test the API of DVH
			1) test constructors (values as expected?)
			2) test size
			3) test set/getIDs
			4) test insert/retrieve individual DVHs
			5) test getDVH<Type>Set
			6) test get<Type>Volume
		*/
		int DVHSetTest(int argc, char* argv[] )
			{
			PREPARE_DEFAULT_TEST_REPORTING;

			const IDType structureSetID = "myStructureSet";
			const IDType structureIDPrefix = "myStructure";
			const IDType doseID = "myDose";
			const IDType voxelizationID = "myVoxelization";

			DummyDVHGenerator dvhGenerator;	  
			DVHSetType tvSet;
			IDType structureID = structureIDPrefix+"_TV_";
			for(int i = 0; i < 3; i++){
				tvSet.push_back(dvhGenerator.generateDVH(structureID+ boost::lexical_cast<std::string>(i),doseID));
				}
			DVHSetType htSet;
			structureID = structureIDPrefix+"_HT_";
			for(int i = 0; i < 5; i++){
				htSet.push_back(dvhGenerator.generateDVH(structureID+ boost::lexical_cast<std::string>(i),doseID));
				}
			DVHSetType wvSet;
			structureID = structureIDPrefix+"_WV_";
			for(int i = 0; i < 1; i++){
				wvSet.push_back(dvhGenerator.generateDVH(structureID+ boost::lexical_cast<std::string>(i),doseID));
				}

			//1) test constructors (values as expected?)
			CHECK_NO_THROW(core::DVHSet(structureSetID, doseID));
			CHECK_NO_THROW(core::DVHSet(tvSet, htSet, structureSetID, doseID));
			CHECK_NO_THROW(core::DVHSet(tvSet, htSet, wvSet, structureSetID, doseID));

			//2) test size
			core::DVHSet myDvhSet1(structureSetID, doseID);
			CHECK_EQUAL(myDvhSet1.size(),0);
			core::DVHSet myDvhSet2(tvSet, htSet, structureSetID, doseID);
			CHECK_EQUAL(myDvhSet2.size(),tvSet.size()+htSet.size());
			core::DVHSet myDvhSet3(tvSet, htSet, wvSet, structureSetID, doseID);
			CHECK_EQUAL(myDvhSet3.size(),tvSet.size()+htSet.size()+wvSet.size());	  

			//3) test set/getIDs
			const IDType newStructureSetID = "myNewStructureSet";
			const IDType newDoseID = "myNewDose";
			CHECK_EQUAL(myDvhSet1.getStrSetID(),structureSetID);
			CHECK_EQUAL(myDvhSet1.getDoseID(),doseID);
			CHECK_NO_THROW(myDvhSet1.setStrSetID(newStructureSetID));
			CHECK_NO_THROW(myDvhSet1.setDoseID(newDoseID));
			CHECK_EQUAL(myDvhSet1.getStrSetID(),newStructureSetID);
			CHECK_EQUAL(myDvhSet1.getDoseID(),newDoseID);

			CHECK_EQUAL(myDvhSet3.getStrSetID(),structureSetID);
			CHECK_EQUAL(myDvhSet3.getDoseID(),doseID);
			CHECK_NO_THROW(myDvhSet3.setStrSetID(newStructureSetID));
			CHECK_NO_THROW(myDvhSet3.setDoseID(newDoseID));
			CHECK_EQUAL(myDvhSet3.getStrSetID(),newStructureSetID);
			CHECK_EQUAL(myDvhSet3.getDoseID(),newDoseID);	  

			//4) test insert/retrieve individual DVHs
			DVHRole roleTV = {DVHRole::TargetVolume};
			structureID = structureIDPrefix+"_TV_";
			core::DVH tv = dvhGenerator.generateDVH(structureID+ boost::lexical_cast<std::string>(tvSet.size()),doseID);
			CHECK_EQUAL(myDvhSet1.size(),0);
			CHECK_NO_THROW(myDvhSet1.insert(tv, roleTV));
			CHECK_EQUAL(myDvhSet1.size(),1);
			std::size_t currentSize = myDvhSet2.size();
			CHECK_NO_THROW(myDvhSet2.insert(tv, roleTV));
			CHECK_EQUAL(myDvhSet2.size(),currentSize+1);

			DVHRole roleHT = {DVHRole::HealthyTissue};
			structureID = structureIDPrefix+"_HT_";
			core::DVH ht = dvhGenerator.generateDVH(structureID+ boost::lexical_cast<std::string>(htSet.size()),doseID);
			CHECK_EQUAL(myDvhSet1.size(),1);
			CHECK_NO_THROW(myDvhSet1.insert(ht, roleHT));
			CHECK_EQUAL(myDvhSet1.size(),2);
			currentSize = myDvhSet2.size();
			CHECK_NO_THROW(myDvhSet2.insert(ht, roleHT));
			CHECK_EQUAL(myDvhSet2.size(),currentSize+1);

			DVHRole roleWV = {DVHRole::WholeVolume};
			structureID = structureIDPrefix+"_wv_";
			IDType testID = structureID+ boost::lexical_cast<std::string>(wvSet.size());
			core::DVH wv = dvhGenerator.generateDVH(structureID+ boost::lexical_cast<std::string>(wvSet.size()),doseID);
			CHECK_EQUAL(myDvhSet1.size(),2);
			CHECK_NO_THROW(myDvhSet1.insert(wv, roleWV));
			CHECK_EQUAL(myDvhSet1.size(),3);
			currentSize = myDvhSet2.size();
			CHECK_NO_THROW(myDvhSet2.insert(wv, roleWV));
			CHECK_EQUAL(myDvhSet2.size(),currentSize+1);

			//5) test getDVH<Type>Set
			core::DVH* dvhPtr = myDvhSet1.getDVH(testID);
			CHECK_EQUAL(*dvhPtr,wv);

			dvhPtr = myDvhSet3.getDVH(structureIDPrefix+"_TV_0");
			CHECK_EQUAL(*dvhPtr,tvSet.at(0));

			dvhPtr = myDvhSet3.getDVH(structureIDPrefix+"_TV_2");
			CHECK_EQUAL(*dvhPtr,tvSet.at(2));

			DVHSetType tvTest =  myDvhSet3.getTargetVolumeSet();
			CHECK_EQUAL(tvTest,tvSet);

			DVHSetType htTest =  myDvhSet3.getHealthyTissueSet();
			CHECK_EQUAL(htTest,htSet);

			DVHSetType wvTest =  myDvhSet3.getWholeVolumeSet();	  
			CHECK_EQUAL(wvTest,wvSet);

			//6) test get<Type>Volume
			DoseTypeGy aDoseAbsolute = 10;
			CHECK_EQUAL(0,myDvhSet3.getHealthyTissueVolume(aDoseAbsolute));
			CHECK_EQUAL(0,myDvhSet3.getTargetVolume(aDoseAbsolute));
			CHECK_EQUAL(0,myDvhSet3.getWholeVolume(aDoseAbsolute));

			RETURN_AND_REPORT_TEST_SUCCESS;
			}

		}//end namespace testing
	}//end namespace rttb
