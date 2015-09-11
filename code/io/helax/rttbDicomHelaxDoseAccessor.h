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
#ifndef __DICOM_HELAX_DOSE_ACCESSOR_H
#define __DICOM_HELAX_DOSE_ACCESSOR_H

#include <string>
#include <vector>
#include <exception>

#include <boost/numeric/ublas/matrix.hpp>

#include "osconfig.h"    /* make sure OS specific configuration is included first */
#include "drtdose.h"

#include "rttbAccessorWithGeoInfoBase.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"


namespace rttb
{
	namespace io
	{
		namespace helax
		{

			/*! @class DicomHelaxDoseAccessor
			@brief Load dose data from a directory containing dicom dose files, each file describes the helax dose in one slice.
			*/
			class DicomHelaxDoseAccessor: public core::AccessorWithGeoInfoBase
			{
			public:
				typedef boost::shared_ptr<DRTDoseIOD> DRTDoseIODPtr;

			private:
				/*! vector of DRTDoseIOD shared pointers, each DRTDoseIOD pointer presents the dose in one slice*/
				std::vector<DRTDoseIODPtr> _doseVector;

				/*! vector of dose data(absolute Gy dose/doseGridScaling)*/
				std::vector<Uint16> _doseData;

				double _doseGridScaling;

				IDType _doseUID;

				DicomHelaxDoseAccessor();

			protected:
				/*! @brief Initialize dose data
				@exception InvalidDoseException Thrown if any DRTDoseIOD pointer of _doseVector is invalid: one of column/row/numberOfFrames/doseGridScaling/pixelSpacing=0
				@exception DcmrtException Throw if dcmrt error
				@exception boost/bad_lexical_cast Thrown if the imported header tags are not numerical.
				*/
				bool begin();

				/*! @brief get all required data from dicom information contained in _dose
				@exception boost/bad_lexical_cast Thrown if the imported header tags are not numerical.
				*/
				bool assembleGeometricInfo();


			public:

				~DicomHelaxDoseAccessor();


				/*! @brief Constructor. Initialisation with a vector of DRTDoseIOD pointers
				@exception InvalidDoseException Thrown if any DRTDoseIOD pointer of _doseVector is invalid: one of column/row/numberOfFrames/doseGridScaling/pixelSpacing=0
				@exception DcmrtException Throw if dcmrt error
				*/
				DicomHelaxDoseAccessor(std::vector<DRTDoseIODPtr> aDICOMRTDoseVector);


				GenericValueType getValueAt(const VoxelGridID aID) const;

				GenericValueType getValueAt(const VoxelGridIndex3D& aIndex) const;

				const IDType getUID() const
				{
					return _doseUID;
				};
			};
		}
	}
}

#endif
