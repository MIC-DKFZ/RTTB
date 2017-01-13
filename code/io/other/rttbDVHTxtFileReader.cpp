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

#include <boost/make_shared.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "rttbDVHTxtFileReader.h"
#include "rttbInvalidParameterException.h"
#include "rttbBaseType.h"

namespace rttb
{
	namespace io
	{
		namespace other
		{

			DVHTxtFileReader::DVHTxtFileReader(FileNameString aFileName)
			{
				_fileName = aFileName;
				_resetFile = true;
			}

			DVHTxtFileReader::~DVHTxtFileReader() {}

			void DVHTxtFileReader::resetFileName(FileNameString aFileName)
			{
				_fileName = aFileName;
				_resetFile = true;
			}

			void DVHTxtFileReader::createDVH()
			{
				std::ifstream dvh_ifstr(this->_fileName.c_str(), std::ios::in);

				std::string structureLabel;
				std::string dvhType;
				unsigned int numberOfBins;
				DoseTypeGy prescribedDose=-1;
				double estimated_max_dose_prescribed_dose_ratio=1.0;
				std::deque<DoseTypeGy> dataDifferential;
				std::deque<DoseTypeGy> dataCumulative;

				DoseTypeGy deltaD = 0;
				DoseVoxelVolumeType deltaV = 0;
				IDType strID;
				IDType doseID;	

				enum dataTypes { _deltaV, _deltaD, _strID, _doseID, _numberOfBins, _dvhType, _prescribedDose, _estimated_max_dose_prescribed_dose_ratio};
				std::map<std::string, dataTypes> mapTypes;
				
				mapTypes["DeltaV"] = _deltaV;
				mapTypes["DeltaD"] = _deltaD;
				mapTypes["StructureID"] = _strID;
				mapTypes["DoseID"] = _doseID;
				mapTypes["Number of bins"] = _numberOfBins;
				mapTypes["DVH Type"] = _dvhType;
				mapTypes["Prescribed Dose"] = _prescribedDose;
				mapTypes["Estimated_max_dose_prescribed_dose_ratio"] = _estimated_max_dose_prescribed_dose_ratio;

				bool isDifferential = false;

				if (!dvh_ifstr.is_open())
				{
					throw  core::InvalidParameterException("DVH file name invalid: could not open the dvh file!");
				}
				else
				{
					bool data_begin = false;

					while (!dvh_ifstr.eof())
					{
						std::string line;
						std::getline(dvh_ifstr, line);

						if (!data_begin)
						{
							std::vector<std::string> buffer;
							boost::split(buffer, line, boost::is_any_of(":"));

							if (buffer.size() != 2)
							{
								throw core::InvalidParameterException("Error while splitting the line...");
							}

							if (buffer.at(0) == "DVH Data")
							{
								data_begin = true;
							}
							else
							{
								std::string key = buffer.at(0);
								std::string value = buffer.at(1);

								boost::trim(key);
								boost::trim(value);

								switch (mapTypes.at(key))
								{
								case _deltaD:	
									deltaD = boost::lexical_cast<DoseTypeGy>(value); 
									break;
								case _deltaV:
									deltaV = boost::lexical_cast<DoseVoxelVolumeType>(value);
									break;
								case _strID:
									strID = value;
									break;
								case _doseID:
									doseID = value;
									break;
								case _numberOfBins:
									numberOfBins = boost::lexical_cast<unsigned int>(value);
								case _dvhType:
									dvhType = value;
									if (dvhType == "DIFFERENTIAL")
									{
										isDifferential = true;
									}
									break;
								case _prescribedDose:
									prescribedDose = boost::lexical_cast<DoseTypeGy>(value);
									break;
								case _estimated_max_dose_prescribed_dose_ratio:
									estimated_max_dose_prescribed_dose_ratio = boost::lexical_cast<double>(value);
									break;
								}
							}
						}
						else
						{
							if (line.empty())
							{
								break;
							}
							else
							{
								if (isDifferential)
								{
									loadData(line, dataDifferential);
								}
								else
								{
									loadData(line, dataCumulative);
								}
							}
						}	
					}
				}

				numberOfBins = static_cast<unsigned int>(std::max(dataDifferential.size(), dataCumulative.size()));

				if (numberOfBins == 0)
				{
					throw  core::InvalidParameterException("Invalid dvh file: empty dvh data!");
				}

				if (!isDifferential)
				{
					calculateDataCumulative(dataCumulative, dataDifferential, numberOfBins);
				}
				
				if (deltaD == 0)
				{
					deltaD = prescribedDose * estimated_max_dose_prescribed_dose_ratio / numberOfBins;

					if (deltaD == 0)
					{
						throw  core::InvalidParameterException("Invalid dvh file: deltaD or deltaV must not be zero!");
					}
				}
				
				if (deltaV == 0)
				{
					deltaV = 0.027;
				}

				_dvh = boost::make_shared<core::DVH>(dataDifferential, deltaD, deltaV, strID, doseID);
				_resetFile = false;
			}
			
			DVHTxtFileReader::DVHPointer DVHTxtFileReader::generateDVH()
			{
				if (_resetFile)
				{
					this->createDVH();
				}

				return _dvh;
			}

			void DVHTxtFileReader::calculateDataCumulative(std::deque<DoseTypeGy>& dataCumulative, 
				std::deque<DoseTypeGy>& dataDifferential, unsigned int numberOfBins)
			{
				DoseCalcType differentialDVHi = 0;
				std::deque<DoseCalcType>::iterator it;

				for (it = dataCumulative.begin(); it != dataCumulative.end(); ++it)
				{
					if (dataDifferential.size() == numberOfBins - 1)
					{
						differentialDVHi = *it;
					}
					else
					{
						differentialDVHi = *it - *(it + 1);
					}

					dataDifferential.push_back(differentialDVHi);
				}
			}

			void DVHTxtFileReader::loadData(const std::string& line, std::deque<DoseTypeGy>& data)
			{
				std::vector<std::string> dataBuffer;
				boost::split(dataBuffer, line, boost::is_any_of(","));

				if (dataBuffer.size() != 2)
				{
					throw core::InvalidParameterException("Error while splitting the line...");
				}

				boost::trim(dataBuffer.at(1));

				DoseCalcType dvh_i = boost::lexical_cast<DoseCalcType>(dataBuffer.at(1));

				data.push_back(dvh_i);
			}
		}//end namespace other
	}//end namespace io
}//end namespace rttb

