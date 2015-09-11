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

			VirtuosDoseAccessor::~VirtuosDoseAccessor()
			{
				if (_freeVirtuosData)
				{
					this->freeVirtuosData();
				}
			}

			VirtuosDoseAccessor::VirtuosDoseAccessor(Cubeinfo* aPointerOnVirtuosCube, bool freeVirtuosData,
			        DoseTypeGy normalizationDose, DoseTypeGy prescribedDose):
				_pPointerOnVirtuosCube(new Cubeinfo*), _freeVirtuosData(freeVirtuosData), _doseGridScaling(0),
				_doseUID(""), _doseScalingFactor(0)
			{
				//initialize cube pointer
				*_pPointerOnVirtuosCube = create_cubeinfo(0);
				*_pPointerOnVirtuosCube = aPointerOnVirtuosCube;


				_prescribedDose = prescribedDose;
				_normalizationDose = normalizationDose;

				if (_prescribedDose == 0)
				{
					_prescribedDose = 1;
				}

				if (_normalizationDose == 0)
				{
					_normalizationDose = 1;
				}

				_doseScalingFactor = _prescribedDose / _normalizationDose;
				std::cout << "Prescribed dose: " << _prescribedDose << std::endl;
				std::cout << "Normalization dose: " << _normalizationDose << std::endl;

				//dose import
				this->begin();
			}

			void VirtuosDoseAccessor::begin()
			{
				if (!_pPointerOnVirtuosCube)
				{
					throw core::NullPointerException(" *_pPointerOnVirtuosCube must not be NULL!");
				}

				assembleGeometricInfo();

				//load data
				if ((*_pPointerOnVirtuosCube)->data_type == 1)
				{
					this->importPixelData<int>();
				}
				else if ((*_pPointerOnVirtuosCube)->data_type == 2)
				{
					this->importPixelData<float>();
				}
				else
				{
					throw core::InvalidParameterException(" cube has wrong data type!");
				}
			}

			template <typename TPixelType>
			void VirtuosDoseAccessor::importPixelData()
			{
				doseData.clear();

				int dimX = (*_pPointerOnVirtuosCube)->dimx;
				int dimY = (*_pPointerOnVirtuosCube)->dimy;
				int dimZ = (*_pPointerOnVirtuosCube)->dimz;

				GridVolumeType pixelSpacing = (*_pPointerOnVirtuosCube)->pixdist;
				GridVolumeType sliceThickness = (*_pPointerOnVirtuosCube)->slicedist;

				TPixelType** * access_pointer = (TPixelType***)(*_pPointerOnVirtuosCube)->cube_direct_access;

				for (int k = 0; k < dimZ; k++)
				{
					for (int j = 0; j < dimY; j++)
					{
						for (int i = 0; i < dimX; i++)
						{
							TPixelType voxel_value = access_pointer[k][j][i];
							doseData.push_back(voxel_value * _doseScalingFactor);
						}

					}//end for j
				}//end for k
			}

			void VirtuosDoseAccessor::assembleGeometricInfo()
			{
				if (!_pPointerOnVirtuosCube)
				{
					throw core::NullPointerException(" _pPointerOnVirtuosCube must not be NULL!");
				}

				_geoInfo.setNumColumns((*_pPointerOnVirtuosCube)->dimx);

				_geoInfo.setNumRows((*_pPointerOnVirtuosCube)->dimy);

				_geoInfo.setNumSlices((*_pPointerOnVirtuosCube)->dimz);

				if (_geoInfo.getNumColumns() == 0 || _geoInfo.getNumRows() == 0 || _geoInfo.getNumSlices() == 0)
				{
					throw core::InvalidDoseException("Empty Virtuos dose!") ;
				}

				OrientationMatrix orientation;
				_geoInfo.setOrientationMatrix(orientation);

				WorldCoordinate3D imagePositionPatient;
				imagePositionPatient(0) = (*this->_pPointerOnVirtuosCube)->pixdist / 2;
				imagePositionPatient(1) = (*this->_pPointerOnVirtuosCube)->pixdist / 2;

				if (!((*this->_pPointerOnVirtuosCube)->pos_list))
				{
					throw core::InvalidDoseException("Empty Virtuos dose!") ;
				}

				imagePositionPatient(2) = (*this->_pPointerOnVirtuosCube)->pos_list[0].position;

				_geoInfo.setImagePositionPatient(imagePositionPatient);

				SpacingVectorType3D spacingVector;
				spacingVector(0) = (*_pPointerOnVirtuosCube)->pixdist;
				spacingVector(1) = (*_pPointerOnVirtuosCube)->pixdist;
				_geoInfo.setSpacing(spacingVector);

				if (_geoInfo.getPixelSpacingRow() == 0 || _geoInfo.getPixelSpacingColumn() == 0)
				{
					throw core::InvalidDoseException("Pixel spacing = 0!");
				}

				spacingVector(2) = (*_pPointerOnVirtuosCube)->slicedist;

				if (spacingVector(2) == 0)
				{
					std::cerr << "sliceThickness == 0! It will be replaced with pixelSpacingRow=" <<
					          _geoInfo.getPixelSpacingRow()
					          << "!" << std::endl;
					spacingVector(2) = spacingVector(0);
				}

				_geoInfo.setSpacing(spacingVector);
			}


			DoseTypeGy VirtuosDoseAccessor::getValueAt(const VoxelGridID aID) const
			{
				return doseData.at(aID);
			}

			DoseTypeGy VirtuosDoseAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
			{
				VoxelGridID aVoxelGridID;

				if (_geoInfo.convert(aIndex, aVoxelGridID))
				{
					return getValueAt(aVoxelGridID);
				}
				else
				{
					return -1;
				}
			}

			void VirtuosDoseAccessor::freeVirtuosData()
			{
				if (*(this->_pPointerOnVirtuosCube) != NULL)
				{
					closecube((*(this->_pPointerOnVirtuosCube)));
					nc_free_cubeinfo((*(this->_pPointerOnVirtuosCube)));
					delete this->_pPointerOnVirtuosCube;

					// initialize attributes again
					//this->_pPointerOnVirtuosCube = new Cubeinfo*;
					//*(this->_pPointerOnVirtuosCube) = create_cubeinfo(0);
				}

			}

		}
	}
}

