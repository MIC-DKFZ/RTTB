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

#include "rttbArithmetic.h"


namespace rttb
{
	namespace algorithms
	{
		namespace arithmetic
		{

			void add(const DoseAccessorPointer dose1, const DoseAccessorPointer dose2,
					 MutableDoseAccessorPointer result)
			{
				doseOp::Add addOP;
				arithmetic(dose1, dose2, result, addOP);
			}

			void add(const MaskAccessorPointer mask1, const MaskAccessorPointer mask2,
					 MutableMaskAccessorPointer result)
			{
				maskOp::Add addOP;
				arithmetic(mask1, mask2, result, addOP);
			}

			void subtract(const MaskAccessorPointer mask1, const MaskAccessorPointer mask2,
						  MutableMaskAccessorPointer result)
			{
				maskOp::Subtract subOP;
				arithmetic(mask1, mask2, result, subOP);
			}

			void multiply(const DoseAccessorPointer dose, const MaskAccessorPointer mask,
						  MutableDoseAccessorPointer result)
			{
				doseMaskOp::Multiply multOP;
				arithmetic(dose, mask, result, multOP);
			}

			namespace doseOp
			{
				DoseTypeGy Add::calc(const DoseTypeGy dose1Val, const DoseTypeGy dose2Val) const
				{
					return dose1Val + dose2Val;
				}

				AddWeighted::AddWeighted(const DoseCalcType w1, const DoseCalcType w2):
					weight1(w1), weight2(w2) {};

				DoseTypeGy AddWeighted::calc(const DoseTypeGy dose1Val, const DoseTypeGy dose2Val) const
				{
					return weight1 * dose1Val + weight2 * dose2Val;
				}
			}

			namespace doseMaskOp
			{
				DoseTypeGy Multiply::calc(const DoseTypeGy doseVal, const FractionType maskVal) const
				{
					return doseVal * maskVal;
				}
			}

			namespace maskOp
			{
				FractionType Add::calc(const FractionType mask1Val, const FractionType mask2Val) const
				{
					FractionType added = mask1Val + mask2Val;
					return (1 < added ? 1 : added);
				}

				FractionType Subtract::calc(const FractionType mask1Val, const FractionType mask2Val) const
				{
					FractionType sub = mask1Val - mask2Val;
					return (0 > sub ? 0 : sub);
				}
			}
		}
	}
}