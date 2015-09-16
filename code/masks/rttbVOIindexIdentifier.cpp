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
// @version $Revision:
// @date    $Date:
// @author  $Author:
*/


#include "rttbVOIindexIdentifier.h"
#include "rttbStructureSet.h"
#include "rttbStructure.h"
#include "rttbExceptionMacros.h"
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>



namespace rttb
{
  namespace masks
  {

    const unsigned int VOIindexIdentifier::getIndexByVoiName(StructSetTypePointer spStructSet,
      const std::string& name)
    {

      if (!spStructSet)
      {
        std::ostringstream message; 
        message << "Exception: "<< "invalid method call spStructSet invalid";
        ::rttb::core::Exception e_(message.str().c_str()); 
        throw e_;
      }


      VoiLabelList voiLabelList;

      for (int i = 0; i < spStructSet->getNumberOfStructures(); i++)
      {
        voiLabelList.push_back(spStructSet->getStructure(i)->getLabel());
      }



      int returnValue = -1;

      if (voiLabelList.empty())
      {
        std::ostringstream message; 
        message << "Exception: "<< "invalid method call, object state invalid, voiLabelList empty";
        ::rttb::core::Exception e_(message.str().c_str()); 
        throw e_;
      }


      ::boost::regex optionalNamesREG("([^\|]*)([\|\|][^\|]*)+");
      ::boost::smatch what;

      typedef std::vector< std::string > OptionalVectorType;
      OptionalVectorType optionalVois;
      ::boost::split(optionalVois, name, ::boost::is_any_of("||"));

      for (OptionalVectorType::iterator i = optionalVois.begin(); i != optionalVois.end(); i++)
      {
        int counter = 0;

        /*
          Searches for valid entries in the voi list.
          Takes the first matching entry!
        */
        for (VoiLabelList::const_iterator iter = voiLabelList.begin();
          iter != voiLabelList.end(); iter++)
        {
          if ((*iter).compare(*i) == 0)
          {
            returnValue = counter;
          }

          counter++;
        }

      }

      if (returnValue == -1)
      {
        std::ostringstream message; 
        message << "Exception: "<< "specified voi name not found!";
        ::rttb::core::Exception e_(message.str().c_str()); 
        throw e_;
      }

      return returnValue;
    }

    const std::string VOIindexIdentifier::getVoiNameByIndex(StructSetTypePointer spStructSet,
      const unsigned int& index)
    {
      if (!spStructSet)
      {
        std::ostringstream message; 
        message << "Exception: "<< "invalid method call spStructSet invalid";
        ::rttb::core::Exception e_(message.str().c_str()); 
        throw e_;
      }

      VoiLabelList voiLabelList;

      for (int i = 0; i < spStructSet->getNumberOfStructures(); i++)
      {
        voiLabelList.push_back(spStructSet->getStructure(i)->getLabel());
      }

      if (voiLabelList.empty())
      {
        std::ostringstream message; 
        message << "Exception: "<< "invalid method call, object state invalid, voiLabelList empty";
        ::rttb::core::Exception e_(message.str().c_str()); 
        throw e_;
      }

      if (index >= voiLabelList.size())
      {
        std::ostringstream message; 
        message << "Exception: "<< "invalid index, index out of range";
        ::rttb::core::Exception e_(message.str().c_str()); 
        throw e_;
      }

      return  voiLabelList[index];
    }
  }
}

