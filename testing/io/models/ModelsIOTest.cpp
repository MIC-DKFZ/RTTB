// -----------------------------------------------------------------------
// RTToolbox - DKFZ radiotherapy quantitative evaluation library
//
// (c) Copyright 2007, DKFZ, Heidelberg, Germany
// ALL RIGHTS RESERVED
//
// THIS FILE CONTAINS CONFIDENTIAL AND PROPRIETARY INFORMATION OF DKFZ.
// ANY DUPLICATION, MODIFICATION, DISTRIBUTION, OR
// DISCLOSURE IN ANY FORM, IN WHOLE, OR IN PART, IS STRICTLY PROHIBITED
// WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF DKFZ.
//
//------------------------------------------------------------------------

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <fstream>

#include "litCheckMacros.h"
#include "rttbTCPLQModel.h"
#include "rttbNTCPLKBModel.h"

#include <boost/make_shared.hpp>
#include "boost/filesystem.hpp"

#include "rttbModelXMLWriter.h"

namespace rttb
{

	namespace testing
	{
		static std::string readFile(const std::string& filename);
		int ModelsIOTest(int argc, char* argv[])
		{
			std::string expectedxmlfilenametcpleq;
			std::string expectedxmlfilenamentcplk;

			if (argc > 2)
			{
				expectedxmlfilenametcpleq = argv[1];
				expectedxmlfilenamentcplk = argv[2];
			}

			typedef core::DVH::DataDifferentialType DataDifferentialType;

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

			core::DVH::Pointer dvhPtr = boost::make_shared<core::DVH>(aDataDifferential, binSize,
			                               voxelVolume, structureID,
			                               doseID, voxelizationID);

			//test TCP LQ Model
			models::BioModelParamType alpha = 0.35;
			models::BioModelParamType beta = 0.023333333333333;
			models::BioModelParamType roh = 10000000;
			int numFractions = 8;


			boost::shared_ptr<rttb::models::TCPLQModel> tcplq = boost::make_shared<rttb::models::TCPLQModel>
			        (dvhPtr, roh, numFractions, alpha / beta, alpha, 0.08);

			std::string filename = "BioModeltcpleqIOTest.xml";
			rttb::io::models::ModelXMLWriter writer = rttb::io::models::ModelXMLWriter(filename, tcplq, false);
			CHECK_NO_THROW(writer.writeModel());
			CHECK_EQUAL(boost::filesystem::exists(filename), true);

			std::string defaultAsIs = readFile(filename);
			std::string defaultExpected = readFile(expectedxmlfilenametcpleq);
			//Does not work due to double imprecisions. As long as we don't have a ModelXMLReader, a check does not makes sense. See T21832
			//CHECK_EQUAL(defaultAsIs, defaultExpected);
			CHECK_EQUAL(std::remove(filename.c_str()), 0);

			//test NTCPLKBModel
			models::BioModelParamType aVal = 10;
			models::BioModelParamType mVal = 0.16;
			models::BioModelParamType d50Val = 35;

			boost::shared_ptr<rttb::models::NTCPLKBModel> ntcplk =
			    boost::make_shared<rttb::models::NTCPLKBModel>(dvhPtr, d50Val, mVal, aVal);
			filename = "BioModelntcplkIOTest.xml";
			rttb::io::models::ModelXMLWriter writer2 = rttb::io::models::ModelXMLWriter(filename, ntcplk);
			CHECK_NO_THROW(writer2.writeModel());
			CHECK_EQUAL(boost::filesystem::exists(filename), true);


			defaultAsIs = readFile(filename);
			defaultExpected = readFile(expectedxmlfilenamentcplk);
			//Does not work due to double imprecisions. As long as we don't have a ModelXMLReader, a check does not makes sense. See T21832
			//CHECK_EQUAL(defaultAsIs, defaultExpected);
			CHECK_EQUAL(std::remove(filename.c_str()), 0);
			std::string dvhFilename = "dvhfor" + ntcplk->getModelType() + ".xml";
			CHECK_EQUAL(std::remove(dvhFilename.c_str()), 0);

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

		std::string readFile(const std::string& filename)
		{
			std::ifstream fileStream(filename.c_str());
			std::string content((std::istreambuf_iterator<char>(fileStream)),
			                    (std::istreambuf_iterator<char>()));
			return content;
		}
	}
}
