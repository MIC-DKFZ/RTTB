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
// @version $Revision: 1221 $ (last changed revision)
// @date    $Date: 2015-12-01 13:43:31 +0100 (Di, 01 Dez 2015) $ (last change date)
// @author  $Author: hentsch $ (last changed by)
*/


#ifndef __VoxelizerApplicationData_h
#define __VoxelizerApplicationData_h

#include "boost/shared_ptr.hpp"

#include "rttbDoseAccessorInterface.h"
#include "rttbStructureSetGeneratorInterface.h"

#include <vector>
#include <string>

namespace rttb
{
  namespace apps
  {
    namespace voxelizerTool
    {
      class VoxelizerCmdLineParser;
      /*! @class ApplicationData
      @brief Class for storing all relevant variables needed in VoxelizerTool
      */
      class ApplicationData
      {
      public:
        typedef std::vector<std::string> LoadingStyleArgType;
        core::DoseAccessorInterface::Pointer _dose;
        core::StructureSetGeneratorInterface::StructureSetPointer _struct;
        std::string _structFile;
        std::string _referenceFile;
        std::string _outputFilename;
        std::string _regEx;
        LoadingStyleArgType _referenceFileLoadStyle;
        LoadingStyleArgType _structFileLoadStyle;
        bool _multipleStructs;
        bool _binaryVoxelization;
        bool _addStructures;
        bool _noStrictVoxelization;

        /*! @brief Resets the variables.
        */
        void reset();

        ApplicationData();
      };
      /*! @brief Reads the necessary arguments from the VoxelizerCmdLineParser and writes them in the respective variables of ApplicationData.
      */
      void populateAppData(boost::shared_ptr<VoxelizerCmdLineParser> argParser, ApplicationData& appData);
    }
  }
}
#endif
