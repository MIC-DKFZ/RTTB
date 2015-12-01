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

#include <ostream>
#include <fstream>

#include "rttbDVHTxtFileWriter.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbException.h"

namespace rttb
{
	namespace io
	{
		namespace other
		{

			DVHTxtFileWriter::DVHTxtFileWriter(FileNameString aFileName, DVHType aDVHType)
			{
				this->setFileName(aFileName);
				this->setDVHType(aDVHType);
			}

			void DVHTxtFileWriter::setDVHType(DVHType aDVHType)
			{
				_dvhType = aDVHType;
			}

			FileNameString DVHTxtFileWriter::getFileName() const
			{
				return _fileName;
			}

			void DVHTxtFileWriter::setFileName(FileNameString aFileName)
			{
				_fileName = aFileName;
			}

			DVHType DVHTxtFileWriter::getDVHType() const
			{
				return _dvhType;
			}

			void DVHTxtFileWriter::writeDVH(DVHPointer aDvh)
			{
				if (!aDvh)
				{
					throw core::NullPointerException("aDvh must not be NULL! ");
				}

				std::ofstream out_dvh_ofstream(this->_fileName.c_str(), std::ios::out);

				if (!out_dvh_ofstream.is_open() || !out_dvh_ofstream.good())
				{
					throw core::InvalidParameterException("Invalid dvh file name: could not open write file");
				}
				else
				{
					//setting string stream precission explicitly is mandatory to guarantee that tests
					//run sucessfully on different systems!
					out_dvh_ofstream.precision(10);

					if (_dvhType.Type != DVHType::Differential && _dvhType.Type != DVHType::Cumulative)
					{
						throw core::InvalidParameterException("DVH Type not acceptable: Only: DIFFERENTIAL/CUMULATIVE!");
					}

					if (_dvhType.Type == DVHType::Differential)
					{
						out_dvh_ofstream << "DVH Type: DIFFERENTIAL\n";
					}
					else if (_dvhType.Type == DVHType::Cumulative)
					{
						out_dvh_ofstream << "DVH Type: CUMULATIVE\n";
					}

					out_dvh_ofstream << "DeltaD: " << aDvh->getDeltaD() << "\n";
					out_dvh_ofstream << "DeltaV: " << aDvh->getDeltaV() << "\n";
					out_dvh_ofstream << "StructureID: " << aDvh->getStructureID() << "\n";
					out_dvh_ofstream << "DoseID: " << aDvh->getDoseID() << "\n";
					out_dvh_ofstream << "DVH Data: " << "\n";

					if (_dvhType.Type == DVHType::Differential)
					{
						DataDifferentialType dataDifferential = aDvh->getDataDifferential();
						int numberOfBins = dataDifferential.size();

						for (int i = 0; i < numberOfBins; i++)
						{
							out_dvh_ofstream << i << ","  <<  dataDifferential[i] << "\n";
						}
					}
					else if (_dvhType.Type == DVHType::Cumulative)
					{
						DataDifferentialType dataCumulative = aDvh->calcCumulativeDVH();
						int numberOfBins = dataCumulative.size();

						for (int i = 0; i < numberOfBins; i++)
						{
							out_dvh_ofstream << i << "," << dataCumulative[i] << "\n";
						}
					}


				}
			}
		}
	}
}