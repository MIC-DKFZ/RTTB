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
#include <boost/lexical_cast.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"

#include "rttbDicomHelaxFileDoseAccessorGenerator.h"
#include "rttbDicomHelaxDoseAccessor.h"
#include "rttbInvalidDoseException.h"
#include "rttbDcmrtException.h"
#include "rttbIndexOutOfBoundsException.h"
#include "rttbInvalidParameterException.h"

namespace rttb{
	namespace io{
		namespace helax{
			DicomHelaxFileDoseAccessorGenerator::~DicomHelaxFileDoseAccessorGenerator(){}

			DicomHelaxFileDoseAccessorGenerator::DicomHelaxFileDoseAccessorGenerator(FileNameType aDICOMRTDoseDirName){
				_doseDirName=aDICOMRTDoseDirName;
				

			}

			core::DoseAccessorGeneratorInterface::DoseAccessorPointer DicomHelaxFileDoseAccessorGenerator::generateDoseAccessor() {
				boost::filesystem::path path=boost::filesystem::path(_doseDirName);

				OFCondition status;

				DcmFileFormat fileformat;

				std::vector<DRTDoseIODPtr> doseVector;
				IDType doseUID;

				int file_count=0;
				if(!boost::filesystem::exists(path) || !boost::filesystem::is_directory(path)){
					throw core::InvalidParameterException("Directory not found!");
				}
				else{
					boost::filesystem::directory_iterator end_iter;
					bool isFirstDose=true;

					for(boost::filesystem::directory_iterator dir_itr(path);dir_itr!=end_iter;++dir_itr)
					{
						if(boost::filesystem::is_regular_file(dir_itr->status()))
						{								
								boost::filesystem::path filePath(dir_itr->path().filename().string());
								filePath=boost::filesystem::system_complete(dir_itr->path());


								DRTDoseIODPtr dose= boost::make_shared<DRTDoseIOD>();

								status = fileformat.loadFile(filePath.string().c_str());
								if (!status.good())
								{
									throw core::InvalidDoseException("Error: load dose fileformat.loadFile failed!");
								}
              
								status = dose->read(*fileformat.getDataset());
								if(!status.good())
								{
									throw core::InvalidDoseException("Error: read DRTDoseIOD failed!");
								}

                OFString modality;
                status = dose->getModality(modality);
                if (modality != "RTDOSE")
                {
                  std::cout<<"Error: "<<filePath.string().c_str()<<" is no dose file"<<std::endl;
                  continue;
                }
                else
                {
                  ++file_count;
                }

								OFString uid;
								dose->getSeriesInstanceUID(uid);

								if(file_count==1)
								{
									doseUID=uid.c_str();
								}


								if(doseUID!=uid.c_str())
								{
									std::cout << "There are more than 1 RT dose in the directory. The DoseAccessor returns only the first one! Loaded uid: "<< doseUID<< ";ignored UID: "<< uid <<std::endl;
								}
								else
								{
									doseVector.push_back(dose);
								}

						}
					}

				}


				_doseAccessor=boost::make_shared<io::helax::DicomHelaxDoseAccessor>(doseVector);
				return _doseAccessor;

			}




		}
	}
}
