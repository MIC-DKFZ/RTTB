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

#ifndef __STRUCTURE_SET_GENERATOR_INTERFACE_H
#define __STRUCTURE_SET_GENERATOR_INTERFACE_H

#include <rttbCommon.h>

#include "rttbStructureSet.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

namespace rttb
{
	namespace core
	{
		/*! @class StructureSetGeneratorInterface
			@brief Interface for all structure set generating classes
		*/
		class StructureSetGeneratorInterface
		{
		public:
      rttbClassMacroNoParent(StructureSetGeneratorInterface);

		private:
			StructureSetGeneratorInterface(const
			                               StructureSetGeneratorInterface&) = delete; //not implemented on purpose -> non-copyable
			StructureSetGeneratorInterface& operator=(const
			        StructureSetGeneratorInterface&) = delete;//not implemented on purpose -> non-copyable


		protected:
			StructureSetGeneratorInterface() = default;
			virtual ~StructureSetGeneratorInterface() = default;

    private:
      bool _activeFilter{false};
      std::string _filterRegEx;

		public:

      void setStructureLabelFilterActive(bool active)
      {
        _activeFilter = active;
      };

      bool getStructureLabelFilterActive() const
      {
        return _activeFilter;
      };

      void setFilterRegEx(const std::string& filter)
      {
        _filterRegEx = filter;
      };

      std::string getFilterRegEx() const
      {
        return _filterRegEx;
      };

			/*! @brief Generate StructureSet
				@return Return shared pointer of StructureSet.
			*/
			virtual StructureSet::Pointer generateStructureSet() = 0;
		};
	}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
