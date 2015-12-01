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

#include "DoseIterator_LEGACY.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidDoseException.h"
#include "rttbIndexOutOfBoundsException.h"


using namespace std;


namespace rttb
{

	namespace masks
	{
		namespace legacy

		{


			DoseIterator::~DoseIterator()
			{

			}

			DoseIterator::DoseIterator(core::GeometricInfo& aGeoInfo, DRTDoseIOD* doseIOD,
			                           DRTDoseIOD* maskIOD): doseData(aGeoInfo.getNumberOfVoxels(), 0)
			{
				this->count = -1;

				_dose = doseIOD;
				_mask = maskIOD;

				setDoseInfo(aGeoInfo);
			}

			void DoseIterator::start()
			{
				this->count = -1;
			}

			bool DoseIterator::setDoseInfo(core::GeometricInfo& aGeoInfo)
			{
				/* get Columns (0028,0011) of the dicom dose _dose*/
				column = aGeoInfo.getNumColumns();

				/*! get Rows (0028,0010) of the dicom dose _dose*/
				row = aGeoInfo.getNumRows();

				/* get NumberOfFrames (0028,0008) of the dicom dose _dose*/
				numberOfFrames = aGeoInfo.getNumSlices();

				//std::cout << "numberOfFrames: "<< numberOfFrames << std::endl;
				if (column == 0 || row == 0 || numberOfFrames == 0)
				{
					throw rttb::core::InvalidDoseException("Empty dicom dose!");
				}

				/* get DoseGridScaling (3004,000e) of the dicom dose _dose*/
				OFString doseGridScalingStr;
				this->_dose->getDoseGridScaling(doseGridScalingStr);
				this->doseGridScaling = 1;

				imagePositionPatient = aGeoInfo.getImagePositionPatient();
				imageOrientationRow = aGeoInfo.getImageOrientationRow();
				imageOrientationColumn = aGeoInfo.getImageOrientationColumn();

				/* get PixelSpacing (0028,0030) of the dicom dose _dose*/
				pixelSpacingRow = aGeoInfo.getPixelSpacingRow();
				pixelSpacingColumn = aGeoInfo.getPixelSpacingColumn();

				/* get SliceThickness (0018,0050) of the dicom dose _dose*/
				sliceThickness = aGeoInfo.getSliceThickness();

				return true;

			}



			DoseTypeGy DoseIterator::next()
			{

				int size = this->pixelDataIndexInStructure.size();

				if (count < (size - 1))
				{
					count++;
					DoseTypeGy dose;
					int i = pixelDataIndexInStructure[count];
					dose = doseData.at(i) * this->doseGridScaling;

					return dose;
				}
				else
				{
					throw core::IndexOutOfBoundsException("hasNextVoxel()==false!");

				}

			}

			bool DoseIterator::hasNextVoxel() const
			{

				int size = this->pixelDataIndexInStructure.size();

				if (count < (size - 1))
				{
					return true;
				}
				else
				{
					return false;
				}

			}

			DoseVoxelVolumeType DoseIterator::getDeltaV() const
			{
				return this->pixelSpacingColumn * this->pixelSpacingRow * this->sliceThickness / 1000;
			}

			core::GeometricInfo DoseIterator::getGeometricInfo() const
			{
				core::GeometricInfo ginfo;
				ginfo.setNumColumns(this->column);
				ginfo.setNumRows(this->row);
				OrientationMatrix orientation(0);
				orientation(0, 0) = this->imageOrientationRow.x();
				orientation(1, 0) = this->imageOrientationRow.y();
				orientation(2, 0) = this->imageOrientationRow.z();
				orientation(0, 1) = this->imageOrientationColumn.x();
				orientation(1, 1) = this->imageOrientationColumn.y();
				orientation(2, 1) = this->imageOrientationColumn.z();
				WorldCoordinate3D ortho = this->imageOrientationRow.cross(this->imageOrientationColumn);
				orientation(0, 2) = ortho.x();
				orientation(1, 2) = ortho.y();
				orientation(2, 2) = ortho.z();
				ginfo.setOrientationMatrix(orientation);
				ginfo.setImagePositionPatient(this->imagePositionPatient);
				ginfo.setNumSlices(this->numberOfFrames);
				ginfo.setPixelSpacingColumn(this->pixelSpacingColumn);
				ginfo.setPixelSpacingRow(this->pixelSpacingRow);
				ginfo.setSliceThickness(this->sliceThickness);
				return ginfo;
			}

			double DoseIterator::getCurrentVoxelProportion() const
			{
				return 1;
			}

			const DoseVoxel& DoseIterator::getCurrentRTVoxel()
			{
				return _currentVoxel;
			}


		}

	}
}

