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
#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbBaseTypeModels.h"
#include "rttbBioModel.h"
#include "rttbBioModelScatterPlots.h"
#include "DummyDVHGenerator.h"
#include "DummyModel.h"
#include "rttbIntegration.h"

#include <istream>
#include <sstream>
#include <fstream>

namespace rttb
{
	namespace testing
	{

		typedef models::ScatterPlotType ScatterPlotType;

		/*! @brief BioModelScatterPlotTest. Test the scatterplot methods.
		1) test if parameters are set correctly
		2) test if scatterData contain each model value exactly once
		*/
		int BioModelScatterPlotTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			//generate artificial DVH and corresponding statistical values
			DummyDVHGenerator dummyDVH;

			const IDType structureID = "myStructure";
			const IDType doseID = "myDose";

			core::DVH::Pointer dvhPtr = boost::make_shared<core::DVH>(dummyDVH.generateDVH(structureID,
			                               doseID, 0, 2000));

			//test Dummy Model
			models::BioModelParamType param1 = 0.35;
			models::BioModelParamType param2 = 0.023333333333333;
			models::BioModelParamType param3 = 10000000;

			//model values will be dose/normalisationDose
			DoseTypeGy normalisationDose = 10;

			rttb::models::DummyModel model(dvhPtr);
			model.resetCounters();

			//default number of points is 100
			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(model, 0, param1, 0.5, normalisationDose));
			model.resetCounters();
			ScatterPlotType scatter = models::getScatterPlotVary1Parameter(model, 0, param1, 0.5,
			                          normalisationDose);
			//only 1st parameter should gave been changed
			CHECK_EQUAL(100, model.getSetParam1Count());
			CHECK_EQUAL(0, model.getSetParam2Count());
			CHECK_EQUAL(0, model.getSetParam3Count());
			CHECK_EQUAL(100, model.getCalcCount());

			// all model values should match the corresponding dose/normalizationDose
			ScatterPlotType::iterator iter;

			for (iter = scatter.begin(); iter != scatter.end(); ++iter)
			{
				double currentKey = iter->first;

				if ((currentKey / normalisationDose) != iter->second.first)
				{
					CHECK_EQUAL((currentKey / normalisationDose), iter->second.first);
				}
			}

			model.resetCounters();

			//number of points is 50
			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(model, 1, param2, 0.25, normalisationDose, 50));
			model.resetCounters();
			scatter = models::getScatterPlotVary1Parameter(model, 1, param2, 0.25, normalisationDose, 50);
			//only 1st parameter should gave been changed
			CHECK_EQUAL(0, model.getSetParam1Count());
			CHECK_EQUAL(50, model.getSetParam2Count());
			CHECK_EQUAL(0, model.getSetParam3Count());
			CHECK_EQUAL(50, model.getCalcCount());

			// all model values should match the corresponding dose/normalizationDose
			for (iter = scatter.begin(); iter != scatter.end(); ++iter)
			{
				double currentKey = iter->first;

				if ((currentKey / normalisationDose) != iter->second.first)
				{
					CHECK_EQUAL((currentKey / normalisationDose), iter->second.first);
				}
			}

			model.resetCounters();

			//number of points is 150
			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(model, 2, param3, 0.75, normalisationDose,
			               150));
			model.resetCounters();
			scatter = models::getScatterPlotVary1Parameter(model, 2, param3, 0.75, normalisationDose, 150);
			//only 1st parameter should gave been changed
			CHECK_EQUAL(0, model.getSetParam1Count());
			CHECK_EQUAL(0, model.getSetParam2Count());
			CHECK_EQUAL(150, model.getSetParam3Count());
			CHECK_EQUAL(150, model.getCalcCount());

			// all model values should match the corresponding dose/normalizationDose
			for (iter = scatter.begin(); iter != scatter.end(); ++iter)
			{
				double currentKey = iter->first;

				if ((currentKey / normalisationDose) != iter->second.first)
				{
					CHECK_EQUAL((currentKey / normalisationDose), iter->second.first);
				}
			}

			model.resetCounters();
			std::vector<int> paramIDVec;
			models::BioModel::ParamVectorType meanVec;
			models::BioModel::ParamVectorType varianceVec;

			paramIDVec.push_back(0);
			meanVec.push_back(1);
			varianceVec.push_back(0.6);
			paramIDVec.push_back(1);
			meanVec.push_back(10);
			varianceVec.push_back(0.5);
			paramIDVec.push_back(2);
			meanVec.push_back(100);
			varianceVec.push_back(0.4);

			//number of points is 50
			CHECK_NO_THROW(models::getScatterPlotVaryParameters(model, paramIDVec, meanVec, varianceVec,
			               normalisationDose, 50));
			model.resetCounters();
			scatter = models::getScatterPlotVaryParameters(model, paramIDVec, meanVec, varianceVec,
			          normalisationDose, 50);
			//only 1st parameter should gave been changed
			CHECK_EQUAL(50, model.getSetParam1Count());
			CHECK_EQUAL(50, model.getSetParam2Count());
			CHECK_EQUAL(50, model.getSetParam3Count());
			CHECK_EQUAL(50, model.getCalcCount());

			// all model values should match the corresponding dose/normalizationDose
			for (iter = scatter.begin(); iter != scatter.end(); ++iter)
			{
				double currentKey = iter->first;

				if ((currentKey / normalisationDose) != iter->second.first)
				{
					CHECK_EQUAL((currentKey / normalisationDose), iter->second.first);
				}
			}

			model.resetCounters();

			//test 2000 points
			CHECK_NO_THROW(models::getScatterPlotVaryParameters(model, paramIDVec, meanVec, varianceVec,
			               normalisationDose, 10000));

			//test iterativeIntegration
			/*std::string bedFileName="d:/Temp/bed.txt";
			std::ifstream dvh_ifstr(bedFileName,std::ios::in);
			std::vector<DoseCalcType> volumeV2;
			std::vector<DoseTypeGy> bedV2;

			//std::cout  << "iterative integration"<< std::endl;
			if ( !dvh_ifstr.is_open() )
			{
				std::cerr<< "DVH file name invalid: could not open the dvh file!"<<std::endl;
			}
			else {
				while(!dvh_ifstr.eof())
				{
					std::string line;
					std::getline(dvh_ifstr,line);
					std::size_t found1=line.find("(");
					std::size_t found2=line.find(",");
					std::size_t found3=line.find(");");
					if(found1!= std::string::npos && found2!=std::string::npos && found3!=std::string::npos){
						std::string lineSub1 = line.substr(found1+1,found2-found1-1);
						std::stringstream ss(lineSub1);
						double volume;
						ss >> volume;
						volumeV2.push_back(volume);
						//std::cout  << "("<< volume << ",";

						std::string lineSub2 = line.substr(found2+1,found3-found2-1);
						//std::cout << lineSub2 << std::endl;
						std::stringstream ss2(lineSub2);
						double bed;
						ss2 >> bed;
						bedV2.push_back(bed);
						//std::cout  << bed << ");";

					}

				}

			}

			struct rttb::models::TcpParams params={0.302101,0.08,10000000,volumeV2,bedV2};

			double result = rttb::models::integrateTCP(0, params);*/

			RETURN_AND_REPORT_TEST_SUCCESS;

		}

	}//testing
}//rttb
