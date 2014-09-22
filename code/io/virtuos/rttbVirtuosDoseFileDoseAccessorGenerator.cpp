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


#include "rttbVirtuosDoseFileDoseAccessorGenerator.h"
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

			VirtuosDoseFileDoseAccessorGenerator::~VirtuosDoseFileDoseAccessorGenerator()
			{
			}


			VirtuosDoseFileDoseAccessorGenerator::VirtuosDoseFileDoseAccessorGenerator(
			    const FileNameType aVirtuosDoseFileName, DoseTypeGy normalizationDose, DoseTypeGy prescribedDose):
				_pPointerOnVirtuosCube(new Cubeinfo*)
			{
				_doseFileName = aVirtuosDoseFileName;
				_normalizationDose = normalizationDose;
				_prescribedDose = prescribedDose;


			}


			void VirtuosDoseFileDoseAccessorGenerator::initializeVirtuosCube(FileNameType aVirtuosDoseFileName)
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

			VirtuosDoseFileDoseAccessorGenerator::DoseAccessorPointer
			VirtuosDoseFileDoseAccessorGenerator::generateDoseAccessor()
			{
				//check if file names are valid
				if (!boost::filesystem::exists(_doseFileName))
				{
					throw core::InvalidParameterException("invalid dose file name");
				}

				//initialize cube pointer
				*_pPointerOnVirtuosCube = create_cubeinfo(0);

				if (!(_doseFileName.empty()))
				{
					//initialize dose import

					this->initializeVirtuosCube(_doseFileName);

					if (_prescribedDose == 0)
					{
						_prescribedDose = 1;
					}

					if (_normalizationDose == 0)
					{
						_normalizationDose = 1;
					}

					_doseAccessor = boost::make_shared<io::virtuos::VirtuosDoseAccessor>(*_pPointerOnVirtuosCube, true,
					                _normalizationDose, _prescribedDose);


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

