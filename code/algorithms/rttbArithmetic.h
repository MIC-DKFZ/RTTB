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

#ifndef __ARITHMETIC_H
#define __ARITHMETIC_H

#include "rttbBaseType.h"
#include "rttbDoseAccessorInterface.h"
#include "rttbMutableMaskAccessorInterface.h"
#include "rttbMutableDoseAccessorInterface.h"
#include "rttbMaskAccessorInterface.h"
#include "rttbMaskVoxel.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace algorithms
	{

		namespace arithmetic
		{
			typedef core::DoseAccessorInterface::DoseAccessorPointer DoseAccessorPointer;
			typedef core::MutableDoseAccessorInterface::MutableDoseAccessorPointer MutableDoseAccessorPointer;
			typedef core::MutableMaskAccessorInterface::MutableMaskAccessorPointer MutableMaskAccessorPointer;
			typedef core::MaskAccessorInterface::MaskAccessorPointer MaskAccessorPointer;
			typedef core::MaskAccessorInterface::MaskVoxelList MaskVoxelList;
			typedef core::MaskAccessorInterface::MaskVoxelListPointer MaskVoxelListPointer;


			/*! Applies the given dose operation to the given doses and stores the result in <i>result</i>
			  @pre pointers to accessors are not NULL. The geometric Info of the individual accessors must be equal.
				@exception NullPointerException thrown if one of the input accessors is NULL.
			  @exception InvalidParameterException thrown if the geometricInfo of the given accessors does not match.
			*/
			template <class TDoseOperation>
			void arithmetic(const DoseAccessorPointer dose1, const DoseAccessorPointer dose2,
			                MutableDoseAccessorPointer result, TDoseOperation op);

			///*! Applies the given dose operation to the mapped given doses (transformation given by MatchPoint) and stores the result in <i>result</i>
			//     @pre pointers to accessors are not NULL.
			//   	@exception NullPointerException thrown if one of the input accessors is NULL.
			//     @exception TransformationOutsideImageException thrown if the transformation maps to a position outside the original image.
			//   */
			//   template <class TDoseOperation>
			//   void arithmetic (const DoseAccessorPointer dose1, const MappableDoseAccessorPointer dose2,
			//     MutableDoseAccessorPointer result, TDoseOperation op);

			/*! Applies the given dose-mask operation to the given dose and mask and stores the result in <i>result</i>
			  @pre pointers to accessors are not NULL. The geometric Info of the individual accessors must be equal.
				@exception NullPointerException thrown if one of the input accessors is NULL.
			  @exception InvalidParameterException thrown if the geometricInfo of the given accessors does not match.
			*/
			template <class TDoseMaskOperation>
			void arithmetic(const DoseAccessorPointer dose, const MaskAccessorPointer mask,
			                MutableDoseAccessorPointer result, TDoseMaskOperation op);

			/*! Applies the given mask operation to the given masks and stores the result in <i>result</i>
			  @pre pointers to accessors are not NULL. The geometric Info of the individual accessors must be equal.
				@exception NullPointerException thrown if one of the input accessors is NULL.
			  @exception InvalidParameterException thrown if the geometricInfo of the given accessors does not match.
			*/
			template <class TMaskOperation>
			void arithmetic(const MaskAccessorPointer mask1, const MaskAccessorPointer mask2,
			                MutableMaskAccessorPointer result, TMaskOperation op);

			//convenience functions
			void add(const DoseAccessorPointer dose1, const DoseAccessorPointer dose2,
			         MutableDoseAccessorPointer result);
			//void add(const DoseAccessorPointer dose1, const MappableDoseAccessorPointer dose2,
			//	  MutableDoseAccessorPointer result);
			void add(const MaskAccessorPointer mask1, const MaskAccessorPointer mask2,
			         MutableMaskAccessorPointer result);
			void subtract(const MaskAccessorPointer mask1, const MaskAccessorPointer mask2,
			              MutableMaskAccessorPointer result);
			void multiply(const DoseAccessorPointer dose, const MaskAccessorPointer mask,
			              MutableDoseAccessorPointer result);

			/*all operation classes need to implement the function calc() that performs the entry wise operation
			  The operations are sorted into name spaces according to useful application. If the input values are compatible
			  the operations can also be applied to accessors they were not meant for.
			*/

			//Operations for binary-dose-operation template
			namespace doseOp
			{
				class Add
				{
				public:
					DoseTypeGy calc(const DoseTypeGy dose1Val, const DoseTypeGy dose2Val) const;
				};
				class AddWeighted
				{
				private:
					DoseCalcType weight1, weight2;
				public:
					/* ! Constructor initializes weights applied to individual doses values on adding.
					*/
					AddWeighted(const DoseCalcType w1 = 1, const DoseCalcType w2 = 1);

					DoseTypeGy calc(const DoseTypeGy dose1Val, const DoseTypeGy dose2Val) const;
				};
				class Multiply
				{
				public:
					DoseTypeGy calc(const DoseTypeGy dose1Val, const DoseTypeGy dose2Val) const;
				};
			}

			//Operations for binary-dose-mask-operation template
			namespace doseMaskOp
			{
				class Multiply
				{
				public:
					DoseTypeGy calc(const DoseTypeGy doseVal, const FractionType maskVal) const;
				};

			}

			//Operations for binary-mask-operation template
			//the result of these operations needs to be between 0 and 1
			namespace maskOp
			{
				class Add
				{
				public:
					FractionType calc(const FractionType mask1Val, const FractionType mask2Val) const;
				};
				class Subtract
				{
				public:
					FractionType calc(const FractionType mask1Val, const FractionType mask2Val) const;
				};

			}
		}//end namespace arithmetic

	}//end namespace algorithms
}//end namespace core


#include "rttbArithmetic.tpp"

#endif
