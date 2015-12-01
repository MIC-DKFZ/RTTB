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
#ifndef __DOSE_ITERATOR_INTERFACE_H
#define __DOSE_ITERATOR_INTERFACE_H


#include <string>
#include <vector>
#include <exception>

#include "DoseVoxel_LEGACY.h"
#include "rttbPhysicalInfo.h"


namespace rttb
{

	namespace masks
	{
		namespace legacy
		{
			/*! @class DoseIteratorInterface
			*  @brief This class represents the dose iterator interface.
			*/
			class DoseIteratorInterface: public core::PhysicalInfo
			{
			protected:
				/*! @brief Dose voxel index (x,y,z)*/
				VoxelGridIndex3D _doseIndex;

				/*! @brief Dose UID*/
				IDType _doseUID;

				/*! @brief Patient UID*/
				IDType _patientUID;

				/*! @brief Current DoseVoxel*/
				DoseVoxel _currentVoxel;

				/*! @brief Read dose and structure information
				*  It will be called in constructor or if dose or structure is resetted
				*/
				virtual bool begin() = 0;


			public:
				/*! @brief Set the position before the first index. It must be called before hasNextVoxel() and next()!
				*/
				virtual void start() = 0;

				/*! @brief Test if next voxel exists
				*  @pre start() must be called before it is called!
				*/
				virtual bool hasNextVoxel() const = 0;

				/*! @brief Return dose value (in Gy) of the next index. After next() is called, current index of this iterator is at the next index.
				*  @pre hasNextVoxel() must =true before it is called!
				*/
				virtual DoseTypeGy next() = 0;

				//RALF: man sollte zumindest so ein interface auch einbauen um einen viruellen Call zu sparen
				//virtual bool next(DoseTypeGy& nextValue) = 0;

				/*! @brief Return volume of one voxel (in cm3)*/
				virtual DoseVoxelVolumeType getDeltaV() const = 0;

				/*! @brief Return geometric information*/
				virtual core::GeometricInfo getGeometricInfo() const = 0;

				/*! @brief If masked dose iterator, return the voxel proportion inside a given structure, value 0~1; Otherwise, 1*/
				virtual double getCurrentVoxelProportion() const = 0;

				/*! @brief Get dose uid. If there exists uuid for the dose, you should rewrite the method; otherwise, a uid will be
				*  randomly generated using boost::uuid library.
				*/
				IDType getDoseUID();

				/*! @brief Get patient uid. If there exists patient uuid for the dose, you should rewrite the method; otherwise, a uid will be
				*  randomly generated using boost::uuid library.
				*/
				IDType getPatientUID();

				/*! @brief Return the current RTVoxel Object*/
				virtual const DoseVoxel& getCurrentRTVoxel() = 0;

				/*! @brief Return the size of the dose iterator*/
				virtual int size();






			};
		}
	}
}

#endif
