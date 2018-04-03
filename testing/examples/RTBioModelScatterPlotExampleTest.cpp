// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// Copyright (c) German Cancer Research Center (DKFZ),
// Software development for Integrated Diagnostics and Therapy (SIDT).
// ALL RIGHTS RESERVED.
// See rttbCopyright.txt or
// http://www.dkfz.de/en/sidt/projects/rttb/copyright.html [^]
//
// This software is distributed WITHOUT ANY WARRANTY; without even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE. See the above copyright notices for more information.
//
//------------------------------------------------------------------------
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author $Author$ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/shared_ptr.hpp>

#include "litCheckMacros.h"
#include "rttbBioModel.h"
#include "rttbDVH.h"
#include "rttbTCPLQModel.h"
#include "rttbNTCPLKBModel.h"
#include "rttbNTCPRSModel.h"
#include "rttbBioModelScatterPlots.h"
#include "rttbBioModelCurve.h"
#include "rttbDvhBasedModels.h"
#include "../models/rttbScatterTester.h"
#include "rttbInvalidParameterException.h"
#include "rttbDVHXmlFileReader.h"

namespace rttb
{
	namespace testing
	{


		/*! @brief RTBioModelScatterPlotExampleTest.
		calculating Curves and Scatterplots for TCP and NTCP models.
		The values on curve and scatterplot need to be similar for similar dose values.
		The range of difference is given by the variance used to generate the scatter.

		WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/
		int RTBioModelScatterPlotExampleTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			typedef rttb::models::CurveDataType CurveDataType;
			typedef rttb::models::ScatterPlotType ScatterPlotType;
			typedef core::DVH::DVHPointer DVHPointer;

			//increased accuracy requires double values in the calculation (rttbBaseType.h)
			double toleranceEUD = 1e-5;

			//ARGUMENTS: 1: ptv dvh file name
			//           2: normal tissue 1 dvh file name
			//           3: TV dvh file name

			std::string DVH_FILENAME_PTV;
			std::string DVH_FILENAME_NT1;
			std::string DVH_FILENAME_TV_TEST;

			if (argc > 1)
			{
				DVH_FILENAME_PTV = argv[1];
			}

			if (argc > 2)
			{
				DVH_FILENAME_NT1 = argv[2];
			}

			if (argc > 3)
			{
				DVH_FILENAME_TV_TEST = argv[3];
			}

			//DVH PTV
			rttb::io::other::DVHXMLFileReader dvhReader = rttb::io::other::DVHXMLFileReader(DVH_FILENAME_PTV);
			DVHPointer dvhPtr = dvhReader.generateDVH();

			rttb::io::other::DVHXMLFileReader dvhReader_test_tv = rttb::io::other::DVHXMLFileReader(
			            DVH_FILENAME_TV_TEST);
			DVHPointer dvh_test_tv = dvhReader_test_tv.generateDVH();

			//test TCP LQ Model
			models::BioModelParamType alpha = 0.35;
			models::BioModelParamType beta = 0.023333333333333;
			models::BioModelParamType roh = 10000000;
			int numFractions = 2;

			DoseTypeGy normalizationDose = 68;

			rttb::models::TCPLQModel tcplq = rttb::models::TCPLQModel(dvhPtr, alpha, beta, roh, numFractions);

			CHECK_NO_THROW(tcplq.init());

			CurveDataType curve = models::getCurveDoseVSBioModel(tcplq, normalizationDose);

			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(tcplq, 0, alpha, 0, normalizationDose, 100));
			ScatterPlotType tcpScatter = models::getScatterPlotVary1Parameter(tcplq, 0, alpha, 0,
			                             normalizationDose, 100);
			CHECK_EQUAL(100, tcpScatter.size());

			ScatterTester scatterCompare(curve, tcpScatter);
			CHECK_TESTER(scatterCompare);

			//test also with other parameter
			tcpScatter = models::getScatterPlotVary1Parameter(tcplq, 3, roh, 0, normalizationDose, 100);

			scatterCompare.setCompareScatter(tcpScatter);
			CHECK_TESTER(scatterCompare);

			std::vector<int> paramIdVec;
			models::BioModel::ParamVectorType meanVec;
			models::BioModel::ParamVectorType meanVecTest;
			meanVecTest.push_back(alpha);
			models::BioModel::ParamVectorType varianceVec;
			//"alphaMean":0,"alphaVariance":1,"alpha_beta":2, "rho":3
			paramIdVec.push_back(0);
			meanVec.push_back(tcplq.getAlphaMean());
			varianceVec.push_back(0);
			//setting parameter 1 will change the resulting scatter plot dramatically - is it meant to?
			//this is unexpected since the value was taken from the original calculation
			//paramIdVec.push_back(1); meanVec.push_back(tcplq.getAlphaVariance()); varianceVec.push_back(0);
			paramIdVec.push_back(2);
			meanVec.push_back(tcplq.getAlphaBeta());
			varianceVec.push_back(0);
			paramIdVec.push_back(3);
			meanVec.push_back(tcplq.getRho());
			varianceVec.push_back(0);

			CHECK_THROW_EXPLICIT(models::getScatterPlotVaryParameters(tcplq, paramIdVec, meanVecTest,
			                     varianceVec, normalizationDose, 50), core::InvalidParameterException);
			ScatterPlotType scatterVary = models::getScatterPlotVaryParameters(tcplq, paramIdVec, meanVec,
			                              varianceVec, normalizationDose, 50);
			CHECK_EQUAL(50, scatterVary.size());

			scatterCompare.setCompareScatter(scatterVary);
			CHECK_TESTER(scatterCompare);

			models::BioModelParamType variance = 0.00015;
			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(tcplq, 0, alpha, variance, normalizationDose,
			               100));
			tcpScatter = models::getScatterPlotVary1Parameter(tcplq, 0, alpha, variance, normalizationDose,
			             100);

			scatterCompare.setVariance(variance);
			scatterCompare.setCompareScatter(tcpScatter);
			//allow 5% of the points to deviate more
			scatterCompare.setAllowExceptions(true);
			CHECK_TESTER(scatterCompare);

			//test also with other parameter
			tcpScatter = models::getScatterPlotVary1Parameter(tcplq, 3, roh, variance, normalizationDose, 100);
			scatterCompare.setCompareScatter(tcpScatter);
			//allow 5% of the points to deviate more
			scatterCompare.setAllowExceptions(true);
			CHECK_TESTER(scatterCompare);

			paramIdVec.clear();
			meanVec.clear();
			varianceVec.clear();
			paramIdVec.push_back(0);
			meanVec.push_back(tcplq.getAlphaMean());
			varianceVec.push_back(variance);
			//paramIdVec.push_back(1); meanVec.push_back(tcplq.getAlphaVariance()); varianceVec.push_back(variance);
			paramIdVec.push_back(2);
			meanVec.push_back(tcplq.getAlphaBeta());
			varianceVec.push_back(variance);
			paramIdVec.push_back(3);
			meanVec.push_back(tcplq.getRho());
			varianceVec.push_back(variance);

			scatterVary = models::getScatterPlotVaryParameters(tcplq, paramIdVec, meanVec, varianceVec,
			              normalizationDose, 50);

			scatterCompare.setCompareScatter(scatterVary);
			//allow 5% of the points to deviate more
			scatterCompare.setAllowExceptions(true);
			CHECK_TESTER(scatterCompare);

			models::BioModelParamType alphaBeta = 10;
			tcplq.setParameters(alpha, alphaBeta, roh, 0.08);
			tcplq.init();

			normalizationDose = 40;
			curve = models::getCurveDoseVSBioModel(tcplq, normalizationDose);

			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(tcplq, 0, alpha, 0, normalizationDose, 100));
			tcpScatter = models::getScatterPlotVary1Parameter(tcplq, 0, alpha, 0, normalizationDose, 100);

			scatterCompare.setReferenceCurve(curve);
			scatterCompare.setVariance(0);
			//do not allow larger deviations
			scatterCompare.setAllowExceptions(false);
			scatterCompare.setCompareScatter(tcpScatter);
			CHECK_TESTER(scatterCompare);

			variance = 0.25;
			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(tcplq, 0, alpha, variance, normalizationDose,
			               100));
			tcpScatter = models::getScatterPlotVary1Parameter(tcplq, 0, alpha, variance, normalizationDose,
			             100);

			scatterCompare.setCompareScatter(tcpScatter);
			scatterCompare.setVariance(variance);
			//allow 5% of the points to deviate more
			scatterCompare.setAllowExceptions(true);
			CHECK_TESTER(scatterCompare);


			/*TCP LQ Test*/
			alpha = 0.3;
			beta = 0.03;
			roh = 10000000;
			numFractions = 20;
			rttb::models::TCPLQModel tcplq_test = rttb::models::TCPLQModel(dvh_test_tv, alpha, beta, roh,
			                                      numFractions);

			CHECK_NO_THROW(tcplq_test.init());
			normalizationDose = 60;
			curve = models::getCurveDoseVSBioModel(tcplq_test, normalizationDose);

			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(tcplq_test, 0, alpha, 0, normalizationDose,
			               100));
			tcpScatter = models::getScatterPlotVary1Parameter(tcplq_test, 0, alpha, 0, normalizationDose, 100);

			scatterCompare.setReferenceCurve(curve);
			scatterCompare.setVariance(0);
			scatterCompare.setCompareScatter(tcpScatter);
			CHECK_TESTER(scatterCompare);

			//test also with other parameter
			tcpScatter = models::getScatterPlotVary1Parameter(tcplq_test, 3, roh, 0, normalizationDose, 100);
			scatterCompare.setCompareScatter(tcpScatter);
			CHECK_TESTER(scatterCompare);

			paramIdVec.clear();
			meanVec.clear();
			varianceVec.clear();
			paramIdVec.push_back(0);
			meanVec.push_back(tcplq_test.getAlphaMean());
			varianceVec.push_back(0);
			//paramIdVec.push_back(1); meanVec.push_back(tcplq_test.getAlphaVariance()); varianceVec.push_back(0);
			paramIdVec.push_back(2);
			meanVec.push_back(tcplq_test.getAlphaBeta());
			varianceVec.push_back(0);
			paramIdVec.push_back(3);
			meanVec.push_back(tcplq_test.getRho());
			varianceVec.push_back(0);

			scatterVary = models::getScatterPlotVaryParameters(tcplq_test, paramIdVec, meanVec, varianceVec,
			              normalizationDose, 50);

			scatterCompare.setCompareScatter(scatterVary);
			CHECK_TESTER(scatterCompare);

			variance = 0.00025;
			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(tcplq_test, 0, alpha, variance,
			               normalizationDose, 100));
			tcpScatter = models::getScatterPlotVary1Parameter(tcplq_test, 0, alpha, variance, normalizationDose,
			             100);

			scatterCompare.setCompareScatter(tcpScatter);
			scatterCompare.setVariance(variance);
			CHECK_TESTER(scatterCompare);

			//test also with other parameter
			tcpScatter = models::getScatterPlotVary1Parameter(tcplq_test, 3, roh, variance, normalizationDose,
			             100);
			scatterCompare.setCompareScatter(tcpScatter);
			scatterCompare.setAllowExceptions(true);
			CHECK_TESTER(scatterCompare);
			scatterCompare.setAllowExceptions(false);

			paramIdVec.clear();
			meanVec.clear();
			varianceVec.clear();
			paramIdVec.push_back(0);
			meanVec.push_back(tcplq_test.getAlphaMean());
			varianceVec.push_back(variance);
			//paramIdVec.push_back(1); meanVec.push_back(tcplq_test.getAlphaVariance()); varianceVec.push_back(variance);
			paramIdVec.push_back(2);
			meanVec.push_back(tcplq_test.getAlphaBeta());
			varianceVec.push_back(variance);
			paramIdVec.push_back(3);
			meanVec.push_back(tcplq_test.getRho());
			varianceVec.push_back(variance);

			scatterVary = models::getScatterPlotVaryParameters(tcplq_test, paramIdVec, meanVec, varianceVec,
			              normalizationDose, 50);

			scatterCompare.setCompareScatter(scatterVary);
			//allow 5% of the points to deviate more
			scatterCompare.setAllowExceptions(true);
			CHECK_TESTER(scatterCompare);

			//DVH HT 1
			rttb::io::other::DVHXMLFileReader dvhReader2 = rttb::io::other::DVHXMLFileReader(DVH_FILENAME_NT1);
			DVHPointer dvhPtr2 = dvhReader2.generateDVH();

			CHECK_CLOSE(1.07920836034015810000e+001, models::getEUD(dvhPtr2, 10), toleranceEUD);

			//test RTNTCPLKBModel
			rttb::models::NTCPLKBModel lkb = rttb::models::NTCPLKBModel();
			models::BioModelParamType aVal = 10;
			models::BioModelParamType mVal = 0.16;
			models::BioModelParamType d50Val = 55;
			lkb.setDVH(dvhPtr2);
			lkb.setA(aVal);
			lkb.setM(mVal);
			lkb.setD50(d50Val);
			CHECK_NO_THROW(lkb.init());

			normalizationDose = 60;
			curve = models::getCurveDoseVSBioModel(lkb, normalizationDose);

			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(lkb, 2, aVal, 0, normalizationDose, 100));
			ScatterPlotType scatter = models::getScatterPlotVary1Parameter(lkb, 2, aVal, 0, normalizationDose,
			                          100);

			scatterCompare.setReferenceCurve(curve);
			scatterCompare.setVariance(0);
			scatterCompare.setCompareScatter(scatter);
			CHECK_TESTER(scatterCompare);

			//"d50":0,"m":1,"a":2
			//test also with other parameter
			scatter = models::getScatterPlotVary1Parameter(lkb, 0, d50Val, 0, normalizationDose, 100);
			scatterCompare.setCompareScatter(scatter);
			CHECK_TESTER(scatterCompare);

			paramIdVec.clear();
			meanVec.clear();
			varianceVec.clear();
			paramIdVec.push_back(0);
			meanVec.push_back(lkb.getD50());
			varianceVec.push_back(0);
			paramIdVec.push_back(1);
			meanVec.push_back(lkb.getM());
			varianceVec.push_back(0);
			paramIdVec.push_back(2);
			meanVec.push_back(lkb.getA());
			varianceVec.push_back(0);

			scatterVary = models::getScatterPlotVaryParameters(lkb, paramIdVec, meanVec, varianceVec,
			              normalizationDose, 50);

			scatterCompare.setCompareScatter(scatterVary);
			CHECK_TESTER(scatterCompare);

			variance = 0.00025;
			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(lkb, 2, aVal, variance, normalizationDose,
			               100));
			scatter = models::getScatterPlotVary1Parameter(lkb, 2, aVal, variance, normalizationDose, 100);

			scatterCompare.setCompareScatter(scatter);
			scatterCompare.setVariance(variance);
			CHECK_TESTER(scatterCompare);

			//test also with other parameter
			scatter = models::getScatterPlotVary1Parameter(lkb, 0, d50Val, variance, normalizationDose, 100);
			scatterCompare.setCompareScatter(scatter);
			CHECK_TESTER(scatterCompare);

			paramIdVec.clear();
			meanVec.clear();
			varianceVec.clear();
			paramIdVec.push_back(0);
			meanVec.push_back(lkb.getD50());
			varianceVec.push_back(variance);
			paramIdVec.push_back(1);
			meanVec.push_back(lkb.getM());
			varianceVec.push_back(variance);
			paramIdVec.push_back(2);
			meanVec.push_back(lkb.getA());
			varianceVec.push_back(variance);

			scatterVary = models::getScatterPlotVaryParameters(lkb, paramIdVec, meanVec, varianceVec,
			              normalizationDose, 50);

			scatterCompare.setCompareScatter(scatterVary);
			CHECK_TESTER(scatterCompare);

			//test RTNTCPRSModel
			rttb::models::NTCPRSModel rs = rttb::models::NTCPRSModel();
			models::BioModelParamType gammaVal = 1.7;
			models::BioModelParamType sVal = 1;
			rs.setDVH(dvhPtr2);
			rs.setD50(d50Val);
			rs.setGamma(gammaVal);
			rs.setS(sVal);
			CHECK_NO_THROW(rs.init());

			normalizationDose = 60;
			curve = models::getCurveDoseVSBioModel(rs, normalizationDose);

			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(rs, 0, d50Val, 0, normalizationDose, 100));
			scatter = models::getScatterPlotVary1Parameter(rs, 0, d50Val, 0, normalizationDose, 100);

			scatterCompare.setReferenceCurve(curve);
			scatterCompare.setVariance(0);
			scatterCompare.setCompareScatter(scatter);
			CHECK_TESTER(scatterCompare);

			//"d50":0,"gamma":1,"s":2
			//test also with other parameter
			scatter = models::getScatterPlotVary1Parameter(rs, 1, gammaVal, 0, normalizationDose, 100);
			scatterCompare.setCompareScatter(scatter);
			CHECK_TESTER(scatterCompare);

			paramIdVec.clear();
			meanVec.clear();
			varianceVec.clear();
			paramIdVec.push_back(0);
			meanVec.push_back(rs.getD50());
			varianceVec.push_back(0);
			paramIdVec.push_back(1);
			meanVec.push_back(rs.getGamma());
			varianceVec.push_back(0);
			paramIdVec.push_back(2);
			meanVec.push_back(rs.getS());
			varianceVec.push_back(0);

			scatterVary = models::getScatterPlotVaryParameters(rs, paramIdVec, meanVec, varianceVec,
			              normalizationDose, 50);

			scatterCompare.setCompareScatter(scatterVary);
			CHECK_TESTER(scatterCompare);

			variance = 0.0075;
			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(rs, 0, d50Val, variance, normalizationDose,
			               100));
			scatter = models::getScatterPlotVary1Parameter(rs, 0, d50Val, variance, normalizationDose, 100);

			scatterCompare.setCompareScatter(scatter);
			scatterCompare.setVariance(variance);
			CHECK_TESTER(scatterCompare);

			//test also with other parameter
			scatter = models::getScatterPlotVary1Parameter(rs, 2, sVal, variance, normalizationDose, 100);
			scatterCompare.setCompareScatter(scatter);
			CHECK_TESTER(scatterCompare);

			paramIdVec.clear();
			meanVec.clear();
			varianceVec.clear();
			paramIdVec.push_back(0);
			meanVec.push_back(rs.getD50());
			varianceVec.push_back(variance);
			paramIdVec.push_back(1);
			meanVec.push_back(rs.getGamma());
			varianceVec.push_back(variance);
			paramIdVec.push_back(2);
			meanVec.push_back(rs.getS());
			varianceVec.push_back(variance);

			scatterVary = models::getScatterPlotVaryParameters(rs, paramIdVec, meanVec, varianceVec,
			              normalizationDose, 50);

			scatterCompare.setCompareScatter(scatterVary);
			CHECK_TESTER(scatterCompare);

			RETURN_AND_REPORT_TEST_SUCCESS;

		}

	}//testing
}//rttb
