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

#include "litCheckMacros.h"

#include "rttbBaseType.h"
#include "rttbBaseTypeModels.h"
#include "rttbBioModel.h"
#include "rttbDVH.h"
#include "rttbTCPLQModel.h"
#include "rttbNTCPLKBModel.h"
#include "rttbNTCPRSModel.h"
#include "rttbBaseTypeModels.h"
#include "rttbBioModelCurve.h"
#include "rttbInvalidParameterException.h"
#include "rttbBioModelScatterPlots.h"




namespace rttb
{
	namespace testing
	{

		typedef core::DVH::DataDifferentialType DataDifferentialType;

		/*! @brief RTBioModelTest. TCP calculated using a DVH PTV and LQ Model. NTCP tested using 3 Normal Tissue DVHs and LKB/RS Model.
		TCPLQ:
		1) test constructors (values as expected?)
		2) test init (calcTCPxxx)
		3) test set/get<Values>

		NTCP (LKB):
		1) test constructors (values as expected?)
		2) test init (calcxxx)
		3) test set/get<Values>

		NTCP (RS):
		1) test constructors (values as expected?)
		2) test init (calcxxx)
		3) test set/get<Values>
		*/
		int BioModelTest(int argc, char* argv[])
		{
			PREPARE_DEFAULT_TEST_REPORTING;

			//generate artificial DVH and corresponding statistical values
			DoseTypeGy binSize = DoseTypeGy(0.1);
			DoseVoxelVolumeType voxelVolume = 8;

			DataDifferentialType aDataDifferential;

			DoseCalcType value = 0;
			DVHVoxelNumber numberOfVoxels = 0;

			// creat default values
			for (int i = 0; i < 98; i++)
			{
				value = 0;
				numberOfVoxels += value;
				aDataDifferential.push_back(value);
			}

			aDataDifferential.push_back(10);
			aDataDifferential.push_back(20);

			const IDType structureID = "myStructure";
			const IDType doseID = "myDose";
			const IDType voxelizationID = "myVoxelization";

			core::DVH::DVHPointer dvhPtr = boost::make_shared<core::DVH>(aDataDifferential, binSize,
			                               voxelVolume, structureID,
			                               doseID, voxelizationID);

			//test TCP LQ Model
			models::BioModelParamType alpha = 0.35;
			models::BioModelParamType beta = 0.023333333333333;
			models::BioModelParamType roh = 10000000;
			int numFractions = 8;

			DoseTypeGy normalizationDose = 50;

			//1) test constructors (values as expected?)
			rttb::models::TCPLQModel tcplq = rttb::models::TCPLQModel();
			CHECK_EQUAL(0, tcplq.getAlphaMean());
			CHECK_EQUAL(0, tcplq.getAlphaBeta());
			CHECK_EQUAL(0, tcplq.getRho());
			CHECK_EQUAL(0, tcplq.getValue());


			tcplq = rttb::models::TCPLQModel(dvhPtr, roh, numFractions, alpha / beta, alpha, 0.08);
			CHECK_EQUAL(alpha, tcplq.getAlphaMean());
			CHECK_EQUAL(alpha / beta, tcplq.getAlphaBeta());
			CHECK_EQUAL(roh, tcplq.getRho());
			CHECK_EQUAL(0, tcplq.getValue());


			tcplq = rttb::models::TCPLQModel();
			CHECK_EQUAL(0, tcplq.getAlphaMean());
			CHECK_EQUAL(0, tcplq.getAlphaBeta());
			CHECK_EQUAL(0, tcplq.getRho());
			CHECK_EQUAL(0, tcplq.getValue());

			tcplq = rttb::models::TCPLQModel(dvhPtr, alpha, beta, roh, numFractions);
			CHECK_EQUAL(alpha, tcplq.getAlphaMean());
			CHECK_EQUAL(alpha / beta, tcplq.getAlphaBeta());
			CHECK_EQUAL(roh, tcplq.getRho());
			CHECK_EQUAL(0, tcplq.getValue());

			//2) test init (calcTCPxxx)
			CHECK_NO_THROW(tcplq.init(1));

			//3) test set/get<Values>
			CHECK_EQUAL(0, tcplq.getValue());
			CHECK_NO_THROW(tcplq.setParameters(alpha, 10, roh, 0.08));
			CHECK_EQUAL(10, tcplq.getAlphaBeta());
			CHECK_EQUAL(0.08, tcplq.getAlphaVariance());
			CHECK_EQUAL(alpha, tcplq.getAlphaMean());
			CHECK_EQUAL(roh, tcplq.getRho());

			CHECK_NO_THROW(models::getCurveDoseVSBioModel(tcplq, normalizationDose));

			std::vector<models::BioModelParamType> aParameterVector;
			aParameterVector.push_back(alpha + 0.02);
			CHECK_THROW_EXPLICIT(tcplq.setParameterVector(aParameterVector), core::InvalidParameterException);
			aParameterVector.push_back(0.06);
			aParameterVector.push_back(8);
			aParameterVector.push_back(roh / 10);
			CHECK_NO_THROW(tcplq.setParameterVector(aParameterVector));
			CHECK_EQUAL(8, tcplq.getAlphaBeta());
			CHECK_EQUAL(0.06, tcplq.getAlphaVariance());
			CHECK_EQUAL(alpha + 0.02, tcplq.getAlphaMean());
			CHECK_EQUAL(roh / 10, tcplq.getRho());

			for (int i = 0; i < 4; i++)
			{
				CHECK_NO_THROW(tcplq.setParameterByID(i, models::BioModelParamType(i)));
			}

			CHECK_THROW_EXPLICIT(tcplq.setParameterByID(4, 4.0), core::InvalidParameterException);

			CHECK_EQUAL(0, tcplq.getParameterID("alphaMean"));
			CHECK_EQUAL(0, tcplq.getAlphaMean());
			CHECK_EQUAL(1, tcplq.getParameterID("alphaVariance"));
			CHECK_EQUAL(1, tcplq.getAlphaVariance());
			CHECK_EQUAL(2, tcplq.getParameterID("alpha_beta"));
			CHECK_EQUAL(2, tcplq.getAlphaBeta());
			CHECK_EQUAL(3, tcplq.getParameterID("rho"));
			CHECK_EQUAL(3, tcplq.getRho());


			//test NTCPLKBModel
			//1) test constructors (values as expected?)
			models::BioModelParamType aVal = 10;
			models::BioModelParamType mVal = 0.16;
			models::BioModelParamType d50Val = 35;
			CHECK_NO_THROW(rttb::models::NTCPLKBModel());
			rttb::models::NTCPLKBModel lkb = rttb::models::NTCPLKBModel();
			CHECK_EQUAL(0, lkb.getA());
			CHECK_EQUAL(0, lkb.getM());
			CHECK_EQUAL(0, lkb.getD50());
			CHECK_EQUAL(0, lkb.getValue());
			CHECK_NO_THROW(rttb::models::NTCPLKBModel(dvhPtr, d50Val, mVal, aVal));
			lkb = rttb::models::NTCPLKBModel(dvhPtr, d50Val, mVal, aVal);
			CHECK_EQUAL(0, lkb.getValue());
			CHECK_EQUAL(dvhPtr, lkb.getDVH());
			CHECK_EQUAL(aVal, lkb.getA());
			CHECK_EQUAL(mVal, lkb.getM());
			CHECK_EQUAL(d50Val, lkb.getD50());

			//2) test init (calcxxx)
			CHECK_NO_THROW(lkb.init(1));
			lkb.getValue();

			//3) test set/get<Values>
			lkb = rttb::models::NTCPLKBModel();
			CHECK_EQUAL(0, lkb.getA());
			CHECK_EQUAL(0, lkb.getM());
			CHECK_EQUAL(0, lkb.getD50());
			lkb.setDVH(dvhPtr);
			CHECK_EQUAL(dvhPtr, lkb.getDVH());
			lkb.setA(aVal);
			CHECK_EQUAL(aVal, lkb.getA());
			lkb.setM(mVal);
			CHECK_EQUAL(mVal, lkb.getM());
			lkb.setD50(d50Val);
			CHECK_EQUAL(d50Val, lkb.getD50());

			CHECK_NO_THROW(models::getCurveEUDVSBioModel(lkb));

			aParameterVector.clear();
			aParameterVector.push_back(d50Val + 5);
			CHECK_THROW_EXPLICIT(lkb.setParameterVector(aParameterVector), core::InvalidParameterException);
			aParameterVector.push_back(mVal + 0.2);
			aParameterVector.push_back(aVal + 0.5);
			CHECK_NO_THROW(lkb.setParameterVector(aParameterVector));
			CHECK_EQUAL(aVal + 0.5, lkb.getA());
			CHECK_EQUAL(mVal + 0.2, lkb.getM());
			CHECK_EQUAL(d50Val + 5, lkb.getD50());

			for (int i = 0; i < 3; i++)
			{
				CHECK_NO_THROW(lkb.setParameterByID(i, models::BioModelParamType(i)));
			}

			CHECK_THROW_EXPLICIT(lkb.setParameterByID(3, 4.0), core::InvalidParameterException);

			CHECK_EQUAL(0, lkb.getParameterID("d50"));
			CHECK_EQUAL(0, lkb.getD50());
			CHECK_EQUAL(1, lkb.getParameterID("m"));
			CHECK_EQUAL(1, lkb.getM());
			CHECK_EQUAL(2, lkb.getParameterID("a"));
			CHECK_EQUAL(2, lkb.getA());

			//test NTCPRSModel
			//1) test constructors (values as expected?)
			CHECK_NO_THROW(rttb::models::NTCPRSModel());
			models::BioModelParamType gammaVal = 1.7;
			models::BioModelParamType sVal = 1;
			CHECK_NO_THROW(rttb::models::NTCPRSModel(dvhPtr, d50Val, gammaVal, sVal));
			rttb::models::NTCPRSModel rs = rttb::models::NTCPRSModel(dvhPtr, d50Val, gammaVal, sVal);
			CHECK_EQUAL(dvhPtr, rs.getDVH());
			CHECK_EQUAL(d50Val, rs.getD50());
			CHECK_EQUAL(gammaVal, rs.getGamma());
			CHECK_EQUAL(sVal, rs.getS());

			rs = rttb::models::NTCPRSModel();
			CHECK_EQUAL(0, rs.getGamma());
			CHECK_EQUAL(0, rs.getS());
			CHECK_EQUAL(0, rs.getD50());

			//3) test set/get<Values>
			rs.setDVH(dvhPtr);
			CHECK_EQUAL(dvhPtr, rs.getDVH());
			rs.setD50(d50Val);
			CHECK_EQUAL(d50Val, rs.getD50());
			rs.setGamma(gammaVal);
			CHECK_EQUAL(gammaVal, rs.getGamma());
			rs.setS(sVal);
			CHECK_EQUAL(sVal, rs.getS());

			//2) test init (calcxxx)
			CHECK_NO_THROW(rs.init(1));

			//3) test set/get<Values> continued
			aParameterVector.clear();
			aParameterVector.push_back(d50Val + 5);
			CHECK_THROW_EXPLICIT(rs.setParameterVector(aParameterVector), core::InvalidParameterException);
			aParameterVector.push_back(gammaVal + 0.2);
			aParameterVector.push_back(sVal + 0.5);
			CHECK_NO_THROW(rs.setParameterVector(aParameterVector));
			CHECK_EQUAL(gammaVal + 0.2, rs.getGamma());
			CHECK_EQUAL(sVal + 0.5, rs.getS());
			CHECK_EQUAL(d50Val + 5, rs.getD50());

			for (int i = 0; i < 3; i++)
			{
				CHECK_NO_THROW(rs.setParameterByID(i, models::BioModelParamType(i)));
			}

			CHECK_THROW_EXPLICIT(rs.setParameterByID(3, 4.0), core::InvalidParameterException);

			CHECK_EQUAL(0, rs.getParameterID("d50"));
			CHECK_EQUAL(0, rs.getD50());
			CHECK_EQUAL(1, rs.getParameterID("gamma"));
			CHECK_EQUAL(1, rs.getGamma());
			CHECK_EQUAL(2, rs.getParameterID("s"));
			CHECK_EQUAL(2, rs.getS());

			//Scatter plot tests
			CHECK_NO_THROW(models::getScatterPlotVary1Parameter(tcplq, 0, alpha, 0,
			               normalizationDose)); //variance=0, will be set to 1e-30
			CHECK_THROW_EXPLICIT(models::getScatterPlotVary1Parameter(tcplq, 0, alpha, alpha * 0.1, 0),
			                     core::InvalidParameterException);//normalisationdose=0
			CHECK_THROW_EXPLICIT(models::getScatterPlotVary1Parameter(tcplq, 0, alpha, alpha * 0.1,
			                     normalizationDose, 10000, 0, 0),
			                     core::InvalidParameterException);//maxDose-minDose=0

			RETURN_AND_REPORT_TEST_SUCCESS;

		}

	}//testing
}//rttb