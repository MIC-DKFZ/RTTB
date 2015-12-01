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

#include "DoseIteratorInterface_LEGACY.h"
using namespace std;

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <iostream>
#include <sstream>

namespace rttb
{

	namespace masks
	{
		namespace legacy
		{

			IDType DoseIteratorInterface::getDoseUID()
			{
				boost::uuids::uuid id;
				boost::uuids::random_generator generator;
				id = generator();

				std::stringstream ss;
				ss << id;

				_doseUID = ss.str();
				return _doseUID;

			}

			IDType DoseIteratorInterface::getPatientUID()
			{
				boost::uuids::uuid id;
				boost::uuids::random_generator generator;
				id = generator();

				std::stringstream ss;
				ss << id;

				_patientUID = ss.str();
				return _patientUID;

			}

			int DoseIteratorInterface::size()
			{
				this->start();
				int size = 0;

				while (this->hasNextVoxel())
				{
					this->next();
					size++;
				}

				return size;
			}
		}

	}
}










