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

#include <iterator>
#include <math.h>

#include "rttbDVH.h"
#include "rttbException.h"
#include "rttbInvalidParameterException.h"
#include "rttbUtils.h"

namespace rttb
{
	namespace core
	{

		DVH::~DVH() {}

		DVH::DVH(const DataDifferentialType& aDataDifferential, const DoseTypeGy& aDeltaD,
		         const DoseVoxelVolumeType& aDeltaV,
		         const IDType& aStructureID, const IDType& aDoseID): DVH(aDataDifferential, aDeltaD, aDeltaV, aStructureID, aDoseID, "NONE")
		{
		}

		DVH::DVH(const DataDifferentialType& aDataDifferential, DoseTypeGy aDeltaD,
		         DoseVoxelVolumeType aDeltaV,
		         const IDType& aStructureID, const IDType& aDoseID, const IDType& aVoxelizationID): _deltaD(aDeltaD), _deltaV(aDeltaV),
			_structureID(aStructureID), _doseID(aDoseID), _voxelizationID(aVoxelizationID)
		{
			_dataDifferential.clear();
			_dataDifferential = aDataDifferential;

			this->init();
		}

		DVH::DVH(const DVH& copy) : DVH(copy._dataDifferential, copy._deltaD, copy._deltaV, copy._structureID, copy._doseID, copy._voxelizationID)
		{
      _label = copy._label;
		}

		DVH& DVH::operator=(const DVH& copy)
		{
			if (this != &copy)
			{
				_deltaD = copy._deltaD;
				_deltaV = copy._deltaV;
				_structureID = copy._structureID;
				_doseID = copy._doseID;
				_voxelizationID = copy._voxelizationID;
				_label = copy._label;

				_dataDifferential.clear();
				_dataDifferential = copy._dataDifferential;

			}

			this->init();
			return *this;
		}

		bool operator==(const DVH& aDVH, const DVH& otherDVH)
		{
      bool result;
      //larger error constant because especially numberOfVoxels differ quite a bit after serialization
      const double errorConstantDVH = 1e-4;
      result = valueIsClose(aDVH.getDeltaD(), otherDVH.getDeltaD(), errorConstantDVH);
      result = result && valueIsClose(aDVH.getDeltaV(), otherDVH.getDeltaV(), errorConstantDVH);
      result = result && (aDVH.getDoseID() == otherDVH.getDoseID());
      result = result && (aDVH.getStructureID() == otherDVH.getStructureID());
      result = result && (aDVH.getVoxelizationID() == otherDVH.getVoxelizationID());
      result = result && valueIsClose(aDVH.getNumberOfVoxels(),otherDVH.getNumberOfVoxels(), errorConstantDVH);
      result = result && valueIsClose(aDVH.getMaximum(), otherDVH.getMaximum(), errorConstantDVH);
      result = result && valueIsClose(aDVH.getMinimum(), otherDVH.getMinimum(), errorConstantDVH);
      result = result && valueIsClose(aDVH.getMean(), otherDVH.getMean(), errorConstantDVH);
      result = result && (aDVH.getDataDifferential().size() == otherDVH.getDataDifferential().size());
      if (!result) {
        return result;
      }
      for (size_t i = 0; i < aDVH.getDataDifferential().size(); i++)
      {
        result = result
          && valueIsClose(aDVH.getDataDifferential().at(i), otherDVH.getDataDifferential().at(i),
            errorConstantDVH);
      }

			return result;
		}

		std::ostream& operator<<(std::ostream& s, const DVH& aDVH)
		{
			s << "[ " << aDVH.getStructureID() << ", " << aDVH.getDoseID() << ", " << aDVH.getVoxelizationID()
                << "\n " << "Number of Voxels: " <<
			  aDVH.getNumberOfVoxels() << " " << "Minimum/Maximum/Mean/Standard deviation: " << aDVH.getMinimum() << 
              ", " << aDVH.getMaximum() << ", " << aDVH.getMean() << ", " << aDVH.getStdDeviation() << " ]";
			return s;
		}


		std::deque<DoseCalcType> DVH::getDataDifferential(bool relativeVolume) const
		{
			if (!relativeVolume)
			{
				return _dataDifferential;
			}
			else
			{
				return _dataDifferentialRelative;
			}
		}

    std::deque<DoseCalcType> DVH::getDataCumulative(bool relativeVolume) const
    {
      if (!relativeVolume)
      {
        return _dataCumulative;
      }
      else
      {
        return _dataCumulativeRelative;
      }
    }

		DoseVoxelVolumeType DVH::getDeltaV() const
		{
			return _deltaV;
		}

		DoseTypeGy DVH::getDeltaD() const
		{
			return _deltaD;
		}

		IDType DVH::getDoseID() const
		{
			return this->_doseID;
		}

		IDType DVH::getStructureID() const
		{
			return this->_structureID;
		}

		IDType DVH::getVoxelizationID() const
		{
			return this->_voxelizationID;
		}

		void DVH::setDoseID(IDType aDoseID)
		{
			_doseID = aDoseID;
		}

		void DVH::setStructureID(IDType aStrID)
		{
			_structureID = aStrID;
		}

		DoseStatisticType DVH::getMaximum() const
		{
			return _maximum;
		}

		DoseStatisticType DVH::getMinimum() const
		{
			return _minimum;
		}

		DoseStatisticType DVH::getMean() const
		{
			return _mean;
		}

		DVHVoxelNumber DVH::getNumberOfVoxels() const
		{
			return _numberOfVoxels;
		}

		DoseStatisticType DVH::getStdDeviation() const
		{
			return _stdDeviation;
		}

		DoseStatisticType DVH::getVariance() const
		{
			return _variance;
		}

		void DVH::init()
		{
			if (_deltaD == 0 || _deltaV == 0)
			{
				throw InvalidParameterException("DVH init error: neither _deltaD nor _deltaV must be zero!");
			}

			if (this->_dataDifferential.empty())
			{
				throw InvalidParameterException("DVH init error: data differential is empty!");
			}

			double sum = 0;
			double squareSum = 0;
			_numberOfVoxels = 0;
			_maximum = 0;
			_minimum = 0;
			_dataCumulative.clear();
			_dataCumulativeRelative.clear();
			_dataDifferentialRelative.clear();

			DataDifferentialType::iterator it;

			int i = 0;

			for (it = _dataDifferential.begin(); it != _dataDifferential.end(); ++it)
			{
				_numberOfVoxels += (*it);

				if ((*it) > 0)
				{
					_maximum = (i + 0.5) * this->_deltaD;
				}

				if ((_minimum == 0.0f) && ((*it) > 0))
				{
					_minimum = (i + 0.5) * this->_deltaD;
				}

				sum += (*it) * (i + 0.5) * this->_deltaD;

				squareSum += (*it) * pow((i + 0.5) * this->_deltaD, 2);

				i++;
			}

			_mean = sum / _numberOfVoxels;

			for (it = _dataDifferential.begin(); it != _dataDifferential.end(); ++it)
			{
				DoseCalcType datai = ((*it) * 1.0 / _numberOfVoxels);
				_dataDifferentialRelative.push_back(datai);
			}

			_variance = (squareSum / _numberOfVoxels - _mean * _mean);
			_stdDeviation = pow(_variance, 0.5);

			this->calcCumulativeDVH();
		}

		void DVH::calcCumulativeDVH()
		{

			_dataCumulative.clear();
			_dataCumulativeRelative.clear();

			DoseCalcType cumulativeDVHi = 0;

			size_t size = _dataDifferential.size();

			for (size_t i = 0; i < size; i++)
			{
				cumulativeDVHi += _dataDifferential.at(size - i - 1);
				_dataCumulative.push_front(cumulativeDVHi);
				_dataCumulativeRelative.push_front(cumulativeDVHi / this->getNumberOfVoxels());
			}
		}

		DoseStatisticType DVH::getMedian() const
		{

			double median_voxel = 0;
			int median_i = 0;

			for (GridIndexType i = 0; i < this->_dataDifferential.size(); i++)
			{
				if (median_voxel < (_numberOfVoxels - median_voxel))
				{
					median_voxel += _dataDifferential[i];
					median_i = i;
				}
			}

			double median = (median_i + 0.5) * this->_deltaD;
			return median;
		}

		DoseStatisticType DVH::getModal() const
		{

			double modal_voxel = 0;
			int modal_i = 0;

			for (GridIndexType i = 0; i < this->_dataDifferential.size(); i++)
			{
				if (modal_voxel < _dataDifferential[i])
				{
					modal_voxel = _dataDifferential[i];
					modal_i = i;
				}
			}

			double modal = (modal_i + 0.5) * this->_deltaD;
			return modal;
		}

		VolumeType DVH::getVx(DoseTypeGy xDoseAbsolute) const
		{

			GridIndexType i = static_cast<GridIndexType>(xDoseAbsolute / _deltaD);

			if (i < _dataCumulative.size())
			{
				VolumeType vx = (_dataCumulative.at(i));
				vx = (vx * this->_deltaV);
				return vx;
			}
			else if (i < _dataCumulativeRelative.size())
			{
				VolumeType vx = (_dataCumulativeRelative.at(i));
				vx = (vx * this->_deltaV);
				return vx;
			}
			else
			{
				return 0;
			}
		}

		DoseTypeGy DVH::getDx(VolumeType xVolumeAbsolute) const
		{

			GridIndexType i = 0;

			if (!_dataCumulative.empty())
			{
				for (; i < _dataCumulative.size(); i++)
				{
					double volumeAbsoluteI = _dataCumulative[i] * this->_deltaV;

					if (xVolumeAbsolute > volumeAbsoluteI)
					{
						break;
					}
				}
			}
			else
			{
				for (; i < _dataCumulativeRelative.size(); i++)
				{
					double volumeAbsoluteI = _dataCumulativeRelative[i] * this->_deltaV;

					if (xVolumeAbsolute / this->getNumberOfVoxels() > volumeAbsoluteI)
					{
						break;
					}
				}
			}

			if (i <= _dataCumulative.size() && i > 0)
			{
				DoseTypeGy dx = (i - 1) * this->_deltaD;
				return dx;
			}
			else if (i < _dataCumulativeRelative.size() && i > 0)
			{
				DoseTypeGy dx = (i - 1) * this->_deltaD;
				return dx;
			}
			else
			{
				return 0;
			}
		}

		VolumeType DVH::getAbsoluteVolume(int relativePercent) const
		{
			return (relativePercent * getNumberOfVoxels() * getDeltaV() / 100.0);
		}

		void DVH::setLabel(StructureLabel aLabel)
		{
			_label = aLabel;
		}

		StructureLabel DVH::getLabel() const
		{
			return _label;
		}

		std::map <DoseTypeGy, PercentType> DVH::getNormalizedDVH(DVHType dvhType) const {
			std::map <DoseTypeGy, PercentType> normalizedDVH;
			DataDifferentialType data;
			
			if (dvhType.Type == DVHType::Cumulative) {
				data = getDataCumulative();
			}
      else {
        data = getDataDifferential();
      }

      if (data.empty()) {
        throw InvalidParameterException("DVH data is empty. Can't retrieve normalized DVH");
      }

			for (size_t i = 0; i < data.size(); i++)
			{
				normalizedDVH.insert(std::pair<DoseTypeGy, PercentType>(i * getDeltaD(), data[i] * getDeltaV()));
			}
			return normalizedDVH;
		}

	}//end namespace core
}//end namespace rttb
