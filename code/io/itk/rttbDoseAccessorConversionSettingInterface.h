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
#ifndef __DOSE_ACCESSOR_CONVERSION_SETTING_INTERFACE_H
#define __DOSE_ACCESSOR_CONVERSION_SETTING_INTERFACE_H

namespace rttb
{
	namespace core
	{
		/*! @class DoseAccessorConversionSettingInterface
			@brief Interface for specifying settings for dose accessor converters (e.g. how to handle invalid dose voxels)
		*/
		class DoseAccessorConversionSettingInterface
		{
		public:
			/* Defines if the conversion process should fail (with an exception) if an invalid id/voxel occurs.*/
			void setFailOnInvalidIDs(bool failOnInvalid)
			{
				_failedOnInvalidID = failOnInvalid;
			}

			/* Indicates how the conversion should handle invalid ids/voxels.\n
			 * true: fails with an exception
			 * false: uses the specified "invalid dose value".*/
			bool failsOnInvalidIDs() const
			{
				return _failedOnInvalidID;
			}

			/* Sets the value that should be used for invalid ids/voxels.*/
			void setInvalidDoseValue(DoseTypeGy value)
			{
				_invalidDoseValue = value;
			}

			/* Returns the value that is used for invalid ids/voxels.*/
			DoseTypeGy getInvalidDoseValue() const
			{
				return _invalidDoseValue;
			}

			DoseAccessorConversionSettingInterface() {};
			virtual ~DoseAccessorConversionSettingInterface() {};

		private:
			DoseAccessorConversionSettingInterface(const
			                                       DoseAccessorConversionSettingInterface&); //not implemented on purpose -> non-copyable
			DoseAccessorConversionSettingInterface& operator=(const
			        DoseAccessorConversionSettingInterface&);//not implemented on purpose -> non-copyable

		protected:

			bool _failedOnInvalidID{false};
			DoseTypeGy _invalidDoseValue{-1.0};
		};
	}
}

#endif
