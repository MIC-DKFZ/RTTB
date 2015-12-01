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

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "rttbStructure.h"
#include "rttbNullPointerException.h"
#include "rttbInvalidParameterException.h"

namespace rttb
{
	namespace core
	{

		/*! Compares two polygons in the same plane.
			Helper function for sorting of polygons.
		*/
		bool comparePolygon(PolygonType A, PolygonType B)
		{
			PolygonType::iterator it;

			for (it = A.begin(); it != A.end(); ++it)
			{
				if ((*it)(2) != A.at(0)(2))
				{
					throw std::range_error("Error: A must in the same _z plane!");
				}
			}

			PolygonType::iterator it2;

			for (it2 = B.begin(); it2 != B.end(); ++it2)
			{
				if ((*it2)(2) != B.at(0)(2))
				{
					throw std::range_error("Error: B must in the same _z plane!");
				}
			}

			if (A.size() == 0 || B.size() == 0)
			{
				throw std::range_error("Error: A and B must not be empty!");
			}

			return (A.at(0)(2) < B.at(0)(2));

		}

		Structure::Structure() : _structureVector(0), _label("None")
		{
			if (_strUID.empty())
			{
				boost::uuids::uuid id;
				boost::uuids::random_generator generator;
				id = generator();

				std::stringstream ss;
				ss << id;

				_strUID = ss.str();
			}
		}

		Structure::Structure(PolygonSequenceType strVector) : _structureVector(0), _label("None")
		{
			if (_strUID.empty())
			{
				boost::uuids::uuid id;
				boost::uuids::random_generator generator;
				id = generator();

				std::stringstream ss;
				ss << id;

				_strUID = ss.str();
			}

			_structureVector = strVector;

			sort(_structureVector.begin(), _structureVector.end(), comparePolygon);
		}


		Structure::Structure(const Structure& copy) : _structureVector(copy.getStructureVector()),
			_strUID(copy.getUID()),
			_label(copy.getLabel())
		{
		}

		Structure::~Structure() {}

		const PolygonSequenceType& Structure::getStructureVector() const
		{
			return _structureVector;
		}



		int Structure::getNumberOfEndpoints() const
		{
			int count = 0;
			PolygonSequenceType::const_iterator itVV;

			for (itVV = _structureVector.begin(); itVV != _structureVector.end(); ++itVV)
			{
				count += (int)(*itVV).size();
			}

			return count;
		}

		IDType Structure::getUID() const
		{

			return _strUID;
		}

		void Structure::setUID(const IDType& aUID)
		{
			_strUID = aUID;
		}

		void Structure::setLabel(const StructureLabel& aLabel)
		{
			_label = aLabel;
		}

		StructureLabel Structure::getLabel() const
		{
			return _label;
		}

	}//end namespace core
}//end namespace rttb
