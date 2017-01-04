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

#ifndef __TRANSFORMATION_INTERFACE_H
#define __TRANSFORMATION_INTERFACE_H

#include <boost/shared_ptr.hpp>

#include "rttbBaseType.h"

#include "RTTBInterpolationExports.h"
namespace rttb
{
	namespace interpolation
	{

		/*! @class TransformationInterface
			@brief Base class for transformation (in World coordinates).
			@ingroup interpolation
		*/
        class RTTBInterpolation_EXPORT TransformationInterface
		{
		public:
			typedef boost::shared_ptr<TransformationInterface> Pointer;
		protected:
			/*! @brief Constructor
			*/
			TransformationInterface() {};

			/*! @brief Virtual destructor of interface class
			*/
			virtual ~TransformationInterface() {};
		public:
			/*! @brief performs a transformation targetImage --> movingImage
			*/
			virtual bool transformInverse(const WorldCoordinate3D& worldCoordinateTarget,
			                              WorldCoordinate3D& worldCoordinateMoving) const = 0;

			/*! @brief performs a transformation movingImage --> targetImage
			*/
			virtual bool transform(const WorldCoordinate3D& worldCoordinateMoving,
			                       WorldCoordinate3D& worldCoordinateTarget) const = 0;

		private:
			TransformationInterface(const TransformationInterface&);//not implemented on purpose -> non-copyable
			TransformationInterface& operator=(const
			                                   TransformationInterface&);//not implemented on purpose -> non-copyable
		};

	}
}

#endif
