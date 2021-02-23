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

#include <dcmtk/dcmrt/drtdose.h>

#include <boost/lexical_cast.hpp>

#include <cstdlib>

#include "rttbDicomDoseAccessor.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbDcmrtException.h"
#include "rttbIndexOutOfBoundsException.h"

namespace rttb
{
	namespace io
	{
		namespace dicom
		{

			DicomDoseAccessor::~DicomDoseAccessor()
			= default;

			DicomDoseAccessor::DicomDoseAccessor(DRTDoseIODPtr aDRTDoseIODP, DcmItemPtr aDcmDataset)
			{

				_dose = aDRTDoseIODP;
				_dataSet = aDcmDataset;

				OFString uid;
				_dose->getSeriesInstanceUID(uid);
				_doseUID = uid.c_str();

				this->begin();
			}

			bool DicomDoseAccessor::begin()
			{

				assembleGeometricInfo();

				doseData.clear();

				OFString doseGridScalingStr;
				this->_dose->getDoseGridScaling(doseGridScalingStr);

				try
				{
					_doseGridScaling = boost::lexical_cast<double>(doseGridScalingStr.c_str());
				}
				catch (boost::bad_lexical_cast&)
				{
					throw core::InvalidDoseException("Dose grid scaling not readable or = 0!") ;
				}

				OFCondition status;

				unsigned long count;
				const Uint16* pixelData;
				status = _dataSet->findAndGetUint16Array(DcmTagKey(0x7fe0, 0x0010), pixelData, &count);

				if (status.good())
				{
					for (unsigned int i = 0; i < static_cast<unsigned int>(this->_geoInfo.getNumberOfVoxels()); i++)
					{
						this->doseData.push_back(pixelData[i]);
					}

					return true;
				}
				else
				{
					throw io::dicom::DcmrtException("Read Pixel Data (7FE0,0010) failed!");
				}



			}

      void DicomDoseAccessor::assembleGeometricInfo()
			{

				Uint16 temp = 0;
				this->_dose->getColumns(temp);
				_geoInfo.setNumColumns(temp);

				temp  = 0;
				this->_dose->getRows(temp);
				_geoInfo.setNumRows(temp);

				if (_geoInfo.getNumColumns() == 0 || _geoInfo.getNumRows() == 0)
				{
					throw core::InvalidDoseException("Empty dicom dose!") ;
				}

				OFString numberOfFramesStr;
				OFString imageOrientationRowX, imageOrientationRowY, imageOrientationRowZ;
				OFString imageOrientationColumnX, imageOrientationColumnY, imageOrientationColumnZ;
				WorldCoordinate3D imageOrientationRow;
				WorldCoordinate3D imageOrientationColumn;

				try
				{
					this->_dose->getNumberOfFrames(numberOfFramesStr);
					_geoInfo.setNumSlices(boost::lexical_cast<VoxelGridDimensionType>(numberOfFramesStr.c_str()));


					_dose->getImageOrientationPatient(imageOrientationRowX, 0);
					_dose->getImageOrientationPatient(imageOrientationRowY, 1);
					_dose->getImageOrientationPatient(imageOrientationRowZ, 2);
					_dose->getImageOrientationPatient(imageOrientationColumnX, 3);
					_dose->getImageOrientationPatient(imageOrientationColumnY, 4);
					_dose->getImageOrientationPatient(imageOrientationColumnZ, 5);

					imageOrientationRow(0) = boost::lexical_cast<WorldCoordinate>(imageOrientationRowX.c_str());
					imageOrientationRow(1) = boost::lexical_cast<WorldCoordinate>(imageOrientationRowY.c_str());
					imageOrientationRow(2) = boost::lexical_cast<WorldCoordinate>(imageOrientationRowZ.c_str());

					imageOrientationColumn(0) = boost::lexical_cast<WorldCoordinate>(imageOrientationColumnX.c_str());
					imageOrientationColumn(1) = boost::lexical_cast<WorldCoordinate>(imageOrientationColumnY.c_str());
					imageOrientationColumn(2) = boost::lexical_cast<WorldCoordinate>(imageOrientationColumnZ.c_str());
				}
				catch (boost::bad_lexical_cast&)
				{
					throw core::InvalidDoseException("boost::lexical_cast failed! Empty dicom dose!") ;
				}

				/*Get orientation*/
				OrientationMatrix orientation;
				orientation(0, 0) = imageOrientationRow.x();
				orientation(1, 0) = imageOrientationRow.y();
				orientation(2, 0) = imageOrientationRow.z();
				orientation(0, 1) = imageOrientationColumn.x();
				orientation(1, 1) = imageOrientationColumn.y();
				orientation(2, 1) = imageOrientationColumn.z();

				WorldCoordinate3D perpendicular = imageOrientationRow.cross(imageOrientationColumn);
				orientation(0, 2) = perpendicular.x();
				orientation(1, 2) = perpendicular.y();
				orientation(2, 2) = perpendicular.z();

				_geoInfo.setOrientationMatrix(orientation);

				OFString imagePositionX, imagePositionY, imagePositionZ;
				_dose->getImagePositionPatient(imagePositionX, 0);
				_dose->getImagePositionPatient(imagePositionY, 1);
				_dose->getImagePositionPatient(imagePositionZ, 2);

				WorldCoordinate3D imagePositionPatient;

				try
				{
					imagePositionPatient(0) = boost::lexical_cast<WorldCoordinate>(imagePositionX.c_str());
					imagePositionPatient(1) = boost::lexical_cast<WorldCoordinate>(imagePositionY.c_str());
					imagePositionPatient(2) = boost::lexical_cast<WorldCoordinate>(imagePositionZ.c_str());
				}
				catch (boost::bad_lexical_cast&)
				{
					throw core::InvalidDoseException("Can not read image position X/Y/Z!") ;
				}

				_geoInfo.setImagePositionPatient(imagePositionPatient);

				/*Get spacing*/
				SpacingVectorType3D spacingVector;
				OFString pixelSpacingRowStr, pixelSpacingColumnStr, sliceThicknessStr;
				_dose->getPixelSpacing(pixelSpacingRowStr, 0);
				_dose->getPixelSpacing(pixelSpacingColumnStr, 1);

				try
				{
					spacingVector(1) = boost::lexical_cast<GridVolumeType>(pixelSpacingRowStr.c_str());
					spacingVector(0) = boost::lexical_cast<GridVolumeType>(pixelSpacingColumnStr.c_str());
				}
				catch (boost::bad_lexical_cast&)
				{
					throw core::InvalidDoseException("Can not read Pixel Spacing Row/Column!") ;
				}

				_geoInfo.setSpacing(spacingVector);

				if (_geoInfo.getSpacing()(0) == 0 || _geoInfo.getSpacing()(1) == 0)
				{
					throw core::InvalidDoseException("Pixel spacing is 0!");
				}

				_dose->getSliceThickness(sliceThicknessStr);

				try
				{
					spacingVector(2) = boost::lexical_cast<GridVolumeType>(sliceThicknessStr.c_str());
				}
				catch (boost::bad_lexical_cast&)
				{
					spacingVector(2) = 0 ;
				}

				if (spacingVector(2) == 0)
				{
					OFVector<Float64> gridFrameOffsetVector;
					_dose->getGridFrameOffsetVector(gridFrameOffsetVector);

					if (gridFrameOffsetVector.size() >= 2)
					{
						spacingVector(2) = gridFrameOffsetVector.at(1) - gridFrameOffsetVector.at(
						                       0); //read slice thickness from GridFrameOffsetVector (3004,000c)
					}

					if (spacingVector(2) == 0)
					{
						OFCondition status;
						DcmItem doseitem;
						OFString pixelSpacingBetweenSlices;

						status = _dose->write(doseitem);

						if (status.good())
						{

							status = doseitem.findAndGetOFString(DcmTagKey(0x0018, 0x0088), pixelSpacingBetweenSlices);

							try
							{
								spacingVector(2) = boost::lexical_cast<GridVolumeType>
								                   (pixelSpacingBetweenSlices.c_str());//read slice thickness from PixelSpacingBetweenSlices (0018,0088)
							}
							catch (boost::bad_lexical_cast&)
							{
								spacingVector(2) = 0 ;
							}
						}


						//if no useful tags to compute slicing -> set slice thickness to spacingVector(0)
						if (spacingVector(2) == 0)
						{
							std::cerr << "sliceThickness == 0! It wird be replaced with pixelSpacingRow=" <<
							          _geoInfo.getSpacing()(0)
							          << "!" << std::endl;
							spacingVector(2) = spacingVector(0);
						}


					}
				}

				_geoInfo.setSpacing(spacingVector);
			}

			GenericValueType DicomDoseAccessor::getValueAt(const VoxelGridID aID) const
			{
				return doseData.at(aID) * _doseGridScaling;
			}

			GenericValueType DicomDoseAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
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

		}
	}
}
