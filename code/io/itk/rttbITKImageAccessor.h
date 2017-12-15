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
#ifndef __ITK_IMAGE_ACCESSOR_H
#define __ITK_IMAGE_ACCESSOR_H

#include "rttbAccessorWithGeoInfoBase.h"
#include "rttbBaseType.h"

#include "itkImage.h"

#include "RTTBITKIOExports.h"

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
			class RTTBITKIO_EXPORT ITKImageAccessor : public core::AccessorWithGeoInfoBase
			{
			public:
				typedef ::itk::Image<GenericValueType, 3> ITKImageType;
			private:
				/** @brief The dose as itkImage */
				ITKImageType::ConstPointer _data;

				IDType _UID;

				/*! @brief constructor
					@exception InvalidDoseException if _dose is nullptr
				*/
				ITKImageAccessor() = delete;

				/*! @brief get all required data from the itk image contained in _dose
					@exception InvalidDoseException if PixelSpacing is 0 or size in any dimension is 0.
				*/
				bool assembleGeometricInfo();


			public:
				~ITKImageAccessor() override;

				/*! @brief Constructor. Initialization with a itk image
				@pre image must be a valid instance (and !nullptr)
				@note the doseImage pixels are assumed absolute
				*/
				ITKImageAccessor(ITKImageType::ConstPointer image);

				/*! @brief returns the dose for an id
				*/
				GenericValueType getValueAt(const VoxelGridID aID) const override;

				/*! @brief returns the dose for an index
				*/
				GenericValueType getValueAt(const VoxelGridIndex3D& aIndex) const override;

				const IDType getUID() const override
				{
					return _UID;
				};

			};
		}
	}
}

#endif
