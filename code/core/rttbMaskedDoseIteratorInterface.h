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

#ifndef __MASKED_DOSE_ITERATOR_INTERFACE_NEW_H
#define __MASKED_DOSE_ITERATOR_INTERFACE_NEW_H

#include <rttbCommon.h>

#include "rttbDoseIteratorInterface.h"
#include "rttbMaskAccessorInterface.h"

#include "RTTBCoreExports.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{
	namespace core
	{
        class MaskAccessorInterface;
		/*! @class MaskedDoseIteratorInterface
			@brief Give access to masked dose data.
		*/
        class RTTBCore_EXPORT MaskedDoseIteratorInterface : public DoseIteratorInterface
		{
		public:
      rttbClassMacro(MaskedDoseIteratorInterface, DoseIteratorInterface);
			using MaskAccessorPointer = MaskAccessorInterface::Pointer;
			using DoseAccessorPointer = DoseAccessorInterface::Pointer;
			using DoseIteratorPointer = DoseIteratorInterface::Pointer;

		private:
			MaskedDoseIteratorInterface(const MaskedDoseIteratorInterface&) = delete;
			MaskedDoseIteratorInterface& operator=(const MaskedDoseIteratorInterface&) = delete;

		protected:
			/*! @brief Mask that is to be applied to currently loaded dose*/
			MaskAccessorPointer _spMask;

		public:
			/* Constructor
			@pre core::GeometricInfo represents the same geometric space for both mask and dose,
			i.e. both live on the same data grid. Both accessors need to be valid.
			*/
			MaskedDoseIteratorInterface(MaskAccessorPointer aMaskAccessor, DoseAccessorPointer aDoseAccessor);

			~MaskedDoseIteratorInterface() override = default;

			inline MaskAccessorPointer getMaskAccessor() const
			{
				return _spMask;
			};

			/* Return doseValue*voxelFraction for the current position
			*/
			virtual DoseTypeGy getCurrentMaskedDoseValue() const = 0;
		};
	}
}
#endif