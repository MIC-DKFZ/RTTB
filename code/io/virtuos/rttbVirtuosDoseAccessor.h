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
#ifndef __VIRTUOS_DOSE_ACCESSOR_H
#define __VIRTUOS_DOSE_ACCESSOR_H

#include <string>
#include <vector>
#include <exception>

#include "rttbDoseAccessorInterface.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"

#include "ncfile.h"

namespace rttb
{
	namespace io
	{
		namespace virtuos
		{

			/*! @class VirtuosDoseAccessor
			@brief This class gives access to dose information from Virtuos Cubeinfo
			*/
			class VirtuosDoseAccessor: public core::DoseAccessorInterface
			{
			private:
				Cubeinfo** _pPointerOnVirtuosCube;


				/*! absolute Gy dose/doseGridScaling*/
				std::vector<DoseTypeGy> doseData;

				double _doseGridScaling;

				IDType _doseUID;

				DoseTypeGy _prescribedDose;
				DoseTypeGy _normalizationDose;

				DoseTypeGy _doseScalingFactor;

				bool _freeVirtuosData;//if virtuos cube info should be closed


				/*! close virtuos data cube.
				*/
				void freeVirtuosData();

				/*! load actual pixel data from virtuos cube to doseData. TPixelType is usually int but may be float for
				special virtuos cubes (data_type = 2).
				@pre virtuos cube contains data (dimensions are at least 1)

				*/
				template <typename TPixelType>
				void importPixelData();

			protected:
				/*! @brief import dose data and relevant geometric information
				@throw NullPointerException Thrown if _pPointerOnVirtuosCube is NULL
				@throw InvalidDoseException Thrown if one dimension of the virtuos cube is zero
				@throw InvalidParameterException Thrown if _pPointerOnVirtuosCube is invalid
				*/
				void begin();

				/*! @brief get all required geometrical data from virtuos cube
				*/
				void assembleGeometricInfo();


			public:
				~VirtuosDoseAccessor();

				/*! @brief Constructor. Initialisation with a Cubeinfo pointer.
				@param normalizationDose is defined as (prescribedDose*1000)/maxDoseInGy. Default is 1 Gy.
				@param prescribedDose the does that was planned in the reference point in Gy. Default is 1 Gy.
				@param freeVirtuoData If virtuos cube info should be closed, freeVirtuosData should be true.
				@throw NullPointerException Thrown if _pPointerOnVirtuosCube is NULL
				@throw InvalidDoseException Thrown if one dimension of the virtuos cube is zero
				@throw InvalidParameterException Thrown if aPointerOnVirtuosCube is invalid
				*/
				VirtuosDoseAccessor(Cubeinfo* aPointerOnVirtuosCube, bool freeVirtuosData,
				                    DoseTypeGy normalizationDose = 1, DoseTypeGy prescribedDose = 1);

				DoseTypeGy getDoseAt(const VoxelGridID aID) const;

				/*! @return dose value at given grid position. If position is invalid return -1
				*/
				DoseTypeGy getDoseAt(const VoxelGridIndex3D& aIndex) const;

				const IDType getDoseUID() const
				{
					return _doseUID;
				};

				const DoseTypeGy getPrescribedDose() const
				{
					return _prescribedDose;
				};

				const DoseTypeGy getNormalizationDose() const
				{
					return _normalizationDose;
				};
			};
		}
	}
}

#endif
