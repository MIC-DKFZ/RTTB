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
// @version $Revision: 1066 $ (last changed revision)
// @date    $Date: 2015-08-19 11:47:07 +0200 (Mi, 19 Aug 2015) $ (last change date)
// @author  $Author: floca $ (last changed by)
*/
#ifndef __ITK_IMAGE_ACCESSOR_H
#define __ITK_IMAGE_ACCESSOR_H

#include <exception>

#include <boost/shared_ptr.hpp>

#include "rttbAccessorWithGeoInfoBase.h"
#include "rttbGeometricInfo.h"
#include "rttbBaseType.h"

#include "itkImage.h"

namespace rttb
{
	namespace io
	{
		namespace itk
		{
			/*! @class ITKImageAccessor
			@brief This class gives access to information stored in an itk image
			@details There is no value scaling, i.e., it is assumed that the values in the itkImage are absolute.
			*/
			class ITKImageAccessor: public core::AccessorWithGeoInfoBase
			{
			public:
				typedef ::itk::Image<GenericValueType, 3> ITKImageType;
			private:
				/** @brief The dose as itkImage */
				ITKImageType::ConstPointer _data;

				IDType _UID;

				/*! @brief constructor
					@exception InvalidDoseException if _dose is NULL
				*/
				ITKImageAccessor();

				/*! @brief get all required data from the itk image contained in _dose
					@exception InvalidDoseException if PixelSpacing is 0 or size in any dimension is 0.
				*/
				bool assembleGeometricInfo();


			public:
				~ITKImageAccessor();

				/*! @brief Constructor. Initialization with a itk image
				@pre image must be a valid instance (and not null)
				@note the doseImage pixels are assumed absolute
				*/
				ITKImageAccessor(ITKImageType::ConstPointer image);

				/*! @brief returns the dose for an id
				*/
				GenericValueType getValueAt(const VoxelGridID aID) const;

				/*! @brief returns the dose for an index
				*/
				GenericValueType getValueAt(const VoxelGridIndex3D& aIndex) const;

				const IDType getUID() const
				{
					return _UID;
				};

			};
		}
	}
}

#endif
