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
// @version $Revision: 1333 $ (last changed revision)
// @date $Date: 2016-04-22 11:12:14 +0200 (Fr, 22 Apr 2016) $ (last change date)
// @author $Author: hentsch $ (last changed by)
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>

#include "litCheckMacros.h"

#include "rttbGenericMaskedDoseIterator.h"
#include "rttbGenericDoseIterator.h"
#include "rttbDicomDoseAccessor.h"
#include "rttbDicomFileDoseAccessorGenerator.h"
#include "rttbDicomFileStructureSetGenerator.h"
#include "rttbDVHCalculator.h"
#include "rttbBoostMaskAccessor.h"
#include "rttbBoostMaskRedesignAccessor.h"
#include "rttbDVHXMLFileWriter.h"
#include "rttbDVHXMLFileReader.h"
#include "../io/other/CompareDVH.h"

namespace rttb
{

	namespace testing
	{

		/*! @brief VoxelizationDVHComparisonTests.
			Computes the DVH difference of different voxelizations (OTB/Boost legacy/Boost)
            Writes the difference in a DVH and saves in a file
		*/


        core::DoseIteratorInterface::DoseIteratorPointer createMaskDoseIterator(masks::boost::BoostMaskAccessor::StructTypePointer rtstruct, 
            core::GenericDoseIterator::DoseAccessorPointer doseAccessor, const std::string& voxelizationType)
        {
            core::GenericMaskedDoseIterator::MaskAccessorPointer spMaskAccessor;
            if (voxelizationType == "Boost"){
                auto spBoostMaskAccessor =
                    ::boost::make_shared<masks::boost::BoostMaskAccessor>(rtstruct, doseAccessor->getGeometricInfo());
                spBoostMaskAccessor->updateMask();
                spMaskAccessor = spBoostMaskAccessor;
            }
            else if (voxelizationType == "BoostRedesign"){
                auto spBoostRedesignMaskAccessor =
                    ::boost::make_shared<masks::boostRedesign::BoostMaskAccessor>(rtstruct, doseAccessor->getGeometricInfo());
                spBoostRedesignMaskAccessor->updateMask();
                spMaskAccessor = spBoostRedesignMaskAccessor;
            }

            auto spMaskedDoseIteratorTmp =
                ::boost::make_shared<core::GenericMaskedDoseIterator>(spMaskAccessor, doseAccessor);
            core::DoseIteratorInterface::DoseIteratorPointer spMaskedDoseIterator(spMaskedDoseIteratorTmp);
            return spMaskedDoseIterator;
        }

        rttb::core::DVH calcDVH(core::DVHCalculator::DoseIteratorPointer doseIterator, const IDType& structUID, const IDType& doseUID)
        {
            rttb::core::DVHCalculator calc(doseIterator, structUID, doseUID);
            rttb::core::DVH dvh = *(calc.generateDVH());
            return dvh;
        }

        void writeCumulativeDVH(const std::string& filename, rttb::core::DVH dvh)
        {
            DVHType typeCum = { DVHType::Cumulative };
            io::other::DVHXMLFileWriter dvhWriter(filename, typeCum);
            dvhWriter.writeDVH(boost::make_shared<rttb::core::DVH>(dvh));
        }


		int VoxelizationDVHComparisonTest(int argc, char* argv[])
		{
			typedef core::GenericDoseIterator::DoseAccessorPointer DoseAccessorPointer;
			typedef core::GenericMaskedDoseIterator::MaskAccessorPointer MaskAccessorPointer;
			typedef core::DVHCalculator::DoseIteratorPointer DoseIteratorPointer;
			typedef core::DVHCalculator::MaskedDoseIteratorPointer MaskedDoseIteratorPointer;
            typedef masks::boost::BoostMaskAccessor::StructTypePointer StructTypePointer;
			typedef core::DVH::DVHPointer DVHPointer;
			typedef core::StructureSetGeneratorInterface::StructureSetPointer StructureSetPointer;

			PREPARE_DEFAULT_TEST_REPORTING;

			std::string RTSTRUCT_FILENAME;
			std::string RTDOSE_FILENAME;
            std::string RTDVH_XML_OTB_DIRECTORY;
            std::string RTDVH_XML_BOOST_DIRECTORY;
            std::string RTDVH_XML_BOOSTREDESIGN_DIRECTORY;

			if (argc > 5)
			{
				RTSTRUCT_FILENAME = argv[1];
                RTDOSE_FILENAME = argv[2];
                RTDVH_XML_OTB_DIRECTORY = argv[3];
                RTDVH_XML_BOOST_DIRECTORY = argv[4];
                RTDVH_XML_BOOSTREDESIGN_DIRECTORY = argv[5];
			}

			// read dicom-rt dose
			io::dicom::DicomFileDoseAccessorGenerator doseAccessorGenerator1(RTDOSE_FILENAME.c_str());

			DoseAccessorPointer doseAccessor1(doseAccessorGenerator1.generateDoseAccessor());

			//create a vector of MaskAccessors (one for each structure)
			StructureSetPointer rtStructureSet = io::dicom::DicomFileStructureSetGenerator(
			        RTSTRUCT_FILENAME.c_str()).generateStructureSet();

			//start evaluation
			clock_t start(clock());

			if (rtStructureSet->getNumberOfStructures() > 0)
			{
				for (int j = 0; j < static_cast<int>(rtStructureSet->getNumberOfStructures()); j++)
				{
					std::cout << rtStructureSet->getStructure(j)->getLabel() << std::endl;

                    auto spMaskedDoseIteratorBoostRedesign = createMaskDoseIterator(rtStructureSet->getStructure(j), doseAccessor1, "BoostRedesign");
					
                    auto label = rtStructureSet->getStructure(j)->getLabel();
                    ::boost::replace_all(label, "/", "_");
                    boost::filesystem::path dvhOTBFilename(RTDVH_XML_OTB_DIRECTORY);
                    dvhOTBFilename /= "DVH_" + label + ".xml";

                    boost::filesystem::path dvhBoostFilename(RTDVH_XML_BOOST_DIRECTORY);
                    dvhBoostFilename /= "DVH_" + label + ".xml";

                    io::other::DVHXMLFileReader dvhReaderOTB(dvhOTBFilename.string());
                    auto dvhOTB = dvhReaderOTB.generateDVH();

                    io::other::DVHXMLFileReader dvhReaderBoost(dvhBoostFilename.string());
                    auto dvhBoost = dvhReaderBoost.generateDVH();

                    auto dvhBoostRedesign = calcDVH(spMaskedDoseIteratorBoostRedesign, (rtStructureSet->getStructure(j))->getUID(), doseAccessor1->getUID());
                    boost::filesystem::path dvhBoostRedesignFilename(RTDVH_XML_BOOSTREDESIGN_DIRECTORY);
                    dvhBoostRedesignFilename /= "DVH_" + label + ".xml";
                    writeCumulativeDVH(dvhBoostRedesignFilename.string(), dvhBoostRedesign);

					std::cout << "=== Dose 1 Structure " << j << "===" << std::endl;
                    std::cout << "with OTB voxelization: " << std::endl;
                    std::cout << dvhOTB << std::endl;
                    std::cout << "with Boost voxelization: " << std::endl;
                    std::cout << dvhBoost << std::endl;
                    std::cout << "with BoostRedesign voxelization: " << std::endl;
                    std::cout << dvhBoostRedesign << std::endl;

					//compare DVH for different voxelizations
                    auto diffDVH = computeDiffDVH(dvhOTB, boost::make_shared<core::DVH>(dvhBoostRedesign));
                    boost::filesystem::path dvhBoostRedesignDiffFilename(RTDVH_XML_BOOSTREDESIGN_DIRECTORY);
                    dvhBoostRedesignDiffFilename /= "DVHDiff_" + label + ".xml";
                    writeCumulativeDVH(dvhBoostRedesignDiffFilename.string(), *diffDVH);
				}
			}

			clock_t finish(clock());

			std::cout << "DVH Calculation time: " << finish - start << " ms" << std::endl;

			RETURN_AND_REPORT_TEST_SUCCESS;
		}

	}//testing
}//rttb

