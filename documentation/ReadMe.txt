/**
\page Build instructions

\section tests Compiler tests

RTToolbox is currently tested with the following compilers (only x64):
- Visual Studio 2013
- Visual Studio 2015
- GCC 4.9

\section linking Static/Dynamic library support

Can be changed with advanced option BUILD_SHARED_LIBS

For Windows:
- static linking: default
- dynamic linking: experimental 
For Linux:
- dynamic linking: default

\section thirdPartyLibraries Needed Third Party Libraries

Please load and compile the following third party libraries/tools:

Required:
1. CMake (version 3.1 or higher)
2. boost (version 1.58.0 or higher)

Optional:
3. DCMTK (with RT support - 3.6.1_20121102 or newer; if you want DICOM support)]
4. ITK (version 4.4 or higher; if you want DoseInterpolation support with itk::Transform or ITK File IO support)
5. MatchPoint (version 0.12 or higher, see http://mitk.org/download/thirdparty/MatchPoint_rev1610.tar.gz if you want DoseInterpolation support with MatchPoint Registration objects)
6. doxygen (if you want to generate a source code documentation)

\subsection installing Third party installation Instruction

Remark: To make sure everything runs smoothly, please make sure that all libraries and the RTToolbox
are either compiled with \MD or \MT flags.

If third party library packages cannot be found automatically, cmake will ask for them. 
Please give the location of the root folder, where the libraries where built for boost, DCMTK, ITK and MatchPoint.

[A. Boost]

A.1. Build (using the same compiler options as RTToolbox, usually STATIC LINKING and x64 architecture). The following components are needed: filesystem, regex, system, thread, regex and program_options.

[B. If you want DICOM support - DCMTK]

For Windows:

To compile DCMTK with \MD flags (standard for all other libs), you need to patch the CMAKE
options of DCMTK (<Path>\<to>\DCMTK\CMake\dcmtkPrepare.cmake), either by replacing "/MT" with "/MD"
or by explicitly replacing lines 135 to 171 with the following lines:


IF(DCMTK_OVERWRITE_WIN32_COMPILER_FLAGS AND NOT BUILD_SHARED_LIBS)

  
	# settings for Microsoft Visual Studio
  
	IF(CMAKE_GENERATOR MATCHES "Visual Studio .*")
    
		# get Visual Studio Version
    
		STRING(REGEX REPLACE "Visual Studio ([0-9]+).*" "\\1" VS_VERSION "${CMAKE_GENERATOR}")
    
		# these settings never change even for C or C++
    
		SET(CMAKE_C_FLAGS_DEBUG "/MDd /Z7 /Od")
    
		SET(CMAKE_C_FLAGS_RELEASE "/DNDEBUG /MD /O2")
    
		SET(CMAKE_C_FLAGS_MINSIZEREL "/DNDEBUG /MD /O2")
    
		SET(CMAKE_C_FLAGS_RELWITHDEBINFO "/DNDEBUG /MDd /Z7 /Od")

    
		SET(CMAKE_CXX_FLAGS_DEBUG "/MDd /Z7 /Od")
    
		SET(CMAKE_CXX_FLAGS_RELEASE "/DNDEBUG /MD /O2")
    
		SET(CMAKE_CXX_FLAGS_MINSIZEREL "/DNDEBUG /MD /O2")
    
		SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/DNDEBUG /MDd /Z7 /Od")
    
		# specific settings for the various Visual Studio versions
    
		IF(VS_VERSION EQUAL 6)
      
			SET(CMAKE_C_FLAGS "/nologo /W3 /GX /Gy /YX")
      
			SET(CMAKE_CXX_FLAGS "/nologo /W3 /GX /Gy /YX /Zm500") # /Zm500 increments heap size which is needed on some system to compile templates in dcmimgle
    
		ENDIF(VS_VERSION EQUAL 6)
    
		IF(VS_VERSION EQUAL 7)
      
			SET(CMAKE_C_FLAGS "/nologo /W3 /Gy")
      
			SET(CMAKE_CXX_FLAGS "/nologo /W3 /Gy")
    
		ENDIF(VS_VERSION EQUAL 7)
    
		IF(VS_VERSION GREATER 7)
      
			SET(CMAKE_C_FLAGS "/nologo /W3 /Gy /EHsc")
      
			SET(CMAKE_CXX_FLAGS "/nologo /W3 /Gy /EHsc")
    
		ENDIF(VS_VERSION GREATER 7)
  
	ENDIF(CMAKE_GENERATOR MATCHES "Visual Studio .*")


ENDIF(DCMTK_OVERWRITE_WIN32_COMPILER_FLAGS AND NOT BUILD_SHARED_LIBS)

B.2. Build DCMTK

[C. If you want ITKIO or DoseInterpolation support with itk::Transform - ITK]

C.1. Build ITK. The default configuration is fine.

[D. If you want support of DoseInterpolation with MatchPoint Registration objects - MatchPoint]

D.1. Build MatchPoint. The default configuration is fine.

RT-Toolbox installation Instruction
------------------------

[A. Configure/Build RTToolbox]

A.1. Configure with CMake

A.1.1. Set BOOST_INCLUDE_DIR and BOOST_DIR to the main boost directory (where "boost_build.jam" is located)

A.2. Select all packages you like to build (Parameters "BUILD_*"; e.g. BUILD_IO_Dicom). BUILD_All_Modules builds all modules (except Apps, Testing modules and Masks_Legacy).
	A short overview:
	- BUILD_IO_Dicom: Reading and writing of DICOM-RT files
	- BUILD_IO_HELAX: Reading of Helax DICOM files
	- BUILD_IO_ITK: Generic reading/writing with ITK
	- BUILD_Interpolation: Dose Interpolation
	- BUILD_InterpolationMatchPointTransformation: Dose Interpolation with Match Point registration support.
	- BUILD_Masks: Voxelization support
	- BUILD_Masks_Legacy: Legacy voxelization support. REMARK: This module is not supported anymore and may yield to wrong results.
	- BUILD_Models: Calculation of dosimetrical models like TCP, NTCP etc.
	- Some modules of RT-Toolbox are mandatory (RTTBCore) and build automatically.
   
A.2.1. If you select BUILD_IO_DICOM, the configuration will ask you for the DCMTK main directory (parameter DCMTK_DIR; where you have built DCMTK).
      REMARK: If you have built DCMTK as out source build you have to also set the CMake parameter DCMTK_SOURCE_DIR to the root directory of the DCMTK source.
	
A.2.2. If you select BUILD_Interpolation or BUILD_IO_ITK, set ITK_DIR to the binary directory of ITK.
	
A.2.3. If you select BUILD_InterpolationMatchPointTransformation, set MatchPoint_DIR to the binary directory of MatchPoint.

A.3 If you want to use the RTTB command line apps, select BUILD_Apps. You can select out of five apps.
	- BioModelCalc: calculate the radiobiological effect based on dose
	- DoseAcc: Do dose accumulation
	- DoseMap: Do dose mapping
	- DoseTool: Compute Dose statistics and DVH
	- VoxelizerTool: Voxelize an RTSTRUCT file

A.4. Generate CMake configuration

A.5. Build RTToolbox and have fun!

[B. Tests]

B.1. Enable BUILD_TESTING
B.2. Press Configure
B.3. Enable tests of interest (BUILD_Tester_All builds all tests)
B.5. Run tests (build RUN_TESTS project or type ctest) to ensure that everything is correct.

[C. Documentation]

C.1. Generate the documentation using doxygen (www.doxygen.org) and the configuration found in "RTTB_binary_dir/documentation/doxygen.config".

[D. Examples]

D.1. Enable BUILD_TESTING
D.2. Enable BUILD_RTToolbox_Test_examples
D.3. Run tests (build RUN_TESTS project) to also run the examples. These examples (located in testing/examples/) are a good starting point for own projects and show basic functionality of RTToolbox like DVH generation.

Remarks
-------
R.1. ITK Version: Only use one ITK version consistently throughout all libraries and RTToolbox! Otherwise, this will lead to linker errors.
**/