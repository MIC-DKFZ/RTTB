// -----------------------------------------------------------------------
//
// (c) Copyright 2008  company=DKFZ";  location="Heidelberg ; Germany"
// ALL RIGHTS RESERVED
//
// THIS FILE CONTAINS CONFIDENTIAL AND PROPRIETARY INFORMATION OF DKFZ.
// ANY DUPLICATION; MODIFICATION; DISTRIBUTION; OR
// DISCLOSURE IN ANY FORM; IN WHOLE; OR IN PART; IS STRICTLY PROHIBITED
// WITHOUT THE PRIOR EXPRESS WRITTEN PERMISSION OF DKFZ.
// -----------------------------------------------------------------------
// @file VOIindexIdentifier.cpp
// @version
// @date
// @author $Author$
// @author $Author$
// @author
// Subversion HeadURL: $HeadURL$


// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// !!!EXPERIMENTAL CODE!!!
//
// This code may not be used for release.
// Add #define SIDT_ENFORCE_MATURE_CODE to any release module
// to ensure this policy.
// -----------------------------------------------------------------------


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




        void VOIindexIdentifier::init(StructSetTypePointer spInStructSet)
        {
            this->_spStructSet.reset();
            this->_voiLabelList.clear();
            this->_spStructSet = spInStructSet;

            for (int i = 0; i < this->_spStructSet->getNumberOfStructures(); i++)
            {
                this->_voiLabelList.push_back(this->_spStructSet->getStructure(i)->getLabel());
            }
        }


        const unsigned int VOIindexIdentifier::getIndexByVoiName(StructSetTypePointer spStructSet,
                std::string name)
        {
            if (!spStructSet)
            {
              rttbDefaultExceptionMacro("invalid method call spStructSet invalid");
            }
            this->init(spStructSet);
            int returnValue = -1;

            if (this->_voiLabelList.empty())
            {
              rttbDefaultExceptionMacro("invalid method call, object state invalid, voiLabelList empty");
            }


            ::boost::regex optionalNamesREG("([^\|]*)([\|\|][^\|]*)+");
            ::boost::smatch what;

            typedef std::vector< std::string > OptionalVectorType;
            OptionalVectorType optionalVois;
            ::boost::split(optionalVois, name, ::boost::is_any_of("||"));

            for (OptionalVectorType::iterator i = optionalVois.begin(); i != optionalVois.end(); i++)
            {
                int counter = 0;

                for (VoiLabelList::const_iterator iter = this->_voiLabelList.begin();
                     iter != this->_voiLabelList.end(); iter++)
                {
                    if ((*iter).compare(*i) == 0)
                    {
                        returnValue = counter;
                    }

                    counter++;
                }

            }

            return returnValue;
        }

        const std::string VOIindexIdentifier::getVoiNameByIndex(StructSetTypePointer spStructSet,
                const unsigned int index)
        {
            if (!spStructSet)
            {
              rttbDefaultExceptionMacro("invalid method call spStructSet invalid");
            }
            this->init(spStructSet);

            if (this->_voiLabelList.empty())
            {
                rttbDefaultExceptionMacro("invalid method call, object state invalid, voiLabelList empty");
            }

            if (index >= this->_voiLabelList.size())
            {
                rttbDefaultExceptionMacro("invalid index, index out of range");
            }

            return  this->_voiLabelList[index];
        }
    }
}

