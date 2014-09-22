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
#include <boost/filesystem.hpp>

#include "rttbVirtuosPlanFileDoseAccessorGenerator.h"
#include "rttbVirtuosDoseAccessor.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbInvalidParameterException.h"
#include "rttbIndexOutOfBoundsException.h"

//VIRTUOS
#include "pln1file.h"
#include "plt_type.h"
#include "rtp_type.h"


namespace rttb
{
	namespace io
	{
		namespace virtuos
		{

			VirtuosPlanFileDoseAccessorGenerator::~VirtuosPlanFileDoseAccessorGenerator()
			{
			}

			VirtuosPlanFileDoseAccessorGenerator::VirtuosPlanFileDoseAccessorGenerator(
			    const FileNameType aVirtuosDoseFileName, const FileNameType aVirtuosPlanFileName):
				_pPointerOnVirtuosCube(new Cubeinfo*)
			{
				_doseFileName = aVirtuosDoseFileName;
				_planFileName = aVirtuosPlanFileName;

			}



			void VirtuosPlanFileDoseAccessorGenerator::initializeVirtuosCube(FileNameType aVirtuosDoseFileName)
			{
				if (aVirtuosDoseFileName.empty())
				{
					throw core::InvalidParameterException("empty File Name");
				}

				size_t gzPosition = aVirtuosDoseFileName.find(".gz");

				if (gzPosition != std::string::npos)
				{
					aVirtuosDoseFileName.erase(gzPosition, aVirtuosDoseFileName.length());
				}

				nc_init_cubeinfo(*_pPointerOnVirtuosCube);

				int opencubeErrorCode = opencube(aVirtuosDoseFileName.c_str() , *_pPointerOnVirtuosCube);

				if (opencubeErrorCode != 0)
				{
					std::stringstream opencubeErrorCodeAsStringStream;
					opencubeErrorCodeAsStringStream << opencubeErrorCode;
					throw core::InvalidParameterException(std::string("VirtuosIO::openCube returned error Code: ") +
					                                      opencubeErrorCodeAsStringStream.str());
				}


				if ((*_pPointerOnVirtuosCube)->dimx == 0 || (*_pPointerOnVirtuosCube)->dimy == 0
				    || (*_pPointerOnVirtuosCube)->dimz == 0)
				{
					throw core::InvalidParameterException("invalid Cube dimension");
				}

			}

			VirtuosPlanFileDoseAccessorGenerator::DoseAccessorPointer
			VirtuosPlanFileDoseAccessorGenerator::generateDoseAccessor()
			{
				//initialize cube pointer
				*_pPointerOnVirtuosCube = create_cubeinfo(0);

				//check if file names are valid
				if (!boost::filesystem::exists(_doseFileName))
				{
					throw core::InvalidParameterException("invalid dose file name");
				}

				if (!boost::filesystem::exists(_planFileName))
				{
					throw core::InvalidParameterException("invalid plan file name");
				}

				if (!(_doseFileName.empty()))
				{
					//import relevant plan data
					if (_planFileName.empty())
					{
						throw core::InvalidParameterException("empty plan file name");
					}

					size_t plnPosition = _planFileName.find(".pln");

					if (plnPosition == std::string::npos)
					{
						throw core::InvalidParameterException("plan file name invalid!");
					}

					RTPType virtuosPlan;
					rtp_plan_init(&virtuosPlan, RTP_S_DEVICE_COORD_IEC);
					rtp_plan_clear(&virtuosPlan, FALSE, RTP_S_DEVICE_COORD_IEC);
					int error = rtp_plan_read(const_cast<char*>(_planFileName.c_str()), &virtuosPlan);

					if (error)
					{
						std::stringstream expSStr;
						expSStr << "Read virtuos plan: rtp_plan_read returned error code ";
						expSStr << error;
						throw core::InvalidParameterException(expSStr.str());
					}

					DoseTypeGy prescribedDose = virtuosPlan.dose_info.prescr_dose;
					DoseTypeGy normalizationDose = virtuosPlan.dose_info.norm_dose;

					if (prescribedDose == 0)
					{
						prescribedDose = 1;
					}

					if (normalizationDose == 0)
					{
						normalizationDose = 1;
					}


					this->initializeVirtuosCube(_doseFileName);

					_doseAccessor = boost::make_shared<io::virtuos::VirtuosDoseAccessor>(*_pPointerOnVirtuosCube, true,
					                normalizationDose, prescribedDose);

				}
				else
				{
					throw core::InvalidParameterException("empty dose file name");
				}

				return _doseAccessor;
			}


		}
	}
}

