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

#include <time.h>

#include "DummyDVHGenerator.h"
namespace rttb
{
	namespace testing
	{

		DummyDVHGenerator::DummyDVHGenerator(): _binSize(DoseTypeGy(0.1)), _voxelVolume(8), _value(0)
		{
			/* initialize random seed: */
			srand(static_cast<unsigned int>(time(nullptr)));
		};

		core::DVH DummyDVHGenerator::generateDVH(IDType structureID, IDType doseID)
		{
			core::DVH::DataDifferentialType aDataDifferential;

			for (int i = 0; i < 100; i++)
			{
				_value = DoseCalcType((double(rand()) / RAND_MAX) * 1000);
				//cut off values to avoid problems on comparisson with reimported values after
				//writing to file.
				_value = floor(_value * 1000000) / 1000000;
				aDataDifferential.push_back(_value);
			}

			return core::DVH(aDataDifferential, _binSize, _voxelVolume, structureID, doseID);
		}

		core::DVH DummyDVHGenerator::generateDVH(IDType structureID, IDType doseID, DoseCalcType value)
		{
			_value = value;

			core::DVH::DataDifferentialType aDataDifferential;

			for (int i = 0; i < 100; i++)
			{
				aDataDifferential.push_back(_value);
			}

			return core::DVH(aDataDifferential, _binSize, _voxelVolume, structureID, doseID);
		}

		core::DVH DummyDVHGenerator::generateDVH(IDType structureID, IDType doseID, DoseCalcType minValue,
		        DoseCalcType maxValue)
		{
			_voxelVolume = 0.2 * 0.2 * 0.4;
			bool decrease = false;

			core::DVH::DataDifferentialType aDataDifferential;

			for (int i = 0; i <= 200; i++)
			{
				if ((i > 20) && (i < 180))
				{
					if ((_value == 0) && (!decrease))
					{
						_value = DoseCalcType((double(rand()) / RAND_MAX) * 10) + minValue;
					}
					else if (!decrease)
					{
						_value = _value + DoseCalcType((double(rand()) / RAND_MAX) * (maxValue / 10));
					}

					if ((_value > maxValue) || (decrease))
					{
						decrease = true;
						_value = _value - DoseCalcType((double(rand()) / RAND_MAX) * (maxValue / 3));
					}

					if (_value < 0)
					{
						_value = 0;
					}
				}
				else
				{
					_value = 0;
				}

				aDataDifferential.push_back(_value);
			}

			return core::DVH(aDataDifferential, _binSize, _voxelVolume, structureID, doseID);
		}
	}
}