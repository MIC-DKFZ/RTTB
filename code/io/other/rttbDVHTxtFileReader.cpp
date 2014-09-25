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

#include <istream>
#include <sstream>
#include <fstream>

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include "rttbDVHTxtFileReader.h"
#include "rttbInvalidParameterException.h"
#include "rttbBaseType.h"

namespace rttb{
	namespace io{
		namespace other{

			DVHTxtFileReader::DVHTxtFileReader(FileNameString aFileName){
				_fileName=aFileName;
				_resetFile=true;
			}

			DVHTxtFileReader::~DVHTxtFileReader(){}

			void DVHTxtFileReader::resetFileName(FileNameString aFileName){
				_fileName=aFileName;
				_resetFile=true;
			}

			void DVHTxtFileReader::createDVH(){
				std::ifstream dvh_ifstr(this->_fileName.c_str(),std::ios::in);

				std::string structureLabel;
				std::string dvhType;
				int numberOfBins;
				DoseTypeGy prescribedDose;
				double _estimated_max_dose_prescribed_dose_ratio;
				int voxelsInStructure;
				std::deque<DoseTypeGy> dataDifferential;
				std::deque<DoseTypeGy> dataCumulative;

				DoseTypeGy deltaD=0;
				DoseVoxelVolumeType deltaV=0;
				IDType strID;
				IDType doseID;

				if ( !dvh_ifstr.is_open() ) 
				{
					throw  core::InvalidParameterException("DVH file name invalid: could not open the dvh file!");
				}
				else {
					bool data_begin=false;

					while(!dvh_ifstr.eof())
					{
						std::string line;
						std::getline(dvh_ifstr,line);

						if(line.find("DVH Data:")!= std::string::npos)
							data_begin=true;
						if(data_begin && (line.find(",")!= std::string::npos))
						{				
							std::stringstream ss(line.substr(line.find(",")+1));
							DoseCalcType dvh_i;
							ss >> dvh_i;
							if(dvhType=="DIFFERENTIAL")
							{
								dataDifferential.push_back(dvh_i);
							}
							else if(dvhType=="CUMULATIVE")
							{
								dataCumulative.push_back(dvh_i);
							}
						}
						if(line.find("DeltaD: ")!= std::string::npos)
						{
							std::stringstream ss(line.substr(8));
							ss >> deltaD;
						}
						if(line.find("DeltaV: ")!= std::string::npos)
						{
							std::stringstream ss(line.substr(8));
							ss >> deltaV;
						}
						if(line.find("StructureID: ")!= std::string::npos)
						{
							std::stringstream ss(line.substr(13));
							ss >> strID;
						}
						if(line.find("DoseID: ")!= std::string::npos)
						{
							std::stringstream ss(line.substr(8));
							ss >> doseID;
						}
						if(line.find("Number of bins: ")!= std::string::npos)
						{
							std::stringstream ss(line.substr(16));
							ss >> numberOfBins;
						}
						if(line.find("Structure Label: ")!= std::string::npos)
						{
							std::stringstream ss(line.substr(17));
							ss >> structureLabel;
						}
						if(line.find("DVH Type: ")!= std::string::npos)
						{
							std::stringstream ss(line.substr(10));
							ss >> dvhType;
						}
						if(line.find("Prescribed Dose: ")!=std::string::npos)
						{
							std::stringstream ss(line.substr(17));
							ss >> prescribedDose;
						}
						if(line.find("Estimated_max_dose_prescribed_dose_ratio: ")!=std::string::npos)
						{
							std::stringstream ss(line.substr(42));
							ss >> _estimated_max_dose_prescribed_dose_ratio;
						}
						if(line.find("Voxels In Structure: ")!=std::string::npos)
						{
							std::stringstream ss(line.substr(21));
							ss >> voxelsInStructure;
						}
					}
				}

				numberOfBins=std::max(dataDifferential.size(),dataCumulative.size());

				if(dvhType=="CUMULATIVE")
				{
					DoseCalcType differentialDVHi = 0;
					std::deque<DoseCalcType>::iterator it;

					for(it=dataCumulative.begin();it!=dataCumulative.end();it++ )
					{
						if(dataDifferential.size()==numberOfBins-1)
						{
							differentialDVHi=*it;
						}
						else{
							differentialDVHi = *it-*(it+1);
						}
						dataDifferential.push_back(differentialDVHi);
					}
				}
				if(numberOfBins==0)
				{
					throw  core::InvalidParameterException("Invalid dvh file: empty dvh data!");
				}
				if(deltaD==0)
				{
					deltaD=prescribedDose*_estimated_max_dose_prescribed_dose_ratio/numberOfBins;
				}
				if(deltaV==0)
				{
					deltaV=0.027;
				}
				if(deltaD==0 || deltaV==0)
				{
					throw  core::InvalidParameterException("Invalid dvh file: deltaD or deltaV must not be zero!");
				}

				_dvh=boost::make_shared<core::DVH>(dataDifferential,deltaD,deltaV,strID,doseID);
				_resetFile=false;
			}

			DVHTxtFileReader::DVHPointer DVHTxtFileReader::generateDVH(){
				if(_resetFile){
					this->createDVH();
				}
				return _dvh;
			}
		}//end namepsace other
	}//end namespace io
}//end namespace rttb

