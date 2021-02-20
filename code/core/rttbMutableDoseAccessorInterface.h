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

#ifndef __MUTABLE_DOSE_ACCESSOR_INTERFACE_NEW_H
#define __MUTABLE_DOSE_ACCESSOR_INTERFACE_NEW_H

#include "rttbDoseAccessorInterface.h"
#include "rttbBaseType.h"
#include "rttbCommon.h"

#include "RTTBCoreExports.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{
	namespace core
	{

		/*! @class MutableDoseAccessorInterface
			@brief Extends the DoseAccessorInterface to provide writing access to the data.
		*/
		class RTTBCore_EXPORT MutableDoseAccessorInterface: public DoseAccessorInterface
		{

		public:
      rttbClassMacro(MutableDoseAccessorInterface, DoseAccessorInterface)

			virtual void setDoseAt(const VoxelGridID aID, DoseTypeGy value) = 0;

			virtual void setDoseAt(const VoxelGridIndex3D& aIndex, DoseTypeGy value) = 0;

		protected:
			MutableDoseAccessorInterface() = default;
			~MutableDoseAccessorInterface() override = default;

		private:
			MutableDoseAccessorInterface(const MutableDoseAccessorInterface&) = delete; //not implemented on purpose -> non-copyable
			MutableDoseAccessorInterface& operator=(const
				MutableDoseAccessorInterface&) = delete;//not implemented on purpose -> non-copyable
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
