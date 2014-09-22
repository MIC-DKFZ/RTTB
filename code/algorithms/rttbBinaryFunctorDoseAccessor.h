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
#ifndef __BINARY_FUNCTOR_DOSE_ACCESSOR_H
#define __BINARY_FUNCTOR_DOSE_ACCESSOR_H

#include <boost/shared_ptr.hpp>

#include "rttbDoseAccessorInterface.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"

namespace rttb
{
	namespace algorithms
	{

		/*! @class BinaryFunctorDoseAccessor
		@brief Class that allows to access the results of a binary dose operation.
		@details this Accessor takes two dose accessors as operants (the operants must have the same geometry)
		and computes a resulting dose value be using a given dose operation functor.
		The resulting dose value will be returned from the accessor as its value upon request.
		@remark this can be seen as a lazy filter pattern, thus the accessor is filtering/operating on dose values upon request.
		*/
		template <class TDoseOperation>
		class BinaryFunctorDoseAccessor: public core::DoseAccessorInterface
		{
		public:
			typedef boost::shared_ptr<BinaryFunctorDoseAccessor> BinaryFunctorDoseAccessorPointer;
		protected:
			DoseAccessorPointer _spDose1;
			DoseAccessorPointer _spDose2;
			TDoseOperation _functor;

		public:
			/*! @brief Constructor.
				@param dose1 pointer to the 1st dose operand
				@param dose2 pointer to the 2nd dose operand
				@param functor Instance of the operation that should be used
				@pre all input parameters have to be valid
				@exception core::NullPointerException if one input parameter is NULL or if geometricInfos don't match
			*/
			BinaryFunctorDoseAccessor(const DoseAccessorPointer dose1, const DoseAccessorPointer dose2,
			                          const TDoseOperation& functor);

			/*! @brief Virtual destructor
			*/
			virtual ~BinaryFunctorDoseAccessor() {};

			/*! @pre: the geometricInfo of both doseAccessors are equal
			*/
			inline const core::GeometricInfo& getGeometricInfo() const
			{
				return _spDose1->getGeometricInfo();
			};

			/*! @pre: the geometricInfo of both doseAccessors are equal
			*/
			inline GridSizeType getGridSize() const
			{
				return _spDose1->getGeometricInfo().getNumberOfVoxels();
			};

			/*! @brief Returns the result dose computed by the functor.
			    It uses the dose values of both operand doses specified via the passed ID.
				@return the dose value if inside, -1 else
				@pre <TDoseOperation>.calc(dose1,dose2) has to be implemented
			*/
			DoseTypeGy getDoseAt(const VoxelGridID aID) const;

			/*! @brief Returns the result dose computed by the functor.
			    It uses the dose values of both operand doses specified via the passed index.
				@return the dose value if inside, -1 else
				@pre <TDoseOperation>.calc(dose1,dose2) has to be implemented
			*/
			DoseTypeGy getDoseAt(const VoxelGridIndex3D& aIndex) const;

			//@todo: which UID should be used here?
			const IDType getDoseUID() const
			{
				return IDType();
			}
		};
	}
}

#include "rttbBinaryFunctorDoseAccessor.tpp"

#endif
