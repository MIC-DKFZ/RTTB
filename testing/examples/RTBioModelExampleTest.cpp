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
#include "rttbDoseIteratorInterface.h"
#include "rttbDVHXMLFileReader.h"

namespace rttb
{
	namespace testing
	{


		/*! @brief RTBioModelTest.
		TCP calculated using a DVH PTV and LQ Model.
		NTCP tested using 3 Normal Tissue DVHs and LKB/RS Model.

		Test if calculation in new architecture returns similar results to the
		original implementation.

		WARNING: The values for comparison need to be adjusted if the input files are changed!
		*/
		int RTBioModelExampleTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			typedef rttb::models::CurveDataType CurveDataType;
			typedef std::multimap<double , std::pair<double, double> > ScatterPlotType;
			typedef core::DVH::Pointer DVHPointer;

			//increased accuracy requires double values in the calculation (rttbBaseType.h)
			double toleranceEUD = 1e-5;
			double tolerance = 1e-7;

			//ARGUMENTS: 1: ptv dvh file name
			//           2: normal tissue 1 dvh file name
			//           3: normal tissue 2 dvh file name
			//           4: normal tissue 3 dvh file name
			//...........5: Virtuos MPM_LR_ah dvh lung file name
			//...........6: Virtuos MPM_LR_ah dvh target file name

			std::string DVH_FILENAME_PTV;
			std::string DVH_FILENAME_NT1;
			std::string DVH_FILENAME_NT2;
			std::string DVH_FILENAME_NT3;
			std::string DVH_FILENAME_TV_TEST;
			std::string DVH_Virtuos_Target;
			std::string DVH_Virtuos_Lung;

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
				DVH_FILENAME_NT2 = argv[3];
			}

			if (argc > 4)
			{
				DVH_FILENAME_NT3 = argv[4];
			}

			if (argc > 5)
			{
				DVH_FILENAME_TV_TEST = argv[5];
			}

			if (argc > 6)
			{
				DVH_Virtuos_Lung = argv[6];
			}

			if (argc > 7)
			{
				DVH_Virtuos_Target = argv[7];
			}


			//DVH PTV
			rttb::io::other::DVHXMLFileReader dvhReader = rttb::io::other::DVHXMLFileReader(DVH_FILENAME_PTV);
			DVHPointer dvhPtr = dvhReader.generateDVH();

			CHECK_CLOSE(6.04759613161786830000e+001, models::getEUD(dvhPtr, 10), toleranceEUD);

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
			CHECK_EQUAL(alpha, tcplq.getAlphaMean());
			CHECK_EQUAL(alpha / beta, tcplq.getAlphaBeta());
			CHECK_EQUAL(roh, tcplq.getRho());

			CHECK_NO_THROW(tcplq.init());

			if (tcplq.init())
			{
				CHECK_CLOSE(1.00497232941856940000e-127, tcplq.getValue(), tolerance);
			}

			CurveDataType curve = models::getCurveDoseVSBioModel(tcplq, normalizationDose);
			CurveDataType::iterator it;

			for (it = curve.begin(); it != curve.end(); ++it)
			{
				if ((*it).first < 72)
				{
					CHECK_EQUAL(0, (*it).second);
				}
				else if ((*it).first > 150)
				{
					CHECK((*it).second > 0.9);
				}
			}

			models::BioModelParamType alphaBeta = 10;
			tcplq.setParameters(alpha, alphaBeta, roh, 0.08);
			CHECK_EQUAL(alpha, tcplq.getAlphaMean());
			CHECK_EQUAL(alphaBeta, tcplq.getAlphaBeta());
			CHECK_EQUAL(roh, tcplq.getRho());

			if (tcplq.init())
			{
				CHECK_CLOSE(1.84e-005, tcplq.getValue(), tolerance);
			}

			normalizationDose = 40;
			curve = models::getCurveDoseVSBioModel(tcplq, normalizationDose);

			alpha = 1;
			alphaBeta = 14.5;
			tcplq.setAlpha(alpha);
			tcplq.setAlphaBeta(alphaBeta);
			tcplq.setRho(roh);
			CHECK_EQUAL(alpha, tcplq.getAlphaMean());
			CHECK_EQUAL(alphaBeta, tcplq.getAlphaBeta());
			CHECK_EQUAL(roh, tcplq.getRho());

			if (tcplq.init())
			{
				CHECK_CLOSE(0.954885, tcplq.getValue(), toleranceEUD);
			}

			alpha = 0.9;
			alphaBeta = 1;
			tcplq.setAlpha(alpha);
			tcplq.setAlphaBeta(alphaBeta);
			tcplq.setRho(roh);
			CHECK_EQUAL(alpha, tcplq.getAlphaMean());
			CHECK_EQUAL(alphaBeta, tcplq.getAlphaBeta());
			CHECK_EQUAL(roh, tcplq.getRho());

			if (tcplq.init())
			{
				CHECK_EQUAL(1, tcplq.getValue());
			}


			//TCP LQ Test
			alpha = 0.3;
			beta = 0.03;
			roh = 10000000;
			numFractions = 20;
			rttb::models::TCPLQModel tcplq_test = rttb::models::TCPLQModel(dvh_test_tv, alpha, beta, roh,
			                                      numFractions);
			CHECK_EQUAL(alpha, tcplq_test.getAlphaMean());
			CHECK_EQUAL(alpha / beta, tcplq_test.getAlphaBeta());
			CHECK_EQUAL(roh, tcplq_test.getRho());
			CHECK_NO_THROW(tcplq_test.init());

			if (tcplq_test.init())
			{
				CHECK_CLOSE(9.79050278878883180000e-001, tcplq_test.getValue(), tolerance);
			}

			normalizationDose = 60;
			curve = models::getCurveDoseVSBioModel(tcplq_test, normalizationDose);

			//DVH HT 1
			rttb::io::other::DVHXMLFileReader dvhReader2 = rttb::io::other::DVHXMLFileReader(DVH_FILENAME_NT1);
			DVHPointer dvhPtr2 = dvhReader2.generateDVH();

			CHECK_CLOSE(1.07920836034015810000e+001, models::getEUD(dvhPtr2, 10), toleranceEUD);

			//test RTNTCPLKBModel
			rttb::models::NTCPLKBModel lkb = rttb::models::NTCPLKBModel();
			models::BioModelParamType aVal = 10;
			models::BioModelParamType mVal = 0.16;
			models::BioModelParamType d50Val = 55;
			CHECK_EQUAL(0, lkb.getA());
			CHECK_EQUAL(0, lkb.getM());
			CHECK_EQUAL(0, lkb.getD50());
			lkb.setDVH(dvhPtr2);
			CHECK_EQUAL(dvhPtr2, lkb.getDVH());
			lkb.setA(aVal);
			CHECK_EQUAL(aVal, lkb.getA());
			lkb.setM(mVal);
			CHECK_EQUAL(mVal, lkb.getM());
			lkb.setD50(d50Val);
			CHECK_EQUAL(d50Val, lkb.getD50());
			CHECK_NO_THROW(lkb.init());

			if (lkb.init())
			{
				CHECK_CLOSE(2.53523522831366570000e-007, lkb.getValue(), tolerance);
			}

			//test RTNTCPRSModel
			rttb::models::NTCPRSModel rs = rttb::models::NTCPRSModel();
			models::BioModelParamType gammaVal = 1.7;
			models::BioModelParamType sVal = 1;
			CHECK_EQUAL(0, rs.getGamma());
			CHECK_EQUAL(0, rs.getS());
			CHECK_EQUAL(0, rs.getD50());
			rs.setDVH(dvhPtr2);
			CHECK_EQUAL(dvhPtr2, rs.getDVH());
			rs.setD50(d50Val);
			CHECK_EQUAL(d50Val, rs.getD50());
			rs.setGamma(gammaVal);
			CHECK_EQUAL(gammaVal, rs.getGamma());
			rs.setS(sVal);
			CHECK_EQUAL(sVal, rs.getS());
			CHECK_NO_THROW(rs.init());

			if (rs.init())
			{
				CHECK_CLOSE(3.70385888626145740000e-009, rs.getValue(), tolerance);
			}

			//DVH HT 2
			rttb::io::other::DVHXMLFileReader dvhReader3 = rttb::io::other::DVHXMLFileReader(DVH_FILENAME_NT2);
			DVHPointer dvhPtr3 = dvhReader3.generateDVH();
			CHECK_CLOSE(1.26287047025885110000e+001, models::getEUD(dvhPtr3, 10), toleranceEUD);

			//test RTNTCPLKBModel
			aVal = 10;
			mVal = 0.16;
			d50Val = 55;

			lkb.setDVH(dvhPtr3);
			CHECK_EQUAL(dvhPtr3, lkb.getDVH());
			lkb.setA(aVal);
			CHECK_EQUAL(aVal, lkb.getA());
			lkb.setM(mVal);
			CHECK_EQUAL(mVal, lkb.getM());
			lkb.setD50(d50Val);
			CHECK_EQUAL(d50Val, lkb.getD50());

			if (lkb.init())
			{
				CHECK_CLOSE(7.36294657754956700000e-007, lkb.getValue(), tolerance);
			}

			//test RTNTCPRSModel
			rs = rttb::models::NTCPRSModel();
			gammaVal = 1.7;
			sVal = 1;
			CHECK_EQUAL(0, rs.getGamma());
			CHECK_EQUAL(0, rs.getS());
			CHECK_EQUAL(0, rs.getD50());
			rs.setDVH(dvhPtr3);
			CHECK_EQUAL(dvhPtr3, rs.getDVH());
			rs.setD50(d50Val);
			CHECK_EQUAL(d50Val, rs.getD50());
			rs.setGamma(gammaVal);
			CHECK_EQUAL(gammaVal, rs.getGamma());
			rs.setS(sVal);
			CHECK_EQUAL(sVal, rs.getS());

			if (rs.init())
			{
				CHECK_CLOSE(1.76778795490939440000e-007, rs.getValue(), tolerance);
			}


			//DVH HT 3
			rttb::io::other::DVHXMLFileReader dvhReader4 = rttb::io::other::DVHXMLFileReader(DVH_FILENAME_NT3);
			DVHPointer dvhPtr4 = dvhReader4.generateDVH();
			CHECK_CLOSE(2.18212982041056310000e+001, models::getEUD(dvhPtr4, 10), toleranceEUD);

			//test RTNTCPLKBModel
			aVal = 10;
			mVal = 0.16;
			d50Val = 55;
			lkb.setDVH(dvhPtr4);
			CHECK_EQUAL(dvhPtr4, lkb.getDVH());
			lkb.setA(aVal);
			CHECK_EQUAL(aVal, lkb.getA());
			lkb.setM(mVal);
			CHECK_EQUAL(mVal, lkb.getM());
			lkb.setD50(d50Val);
			CHECK_EQUAL(d50Val, lkb.getD50());

			if (lkb.init())
			{
				CHECK_CLOSE(8.15234192641929420000e-005, lkb.getValue(), tolerance);
			}

			//test RTNTCPRSModel
			rs = rttb::models::NTCPRSModel();
			gammaVal = 1.7;
			sVal = 1;
			CHECK_EQUAL(0, rs.getGamma());
			CHECK_EQUAL(0, rs.getS());
			CHECK_EQUAL(0, rs.getD50());
			rs.setDVH(dvhPtr4);
			CHECK_EQUAL(dvhPtr4, rs.getDVH());
			rs.setD50(d50Val);
			CHECK_EQUAL(d50Val, rs.getD50());
			rs.setGamma(gammaVal);
			CHECK_EQUAL(gammaVal, rs.getGamma());
			rs.setS(sVal);
			CHECK_EQUAL(sVal, rs.getS());

			if (rs.init())
			{
				CHECK_CLOSE(2.02607985020919480000e-004, rs.getValue(), tolerance);
			}


			//test using Virtuos Pleuramesotheliom MPM_LR_ah
			//DVH PTV


			rttb::io::other::DVHXMLFileReader dR_Target = rttb::io::other::DVHXMLFileReader(DVH_Virtuos_Target);
			DVHPointer dvhPtrTarget = dR_Target.generateDVH();

			rttb::io::other::DVHXMLFileReader dR_Lung = rttb::io::other::DVHXMLFileReader(DVH_Virtuos_Lung);
			DVHPointer dvhPtrLung = dR_Lung.generateDVH();


			//test TCP LQ Model
			models::BioModelParamType alphaMean = 0.34;
			models::BioModelParamType alphaVarianz = 0.02;
			models::BioModelParamType alpha_beta = 28;
			models::BioModelParamType rho = 1200;

			int numFractionsVirtuos = 27;

			rttb::models::TCPLQModel tcplqVirtuos = rttb::models::TCPLQModel(dvhPtrTarget, rho,
			                                        numFractionsVirtuos, alpha_beta,
			                                        alphaMean, alphaVarianz);

			if (tcplqVirtuos.init())
			{
				CHECK_CLOSE(0.8894, tcplqVirtuos.getValue(), 1e-4);
			}

			models::BioModelParamType d50Mean = 20;
			models::BioModelParamType m = 0.36;
			models::BioModelParamType a = 1.06;

			rttb::models::NTCPLKBModel lkbVirtuos = rttb::models::NTCPLKBModel(dvhPtrLung, d50Mean, m, a);

			if (lkbVirtuos.init())
			{
				CHECK_CLOSE(0.0397, lkbVirtuos.getValue(), 1e-4);
			}




			RETURN_AND_REPORT_TEST_SUCCESS;

		}

	}//testing
}//rttb
