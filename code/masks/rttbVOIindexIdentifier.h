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
// @file VOIindexIdentifier.h
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



#ifndef __RTTB_MASKS_VOIINDEXIDENTIFIER_H
#define __RTTB_MASKS_VOIINDEXIDENTIFIER_H

#include "rttbStructureSet.h"
#include "rttbStructure.h"
#include <boost/scoped_ptr.hpp>




namespace rttb
{
    namespace masks
    {
        class VOIindexIdentifier
        {

        public:
            typedef ::boost::shared_ptr<VOIindexIdentifier>                            Pointer;
            typedef ::rttb::core::StructureSet                                         StructSetType;
            typedef ::boost::shared_ptr<StructSetType>                                 StructSetTypePointer;
            typedef ::rttb::core::Structure                                            StructType;
            typedef StructType::StructTypePointer                                      StructTypePointer;
            typedef std::vector<std::string>                                           VoiLabelList;


        public:

            VOIindexIdentifier(){}
            virtual ~VOIindexIdentifier() {}


            /*!@brief get the index of the corresponding VOI
            * @param valid struct must be loaded
            * @param std::string filename
            * @return the index */
            const unsigned int getIndexByVoiName(StructSetTypePointer spStructSet,
                                                 const std::string name);


            /*!@brief get the VOI of the corresponding index
            * @param valid struct must be loaded
            * @param std::string filename
            * @return voi name */
            const std::string getVoiNameByIndex(StructSetTypePointer spStructSet,
                                                const unsigned int index);

        private:
            void init(StructSetTypePointer spStructSet);

            StructSetTypePointer                    _spStructSet;
            VoiLabelList                            _voiLabelList;
        };

    }
}

#endif __RTTB_MASKS_VOIINDEXIDENTIFIER_H
