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

#include "rttbDicomFileReaderHelper.h"

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"

#include "rttbInvalidDoseException.h"
#include "rttbDcmrtException.h"
#include "rttbIndexOutOfBoundsException.h"
#include "rttbInvalidParameterException.h"

namespace rttb{
	namespace io{
		namespace dicom{
			bool isFile(FileNameType aName){
				boost::filesystem::path path=boost::filesystem::path(aName);
				if(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path))
					return true;
				else
					return false;
			}

			bool isDirectory(FileNameType aName){
				boost::filesystem::path path=boost::filesystem::path(aName);
				if(boost::filesystem::exists(path) && boost::filesystem::is_directory(path))
					return true;
				else
					return false;
			}

			OFString getModality(DcmDataSetPtr aDcmDataSet){
				OFString modality;
				OFCondition status;
				status = aDcmDataSet->findAndGetOFString(DcmTagKey(0x0008, 0x0060), modality);
				if (!status.good())
				{
					throw DcmrtException("Error: get modality failed!");
				}
				return modality;
			}


			OFString getUID(DcmDataSetPtr aDcmDataSet){
				OFString uid;
				OFCondition status;
				status = aDcmDataSet->findAndGetOFString(DcmTagKey(0x0020, 0x000e), uid);
				if (!status.good())
				{
					throw DcmrtException("Error: get uid failed!");
				}
				return uid;
			}

			std::vector<FileNameType> getFileNamesWithSameUID(FileNameType aDirName, Modality aModality){
				std::vector<FileNameType> fileNameVector;
				std::string modalityStrArray[] = {"RTDOSE", "RTSTRUCT", "RTPLAN"};

				boost::filesystem::path path=boost::filesystem::path(aDirName);

				OFCondition status;
				DcmFileFormat fileformat;

				IDType uid = "";
				DcmDataSetPtr datasetPtr;

				if(aModality.Type != 1 && aModality.Type != 2 && aModality.Type != 3){
					throw core::InvalidParameterException("Error: invalid modality! The modality should be RTDOSE(1)/RTSTRUCT(2)/RTPLAN(3).");
				}

				//if a directory		
				if(isDirectory(aDirName)){

					boost::filesystem::directory_iterator end_iter;
					bool isFirst=true;

					for(boost::filesystem::directory_iterator dir_itr(path);dir_itr!=end_iter;++dir_itr)
					{
						if(boost::filesystem::is_regular_file(dir_itr->status()))
						{								
							boost::filesystem::path filePath(dir_itr->path().filename().string());
							filePath=boost::filesystem::system_complete(dir_itr->path());

							status = fileformat.loadFile(filePath.string().c_str());
							if (!status.good())
							{
								throw DcmrtException("Error: load dose file "+ filePath.string() +" failed!");
							}

							datasetPtr =  boost::make_shared<DcmDataset>(*fileformat.getDataset());
							OFString modalityOFS = getModality(datasetPtr);

							for(unsigned int i=0; i<3; i++){
								if (aModality.Type == (i+1) && modalityOFS == modalityStrArray[i].c_str()) 
								{
									OFString currentUID = getUID(datasetPtr);
									
									//get the first uid of the given modality
									if(isFirst)
									{
										uid = currentUID.c_str();
										isFirst = false;
									}
									if(uid == currentUID.c_str())
									{
										fileNameVector.push_back(filePath.string().c_str());
									}
									break;
								}
							}
						}
					}

				}	
				else if(isFile(aDirName)){
					std::cout << "Important: the given name "+ aDirName +" is a file name, not a directory name. Given modality will be ignored, use the modality of the file." << std::endl;
					fileNameVector = getFileNames(aDirName);
				}
				else{
					throw core::InvalidParameterException("Error: file/directory does not exist!");
				}
				return fileNameVector;

			}

			std::vector<FileNameType> getFileNames(FileNameType aFileName){

				if(!isFile(aFileName)){
					throw core::InvalidParameterException("Error: file does not exist!");
				}

				std::vector<FileNameType> fileNameVector;

				boost::filesystem::path path=boost::filesystem::path(aFileName);

				OFCondition status;
				DcmFileFormat fileformat;
				DcmDataSetPtr datasetPtr;
				OFString modality;//modality of the given file
				OFString uid;//uid of the given file

				status = fileformat.loadFile(aFileName.c_str());
				if (!status.good())
				{
					throw DcmrtException("Error: fileformat.loadFile failed!");
				}

				datasetPtr =  boost::make_shared<DcmDataset>(*fileformat.getDataset());				
				modality = getModality(datasetPtr);
				uid = getUID(datasetPtr);

				//get parent directory
				boost::filesystem::path parentDir = path.parent_path();		
				if(boost::filesystem::is_directory(parentDir)){

					boost::filesystem::directory_iterator end_iter;

					for(boost::filesystem::directory_iterator dir_itr(parentDir);dir_itr!=end_iter;++dir_itr)
					{
						if(boost::filesystem::is_regular_file(dir_itr->status()))
						{								
							boost::filesystem::path currentFilePath(dir_itr->path().filename().string());
							currentFilePath = boost::filesystem::system_complete(dir_itr->path());

							status = fileformat.loadFile(currentFilePath.string().c_str());
							if (!status.good())
							{
								throw DcmrtException("Error: load dose fileformat.loadFile failed!");
							}

							datasetPtr =  boost::make_shared<DcmDataset>(*fileformat.getDataset());
							OFString currentModality = getModality(datasetPtr);
							OFString currentUID = getUID(datasetPtr);

							//if the same modality
							if (modality == currentModality && uid == currentUID){
								fileNameVector.push_back(currentFilePath.string().c_str());
							}

						}


					}
				}
				return fileNameVector;
			}
		}
	}
}

