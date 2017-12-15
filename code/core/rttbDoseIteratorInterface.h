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
#ifndef __DOSE_ITERATOR_INTERFACE_NEW_H
#define __DOSE_ITERATOR_INTERFACE_NEW_H


#include <boost/shared_ptr.hpp>

#include "rttbBaseType.h"
#include "rttbDoseAccessorInterface.h"

namespace rttb
{
	namespace core
	{
        class GeometricInfo;
		/*! @class DoseIteratorInterface
		@brief This class represents the dose iterator interface.
		*/
		class DoseIteratorInterface
		{
		public:
			typedef boost::shared_ptr<DoseAccessorInterface> DoseAccessorPointer;
			typedef boost::shared_ptr<DoseIteratorInterface> DoseIteratorPointer;

		private:
			DoseIteratorInterface(const DoseIteratorInterface&) = delete; //not implemented on purpose -> non-copyable
			DoseIteratorInterface& operator=(const
			                                 DoseIteratorInterface&) = delete;//not implemented on purpose -> non-copyable
			DoseIteratorInterface() = default;

		protected:
			/*! @brief DoseAccessor to get access to actual dose data */
			DoseAccessorPointer _spDoseAccessor;

		public:
			/*!  @brief Constructor with a DoseIterator this should be the default for all implementations.
			*/
			DoseIteratorInterface(DoseAccessorPointer aDoseAccessor);

			virtual ~DoseIteratorInterface() = default;

			/*! @brief Set the iterator to the start of the dose.
			*/
			virtual bool reset() = 0;

			/*! @brief Move to next position. If this position is valid is not necessarily tested.
			*/
			virtual void next() = 0;

			virtual bool isPositionValid() const = 0;

			/*! @brief Return volume of one voxel (in cm3)*/ //previously getDeltaV()
			virtual DoseVoxelVolumeType getCurrentVoxelVolume() const = 0;

			virtual DoseTypeGy getCurrentDoseValue() const = 0;

			/*! @return If this is a masked dose iterator, return the voxel proportion inside a given structure,
				value 0~1; Otherwise, 1
			*/
			virtual FractionType getCurrentRelevantVolumeFraction() const = 0;

			virtual VoxelGridID getCurrentVoxelGridID() const = 0;

			virtual IDType getVoxelizationID() const
			{
				return "";
			};

			IDType getDoseUID() const
			{
				return _spDoseAccessor->getUID();
			};

		}; //end class
	}//end: namespace core
}//end: namespace rttb

#endif
