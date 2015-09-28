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
// @version $Revision: 747 $ (last changed revision)
// @date    $Date: 2014-09-17 12:01:00 +0200 (Mi, 17 Sep 2014) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/
#ifndef __BINARY_FUNCTOR_ACCESSOR_H
#define __BINARY_FUNCTOR_ACCESSOR_H

#include <boost/shared_ptr.hpp>

#include "rttbAccessorInterface.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"

namespace rttb
{
	namespace algorithms
	{

		/*! @class BinaryFunctorAccessor
		@brief Class that allows to access the results of a binary operation.
		@details this Accessor takes two accessors as operants (the operants must have the same geometry)
		and computes a resulting value by using a given operation functor.
		The resulting value will be returned from the accessor as its value upon request.
		@remark this can be seen as a lazy filter pattern, thus the accessor is filtering/operating on dose values upon request.
		*/
		template <class TDoseOperation>
		class BinaryFunctorAccessor: public core::AccessorInterface
		{
		protected:
			AccessorPointer _spData1;
			AccessorPointer _spData2;
			TDoseOperation _functor;

		public:
			/*! @brief Constructor.
				@param data1 pointer to the 1st data operand
				@param data2 pointer to the 2nd data operand
				@param functor Instance of the operation that should be used
				@pre all input parameters have to be valid
				@exception core::NullPointerException if one input parameter is NULL or if geometricInfos don't match
			*/
			BinaryFunctorAccessor(const AccessorPointer data1, const DoseAccessorPointer data2,
			                      const TDoseOperation& functor);

			/*! @brief Virtual destructor
			*/
			virtual ~BinaryFunctorAccessor() {};

			/*! @pre: the geometricInfo of both doseAccessors are equal
			*/
			inline const core::GeometricInfo& getGeometricInfo() const
			{
				return _spData1->getGeometricInfo();
			};

			/*! @pre: the geometricInfo of both doseAccessors are equal
			*/
			inline GridSizeType getGridSize() const
			{
				return _spData1->getGeometricInfo().getNumberOfVoxels();
			};

			/*! @brief Returns the result dose computed by the functor.
			    It uses the dose values of both operand doses specified via the passed ID.
				@return the dose value if inside, -1 else
				@pre <TDoseOperation>.calc(dose1,dose2) has to be implemented
			*/
			GenericValueType getValueAt(const VoxelGridID aID) const;

			/*! @brief Returns the result dose computed by the functor.
			    It uses the dose values of both operand doses specified via the passed index.
				@return the dose value if inside, -1 else
				@pre <TDoseOperation>.calc(dose1,dose2) has to be implemented
			*/
			GenericValueType getValueAt(const VoxelGridIndex3D& aIndex) const;

			//@todo: which UID should be used here?
			const IDType getUID() const
			{
				return IDType();
			}
		};
	}
}

#include "rttbBinaryFunctorAccessor.tpp"

#endif