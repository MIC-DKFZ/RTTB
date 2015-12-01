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
#ifndef __DICOM_DOSE_ITERATOR_USE_MASK_H
#define __DICOM_DOSE_ITERATOR_USE_MASK_H

#include <string>
#include <vector>
#include <map>


#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */
#include "dcmtk/dcmrt/drtdose.h"

#include "DoseIteratorInterface_LEGACY.h"
#include "DoseVoxel_LEGACY.h"
#include "rttbGeometricInfo.h"


namespace rttb
{
	namespace masks
	{
		namespace legacy
		{

			/*! @class DoseIterator
			*  @brief This class represents the iterator of the dose voxels in a DRTDoseIOD Object within the >0 region of a image mask
			*/

			class DoseIterator: public DoseIteratorInterface
			{


			private:
				/*! DICOM-RT dose pointer*/
				DRTDoseIOD* _dose;

				/*! DICOM-RT mask pointer. It must have the same image properties as _dose. The structure is masked with 1.*/
				DRTDoseIOD* _mask;



				/*! vector of dose data(absolute Gy dose/doseGridScaling)*/
				std::vector<Uint16> doseData;
				/*! Columns (0028,0011) of the dicom dose _dose*/
				Uint16 column;
				/*! Rows (0028,0010) of the dicom dose _dose*/
				Uint16 row;
				/*! NumberOfFrames (0028,0008) of the dicom dose _dose*/
				Uint16 numberOfFrames;
				/*! DoseGridScaling (3004,000e) of the dicom dose _dose*/
				double doseGridScaling;
				/*! ImagePositionPatient (0020,0032) of the dicom dose _dose*/
				WorldCoordinate3D imagePositionPatient;
				/*! ImageOrientationPatient (0020,0037) of the dicom dose _dose*/
				ImageOrientation imageOrientationRow;
				ImageOrientation imageOrientationColumn;
				/*! PixelSpacing (0028,0030) of the dicom dose _dose*/
				double pixelSpacingRow;
				double pixelSpacingColumn;
				/*! SliceThickness (0018,0050) of the dicom dose _dose*/
				double sliceThickness;


				/*! The current position in pixelDataIndexInStructure*/
				int count;

				/*! The vector stores all indices with pixel data >0 in the mask*/
				std::vector<int> pixelDataIndexInStructure;




				/*! @brief Get information from dose distribution:column, row, numberOfFramesdose,
				*         GridScaling, imageOrientation, pixelSpacing, getSliceThickness() and absolute dose value in each voxel
				*/

				bool setDoseInfo(core::GeometricInfo& aGeoInfo);






			protected:

				/*! @brief Traverse dose distribution, get the dose voxels in the structure: doseVoxelInStructure.
				* It will be called in constructor or if structure is resetted.
				*  @exception RTTBNullPointerException Thrown if _dose or _structure is NULL
				*  @exception RTTBInvalidDoseException Thrown if _dose is invalid: one of column/row/numberOfFrames/doseGridScaling/pixelSpacing=0
				*  @exception RTTBDcmrtException Throw if dcmrt error
				*/
				bool begin()
				{
					start();
					return true;
				};


			public:
				/*! @brief Destructor*/
				virtual ~DoseIterator();

				/*! @brief Standard constructor
				*  @exception RTTBNullPointerException Thrown if _dose or _mask is NULL
				*  @exception RTTBInvalidDoseException Thrown if _dose is invalid: one of column/row/numberOfFrames/doseGridScaling/pixelSpacing=0
				*  @exception RTTBDcmrtException Throw if dcmrt error
				*/
				DoseIterator(core::GeometricInfo& aGeoInfo, DRTDoseIOD* doseIOD, DRTDoseIOD* maskIOD);

				/*! @brief Set the position before the first index.
				*  It must be called before hasNextVoxel() and next()!
				*/
				void start();


				/*! @brief Test if next voxel existiert
				*  @pre start() must be called before it is called!
				*/
				bool hasNextVoxel() const;

				/*! @brief Return dose value (in Gy) of the next index
				*  @pre hasNextVoxel() must =true before it is called!
				*  @exception RTTBIndexOutOfBoundsException Thrown if hasNextVoxel()==false
				*/
				DoseTypeGy next();

				/*! @brief Volume of one voxel (in cm3)*/
				DoseVoxelVolumeType getDeltaV() const;

				/*! @brief Return geometric information*/
				core::GeometricInfo getGeometricInfo() const;

				/*! @brief Return 1? To be modified...*/
				double getCurrentVoxelProportion() const;

				/*! @brief Return the current RTVoxel  Object*/
				const DoseVoxel& getCurrentRTVoxel();
			};
		}
	}
}

#endif
