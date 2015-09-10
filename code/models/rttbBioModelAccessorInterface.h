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
#ifndef __BIO_MODEL_ACCESSOR_INTERFACE_H
#define __BIO_MODEL_ACCESSOR_INTERFACE_H

#include <boost/shared_ptr.hpp>

#include "rttbBaseType.h"
#include "rttbBaseTypeModels.h"
#include "rttbGeometricInfo.h"
#include "rttbIndexConversionInterface.h"

namespace rttb
{
	namespace core
	{

		/*! @class BioModelAccessorInterface
			@brief Interface for all BioModelAccessor classes
		*/
		class BioModelAccessorInterface: public IndexConversionInterface
		{
		public:
			typedef boost::shared_ptr<BioModelAccessorInterface> BioModelAccessorPointer;
		private:
			BioModelAccessorInterface(const BioModelAccessorInterface&); //not implemented on purpose -> non-copyable
			BioModelAccessorInterface& operator=(const
			                                     BioModelAccessorInterface&);//not implemented on purpose -> non-copyable

		public:
			BioModelAccessorInterface() {};
			virtual ~BioModelAccessorInterface() {};

			inline const core::GeometricInfo& getGeometricInfo() const
			{
				return _geoInfo;
			};

			inline GridSizeType getGridSize() const
			{
				return _geoInfo.getNumberOfVoxels();
			};

			virtual models::BioModelValueType getBioModelValueAt(const VoxelGridID aID) const = 0;

			virtual models::BioModelValueType getBioModelValueAt(const VoxelGridIndex3D& aIndex) const = 0;

			/*! @brief is true if dose is on a homogeneous grid
				@remarks Inhomogeneous grids are not supported at the moment, but if they will be supported in the future
				the interface does not need to change.
			*/
			virtual bool isGridHomogeneous() const
			{
				return true;
			}

			virtual const IDType getBioModelUID() const = 0;
		protected:
			core::GeometricInfo _geoInfo;
		};
	}
}

#endif
