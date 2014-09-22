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
/*!
// @file
// @version $Revision$ (last changed revision)
// @date $Date$ (last change date)
// @author zhangl (last changed by)
// @author *none* (Reviewer)
// @author zhangl (Programmer)
//
// Subversion HeadURL: $HeadURL: http://sidt-hpc1/dkfz_repository/NotMeVisLab/SIDT/RTToolbox/branch/restructure/testing/core/DVHCalculatorTest.cpp $
*/

// this file defines the rttbCoreTests for the test driver
// and all it expects is that you have a function called RegisterTests


#include "litCheckMacros.h"


#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#include "dcmtk/dcmrt/drtdose.h"
#include "dcmtk/dcmrt/drtstruct.h"

#include "../../code/core/rttbMaskedDoseIteratorInterface_NEW.h"
#include "../../code/core/rttbGenericMaskedDoseIterator_NEW.h"
#include "../../code/core/rttbGenericDoseIterator_NEW.h"
#include "../../code/core/rttbDVHCalculator.h"
#include "../../code/io/dicom/rttbDicomStructureSet.h"
#include "../../code/core/rttbStructure.h"
#include "../../code/core/rttbNullPointerException.h"
#include "../../code/core/rttbInvalidParameterException.h"
#include "../../code/core/rttbInvalidDoseException.h"
#include "../../code/core/rttbException.h"
#include "../../code/core/rttbBaseType_NEW.h"

//#include "CreateTestStructure.h" - currently not used

// sollen externe Methoden verwendet werden?
#include "../../code/io/dicom/rttbDicomDoseAccessor.h"
#include "../../code/masks/rttbOTBMaskAccessor.h"
#include "../../code/algorithms/rttbDoseStatistics.h"

#include <boost/make_shared.hpp>





namespace rttb{

  namespace testing{

	  	

    /*! @brief DVHCalculatorTest.
    1. DVH calculator test: use MaskedDicomDoseIterator(&rtdose, &rtstr) -> now use rttbGenericMaskedDoseIterator_NEW.h
    2. DVH Calculator test: use cache of MaskedDicomDoseIterator resetDose -> obsolete
    3. DVH Calculator test: use MaskedDicomDoseIteratorRandomSampling ->obsolete use dummy mask?
    4. DVH Calculator test: use DicomDoseIteratorUseMask -> obsolete/ ../io/dicom?
    5. DVH Calculator test: use manually generated structure and Mask(DoseIteratorInterface*, Structure* ) -> check if this should be done here or in ../masks
    6. DVHCalculation Test using constructor Mask( DoseIteratorInterface* aDoseIter  , std::vector<DoseVoxel> aDoseVoxelVector ); -> mask generation should be tested in mask tests use default mask here
    */
    
   
    /*static std::string MASK_FILENAME="../../../RTToolbox/testing/data/DICOM/TestDose/TestMask.dcm";
    static std::string HITDOSE_FILENAME="d:/Dotmobi-SVN/rondo/RondoInterface/Resources/TestData/Dicom/HITDemoData/RTDOSE/HITDemoData_20100316T175500_RTDOSE_450.IMA";
    static std::string HITSTRUCT_FILENAME="d:/Dotmobi-SVN/rondo/RondoInterface/Resources/TestData/Dicom/HITDemoData/RTSTRUCT/HITDemoData_20100407T152606_RTSTRUCT_0.IMA";
    static std::string HITBEAMDOSE_FILENAME="d:/Dotmobi-SVN/rondo/RondoInterface/Resources/TestData/Dicom/HITDemoData/RTDOSE/HITDemoData_20100407T152314_RTDOSE_0.IMA";

    static std::string DVH_FILENAME_WRITE="../../../RTToolbox/testing/data/TestDVH/dvh_write.txt";

    static std::string DVH_FILENAME_PTV_HIT="../../../RTToolbox/testing/data/TestDVH/dvh_PTV_HIT.txt";*/

    int RTDVHCalculatorTest(int argc, char* argv[] )
    {
		typedef core::GenericDoseIterator::DoseAcessorPointer DoseAcessorPointer;
	  typedef core::DoseIteratorInterface::DoseIteratorPointer DoseIteratorPointer;
	  typedef core::MaskedDoseIteratorInterface::MaskAcessorPointer MaskAcessorPointer;
	  typedef masks::OTBMaskAccessor::StructTypePointer StructTypePointer;
	  typedef algorithms::DoseStatistics::ResultListPointer ResultListPointer;


      PREPARE_DEFAULT_TEST_REPORTING;
      //ARGUMENTS: 1: structure file name
      //           2: dose1 file name
      //           3: dose2 file name
      //           4: dose3 file name

      std::string STRUCT_FILENAME;
      std::string DOSE_FILENAME;
      std::string DOSE2_FILENAME;
      std::string DOSE3_FILENAME;

      /*STRUCT_FILENAME="d:/RTToolboxProj/trunk/testing/data/DICOM/H000090/RTSTRUCT/H000090_20100610T144958_RTSTRUCT_479.IMA"; 
      DOSE_FILENAME="d:/RTToolboxProj/trunk/testing/data/DICOM/H000090/RTDOSE/H000090_20100610T144958_RTDOSE_396.IMA";
      DOSE2_FILENAME="d:/RTToolboxProj/trunk/testing/data/DICOM/H000090/RTDOSE/H000090_20100610T144958_RTDOSE_397.IMA"; 
      DOSE3_FILENAME="d:/RTToolboxProj/trunk/testing/data/DICOM/H000090/RTDOSE/H000090_20100610T144958_RTDOSE_398.IMA";*/


      if (argc>1)
      {
        STRUCT_FILENAME = argv[1];
      }
      if (argc>2)
      {
        DOSE_FILENAME = argv[2];
      }
      if (argc>3)
      {
        DOSE2_FILENAME = argv[3];
      }
      if (argc>4)
      {
        DOSE3_FILENAME = argv[4];
      }


//      CHECK_THROW_EXPLICIT(core::DVHCalculator(NULL,"",""),core::NullPointerException); -> tested in individual test

      OFCondition status;
      DcmFileFormat fileformat;

      /* read dicom-rt dose */
      ::DRTDoseIOD rtdose;
//      rttb::core::Structure* rtstr;
      double deltaD=0;
      double deltaV=0.03;
	  // use new architecture accessor+iterator
	  // import dicom files
	  CHECK_NO_THROW(DoseAcessorPointer spDoseAccessor(new core::DicomDoseAccessor(DOSE_FILENAME.c_str())));
	  DoseAcessorPointer spDoseAccessor(new core::DicomDoseAccessor(DOSE_FILENAME.c_str()));
	  std::cout <<"ProcessDose "<< DOSE_FILENAME.c_str()<<std::endl;
	  core::GenericDoseIterator ddit(spDoseAccessor);

      /* read dicom-rt structure set*/ 
	  // use toolbox extensions from Andreas?!: DoseToolDCMStructLoader.h
      ::DRTStructureSetIOD rtstruct;
      status = fileformat.loadFile(STRUCT_FILENAME.c_str());
	  CHECK(status.good());      
	  if (!status.good()){
        std::cerr << "Error: load rt structure set loadFile(STRUCT_FILENAME) failed!"<<std::endl;
        return -1;
      }
      status = rtstruct.read(*fileformat.getDataset());
      if(!status.good()){
        std::cerr << "Error: read DRTStructureSetIOD failed!"<<std::endl;
        return -1;
      }

      //get maximum of rtdose

	  CHECK_NO_THROW(rttb::core::DicomStructureSet rtstrset=rttb::core::DicomStructureSet(&rtstruct));
      rttb::core::DicomStructureSet rtstrset=rttb::core::DicomStructureSet(&rtstruct);

      clock_t start(clock());
      //std::vector<DoseIteratorPointer> mditVector;
	  ResultListPointer resultListMax;
	  ResultListPointer resultListMin;
      if(rtstrset.getNumberOfStructures()>0){
        for(int j=0;j<rtstrset.getNumberOfStructures();j++){
			clock_t startLoop(clock());
          StructTypePointer spRtstr=boost::make_shared<core::Structure>(*(rtstrset.getStructure(j)));
		  rttb::core::GeometricInfo geoInf = ddit.getGeometricInfo();
		  
		  boost::shared_ptr<rttb::masks::OTBMaskAccessor> pOTBMaskAccessor = boost::make_shared<rttb::masks::OTBMaskAccessor>(spRtstr,geoInf, rtdose);
		  MaskAcessorPointer spMaskAccessor(pOTBMaskAccessor);
          boost::shared_ptr<rttb::core::GenericMaskedDoseIterator> spTestMaskedDoseIterator = boost::make_shared<rttb::core::GenericMaskedDoseIterator>(spMaskAccessor, spDoseAccessor); //using Mask
		  DoseIteratorPointer mddit(spTestMaskedDoseIterator);
          mddit->reset();
          std::cout << "Initialization time: "<<(clock()-startLoop)/1000<<" s"<<std::endl;

          rttb::core::DVHCalculator calc(mddit,spRtstr->getUID(),mddit->getDoseUID());
          rttb::core::DVH dvh=*(calc.getDVH());
          std::cout <<"DVH(mask) max: "<< dvh.getMaximum()<< "in "<<spRtstr->getVolume()<<" cm^3"<<std::endl;
        }
      }
      clock_t finish(clock());

      std::cout << "DVH Calculation time: "<<(finish-start)/60000<<" min"<<std::endl;

	  DoseAcessorPointer spDoseAccessor2(new core::DicomDoseAccessor(DOSE2_FILENAME.c_str()));
	  std::cout <<"ProcessDose "<< DOSE2_FILENAME.c_str()<<std::endl;
	  
      start = clock();
	  resultListMax.reset();
	  resultListMin.reset();
      if(rtstrset.getNumberOfStructures()>0){
        for(int j=0;j<rtstrset.getNumberOfStructures();j++){
			clock_t startLoop(clock());
          StructTypePointer spRtstr=boost::make_shared<core::Structure>(*(rtstrset.getStructure(j)));
		  rttb::core::GeometricInfo geoInf = ddit.getGeometricInfo();
		  
		  boost::shared_ptr<rttb::masks::OTBMaskAccessor> pOTBMaskAccessor = boost::make_shared<rttb::masks::OTBMaskAccessor>(spRtstr,geoInf, rtdose);
		  MaskAcessorPointer spMaskAccessor(pOTBMaskAccessor);
          boost::shared_ptr<rttb::core::GenericMaskedDoseIterator> spTestMaskedDoseIterator = boost::make_shared<rttb::core::GenericMaskedDoseIterator>(spMaskAccessor, spDoseAccessor2); //using Mask
		  DoseIteratorPointer mddit(spTestMaskedDoseIterator);
		  boost::shared_ptr<rttb::core::GenericDoseIterator> spTestDoseIterator = boost::make_shared<rttb::core::GenericDoseIterator>(spDoseAccessor2); 
		  DoseIteratorPointer ddit2(spTestDoseIterator);
          mddit->reset();
		  std::cout << "Initialization time: "<<(clock()-startLoop)/1000<<" s"<<std::endl;

          rttb::core::DVHCalculator calc2(mddit,spRtstr->getUID(),mddit->getDoseUID());
          rttb::core::DVH dvh2=*(calc2.getDVH());
		  std::cout <<"DVH(mask) max: "<< dvh2.getMaximum()<< "in "<<spRtstr->getVolume()<<" cm^3"<<std::endl;

		  rttb::core::DVHCalculator calc(ddit2,spRtstr->getUID(),ddit2->getDoseUID());
          rttb::core::DVH dvh=*(calc.getDVH());
		  std::cout <<"DVH(dose) max: "<< dvh.getMaximum()<<std::endl;

		  
		  //test statistics here? -> ../algorithms
		  clock_t startStats(clock());
		  std::cout <<"Statistics: "<<std::endl;
          rttb::algorithms::DoseStatistics stat(mddit);
		  std::cout << "Statistics time: "<<(clock()-startStats)<<" s"<<std::endl;
		  std::cout <<"max: "<< stat.getMaximum(resultListMax) <<std::endl;
		  std::cout <<"mean: "<< stat.getMean() <<std::endl;
		  std::cout <<"min: "<< stat.getMinimum(resultListMin) <<std::endl;
          std::cout <<"std: "<< stat.getStdDeviation() <<std::endl;
		  std::cout <<"var: "<< stat.getVariance() <<std::endl;
        }
      }
      finish = clock();

      std::cout << "Calculation time: "<<(finish-start)/60000<<" min"<<std::endl;




      RETURN_AND_REPORT_TEST_SUCCESS;


    }




  }//testing
}//rttb

