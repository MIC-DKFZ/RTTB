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

#include <math.h>
#include <algorithm>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"
#include "rttbStructure.h"
#include "rttbVirtuosFileStructureSetGenerator.h"


namespace rttb
{
	namespace io
	{
		namespace virtuos
		{

			VirtuosFileStructureSetGenerator::VirtuosFileStructureSetGenerator(FileNameType aVirtuosVDXFileName,
			        FileNameType aVirtuosCTXFileName)
				: _pPointerOnVirtuosCube(new Cubeinfo*), _patient(NULL)
			{
				_VDXFileName = aVirtuosVDXFileName;
				_CTXFileName = aVirtuosCTXFileName;

				//check if file names are valid
				if (!boost::filesystem::exists(_VDXFileName))
				{
					throw core::InvalidParameterException("invalid vdx file name");
				}

				if (!boost::filesystem::exists(_CTXFileName))
				{
					throw core::InvalidParameterException("invalid ctx file name");
				}

				*_pPointerOnVirtuosCube = create_cubeinfo(0);

				this->initializeVirtuosCube(_CTXFileName);

			}


			VirtuosFileStructureSetGenerator::~VirtuosFileStructureSetGenerator()
			{
				if (this->_patient != NULL)
				{
					delete this->_patient;
				}

				closecube((*(this->_pPointerOnVirtuosCube)));
				nc_free_cubeinfo((*(this->_pPointerOnVirtuosCube)));
				delete this->_pPointerOnVirtuosCube;
			}

			void VirtuosFileStructureSetGenerator::importStructureSet(FileNameType aVirtuosVDXFileName,
			        FileNameType aVirtuosCTXFileName)
			{
				//check file name
				if (aVirtuosCTXFileName.empty() || aVirtuosVDXFileName.empty())
				{
					throw core::InvalidParameterException("Virtuos VDX/CTX file name must not be empty!");
				}

				size_t vdxPosition = aVirtuosVDXFileName.find(".vdx");

				if (vdxPosition == std::string::npos)
				{
					throw core::InvalidParameterException("Virtuos VDX file name must be *.vdx!");
				}

				//get patientFileName, patientDataPath for Virtuos function
				std::string patientFileName, patientName, patientDataPath;
				patientFileName.assign(aVirtuosVDXFileName, aVirtuosVDXFileName.find_last_of("/") + 1,
				                       aVirtuosVDXFileName.length());
				patientName.assign(patientFileName, 0, patientFileName.find_first_of("."));
				patientDataPath.assign(aVirtuosVDXFileName, 0, aVirtuosVDXFileName.find_last_of("/") + 1);

				//Virtuos: voi create voi model
				int errorcode = voi_create_voi_model_dirolab(patientName.c_str(), patientDataPath.c_str(), 0,
				                this->_patient);

				if (errorcode != 0)
				{
					//throw std::string ("Virtuos Routines unable to create VOI Model! ");
					std::cerr << "voi_create_voi_model_dirolab error: error code " << errorcode << std::endl;
				}

				//Virtuos: voi read vdx
				errorcode = voi_read_vdx_version_2_for_DIROlab(patientFileName.c_str(), patientDataPath.c_str(),
				            this->_patient);

				if (errorcode != 0)
				{
					//throw std::string ("voi_read_vdx_version_2_for_DIROlab failed! ");
					std::cerr << "voi_read_vdx_version_2_for_DIROlab error: error code " << errorcode << std::endl;
				}

				int numberOfVois = this->_patient->getNumberOfVois();

				float firstSliceInFrame = (*_pPointerOnVirtuosCube)->pos_list[0].position;

				float sliceThickness = (*_pPointerOnVirtuosCube)->slicedist;

				float lastSliceInFrame = static_cast<float>(((*_pPointerOnVirtuosCube)->dimz - 1) * sliceThickness + firstSliceInFrame);

				for (int currentVoiNumber = 0; currentVoiNumber < numberOfVois; currentVoiNumber++)
				{
					std::string voiName = "";
					char tmpVoiName[1024];
					voi_get_voi_name_by_index_dirolab(currentVoiNumber, 1024, tmpVoiName, this->_patient);
					voiName.assign(tmpVoiName);

					/* prepare contour extraction */
					D3PointList* contours = NULL;
					contours = d3_list_create(1000000);
					D3Point origin = {0, 0, 0}, y_axis_point = {0, 0, 0}, x_axis_point = {0, 0, 0};
					int maxNumberOfContours = 100000;
					int* pNoOFContours = &maxNumberOfContours;

					PolygonSequenceType polygonSequence;

					for (float z = firstSliceInFrame; z <= lastSliceInFrame; z += sliceThickness)
					{

						origin.x = 0.0;
						origin.y = 0.0;
						origin.z = z ;

						x_axis_point.x = 1.0;
						x_axis_point.y = 0.0;
						x_axis_point.z =  z ;

						y_axis_point.x = 0.0;
						y_axis_point.y = 1.0;
						y_axis_point.z = z;
						*pNoOFContours = 100000; //<-- reason is the next function call

						voi_get_CT_contours_dirolab(voiName.c_str(), origin,  x_axis_point, y_axis_point, pNoOFContours,
						                            &contours, 1,
						                            this->_patient);

						for (int i = 0; i < *pNoOFContours; i++)
						{
							PolygonType polygon;

							for (int j = 0; j < contours[i].used - 1;
							     j++) //Virtuos polygon the last point is the same as the first point
							{
								WorldCoordinate3D point;
								point(0) = contours[i].points[j].x;
								point(1) = contours[i].points[j].y;
								point(2) = contours[i].points[j].z;
								polygon.push_back(point);

							}//end for j

							polygonSequence.push_back(polygon);

						}//end for i

					}//end for z

					boost::shared_ptr<core::Structure> spStruct = boost::make_shared<core::Structure>(polygonSequence);
					spStruct->setLabel(voiName);
					_strVector.push_back(spStruct);
				}//end for currentVoiNumber

			}

			void VirtuosFileStructureSetGenerator::initializeVirtuosCube(FileNameType aVirtuosCTXFileName)
			{
				if (aVirtuosCTXFileName.empty())
				{
					throw core::InvalidParameterException("Empty File Name");
				}

				size_t gzPosition = aVirtuosCTXFileName.find(".gz");

				if (gzPosition != std::string::npos)
				{
					aVirtuosCTXFileName.erase(gzPosition, aVirtuosCTXFileName.length());
				}

				nc_init_cubeinfo(*_pPointerOnVirtuosCube);

				int opencubeErrorCode = opencube(aVirtuosCTXFileName.c_str() , *_pPointerOnVirtuosCube);

				if (opencubeErrorCode != 0)
				{
					std::stringstream opencubeErrorCodeAsStringStream;

					opencubeErrorCodeAsStringStream << opencubeErrorCode;

					throw core::InvalidParameterException(std::string(
					        std::string("VirtuosIO::openCube returned error Code: ") +
					        opencubeErrorCodeAsStringStream.str()));
				}

				if ((*_pPointerOnVirtuosCube)->dimx == 0 || (*_pPointerOnVirtuosCube)->dimy == 0
				    || (*_pPointerOnVirtuosCube)->dimz == 0)
				{
					throw core::InvalidParameterException("Invalid Cube dimension: dimX/dimY/dimZ must not be zero! ");
				}
			}

			VirtuosFileStructureSetGenerator::StructureSetPointer
			VirtuosFileStructureSetGenerator::generateStructureSet()
			{

				this->importStructureSet(_VDXFileName, _CTXFileName);

				return boost::make_shared<core::StructureSet>(_strVector);
			}

		}//end namespace virtuos
	}//end namespace io
}//end namespace rttb
