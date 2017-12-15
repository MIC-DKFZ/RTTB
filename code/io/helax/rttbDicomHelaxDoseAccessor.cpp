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

#include "rttbDicomHelaxDoseAccessor.h"

#include "boost/lexical_cast.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/numeric/ublas/matrix.hpp"

#include <cstdlib>


#include "rttbInvalidDoseException.h"
#include "rttbDcmrtException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace io
	{
		namespace helax
		{

			DicomHelaxDoseAccessor::~DicomHelaxDoseAccessor()
			= default;


			DicomHelaxDoseAccessor::DicomHelaxDoseAccessor(std::vector<DRTDoseIODPtr> aDICOMRTDoseVector)
			{
				for (const auto & i : aDICOMRTDoseVector)
				{
					_doseVector.push_back(i);
				}

				this->begin();

			}

			bool DicomHelaxDoseAccessor::begin()
			{
				if (_doseVector.size() == 0)
				{
					throw core::InvalidParameterException(" The size of aDICOMRTDoseVector is 0!");
				}

				assembleGeometricInfo();

				_doseData.clear();

				OFString doseGridScalingStr;
				_doseVector.at(0)->getDoseGridScaling(
				    doseGridScalingStr);//get the first dose grid scaling as _doseGridScaling

				try
				{
					_doseGridScaling = boost::lexical_cast<double>(doseGridScalingStr.c_str());
				}
				catch (boost::bad_lexical_cast&)
				{
					throw core::InvalidDoseException("Dose grid scaling not readable or = 0!") ;
				}

				for (auto dose : _doseVector)
				{
						OFString currentDoseGridScalingStr;
					dose->getDoseGridScaling(currentDoseGridScalingStr);
					double currentDoseGridScaling;

					try
					{
						currentDoseGridScaling = boost::lexical_cast<double>(currentDoseGridScalingStr.c_str());
					}
					catch (boost::bad_lexical_cast&)
					{
						throw core::InvalidDoseException("Dose grid scaling not readable or = 0!") ;
					}

					OFCondition status;
					DcmFileFormat fileformat;
					DcmItem doseitem;

					status = dose->write(doseitem);

					if (status.good())
					{
						unsigned long count;
						const Uint16* pixelData;
						status = doseitem.findAndGetUint16Array(DcmTagKey(0x7fe0, 0x0010), pixelData, &count);

						if (status.good())
						{
							for (unsigned int j = 0;
							     j < static_cast<unsigned int>(_geoInfo.getNumColumns()*_geoInfo.getNumRows()); j++)
							{
								auto data = static_cast<Uint16>(pixelData[j] * currentDoseGridScaling /
								                                  _doseGridScaling);
								this->_doseData.push_back(data); //recalculate dose data
							}

						}
						else
						{
							throw dicom::DcmrtException("Read Pixel Data (7FE0,0010) failed!");
						}
					}
					else
					{
						throw dicom::DcmrtException("Read DICOM-RT Dose file failed!");
					}
				}

				return true;



			}

			bool DicomHelaxDoseAccessor::assembleGeometricInfo()
			{
				DRTDoseIODPtr dose = _doseVector.at(0);

				Uint16 temp = 0;
				dose->getColumns(temp);
				_geoInfo.setNumColumns(temp);

				temp  = 0;
				dose->getRows(temp);
				_geoInfo.setNumRows(temp);

				OFString numberOfFramesStr;
				dose->getNumberOfFrames(numberOfFramesStr);

				if (!numberOfFramesStr.empty())
				{

					_geoInfo.setNumSlices(boost::lexical_cast<int>(numberOfFramesStr.c_str()));
				}
				else
				{
					_geoInfo.setNumSlices((VoxelGridDimensionType)_doseVector.size());
				}

				if (_geoInfo.getNumColumns() == 0 || _geoInfo.getNumRows() == 0 || _geoInfo.getNumSlices() == 0)
				{
					throw core::InvalidDoseException("Empty dicom dose!") ;
				}

				OFString imageOrientationRowX;
				dose->getImageOrientationPatient(imageOrientationRowX, 0);
				OFString imageOrientationRowY;
				dose->getImageOrientationPatient(imageOrientationRowY, 1);
				OFString imageOrientationRowZ;
				dose->getImageOrientationPatient(imageOrientationRowZ, 2);
				OFString imageOrientationColumnX;
				dose->getImageOrientationPatient(imageOrientationColumnX, 3);
				OFString imageOrientationColumnY;
				dose->getImageOrientationPatient(imageOrientationColumnY, 4);
				OFString imageOrientationColumnZ;
				dose->getImageOrientationPatient(imageOrientationColumnZ, 5);
				WorldCoordinate3D imageOrientationRow;
				WorldCoordinate3D imageOrientationColumn;

				try
				{
					imageOrientationRow(0) = boost::lexical_cast<WorldCoordinate>(imageOrientationRowX.c_str());
					imageOrientationRow(1) = boost::lexical_cast<WorldCoordinate>(imageOrientationRowY.c_str());
					imageOrientationRow(2) = boost::lexical_cast<WorldCoordinate>(imageOrientationRowZ.c_str());

					imageOrientationColumn(0) = boost::lexical_cast<WorldCoordinate>(imageOrientationColumnX.c_str());
					imageOrientationColumn(1) = boost::lexical_cast<WorldCoordinate>(imageOrientationColumnY.c_str());
					imageOrientationColumn(2) = boost::lexical_cast<WorldCoordinate>(imageOrientationColumnZ.c_str());
				}
				catch (boost::bad_lexical_cast&)
				{
					throw core::InvalidDoseException("boost::lexical_cast WorldCoordinate failed! Can not read image orientation X/Y/Z!")
					;
				}

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

				OFString imagePositionX;
				dose->getImagePositionPatient(imagePositionX, 0);
				OFString imagePositionY;
				dose->getImagePositionPatient(imagePositionY, 1);
				OFString imagePositionZ;
				dose->getImagePositionPatient(imagePositionZ, 2);


				WorldCoordinate3D imagePositionPatient;

				try
				{
					imagePositionPatient(0) = boost::lexical_cast<WorldCoordinate>(imagePositionX.c_str());
					imagePositionPatient(1) = boost::lexical_cast<WorldCoordinate>(imagePositionY.c_str());
					imagePositionPatient(2) = boost::lexical_cast<WorldCoordinate>(imagePositionZ.c_str());
				}
				catch (boost::bad_lexical_cast&)
				{
					throw core::InvalidDoseException("boost::lexical_cast ImagePosition failed! Can not read image position X/Y/Z!")
					;
				}

				_geoInfo.setImagePositionPatient(imagePositionPatient);

				SpacingVectorType3D spacingVector;
				OFString pixelSpacingRowStr;
				dose->getPixelSpacing(pixelSpacingRowStr, 0);
				OFString pixelSpacingColumnStr;
				dose->getPixelSpacing(pixelSpacingColumnStr, 1);

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
					throw core::InvalidDoseException("Pixel spacing not readable or = 0!");
				}

				OFString sliceThicknessStr;
				dose->getSliceThickness(sliceThicknessStr);

				try
				{
					spacingVector(2) = boost::lexical_cast<GridVolumeType>(sliceThicknessStr.c_str());
				}
				catch (boost::bad_lexical_cast&)
				{
					spacingVector(2) = 0
					                   ;//if no information about slice thickness, set to 0 and calculate it using z coordinate difference between 1. and 2. dose
				}

				if (spacingVector(2) == 0)
				{
					if (_doseVector.size() > 1)
					{
						DRTDoseIODPtr dose2 = _doseVector.at(1);//get the 2. dose
						OFString imagePositionZ2;
						dose2->getImagePositionPatient(imagePositionZ2, 2);

						try
						{
							spacingVector(2) = boost::lexical_cast<GridVolumeType>(imagePositionZ2.c_str()) -
							                   imagePositionPatient(
							                       2); //caculate slicethickness
						}
						catch (boost::bad_lexical_cast&)
						{
							throw core::InvalidDoseException("Can not read image position Z of the 2. dose!");
						}
					}
					else
					{
						std::cerr << "sliceThickness == 0! It will be replaced with pixelSpacingRow=" <<
						          _geoInfo.getSpacing()(0)
						          << "!" << std::endl;
						spacingVector(2) = spacingVector(0);
					}
				}

				_geoInfo.setSpacing(spacingVector);
				return true;
			}

			GenericValueType DicomHelaxDoseAccessor::getValueAt(const VoxelGridID aID) const
			{
				return _doseData.at(aID) * _doseGridScaling;
			}

			GenericValueType DicomHelaxDoseAccessor::getValueAt(const VoxelGridIndex3D& aIndex) const
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
