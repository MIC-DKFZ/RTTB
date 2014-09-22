RTToolbox - Read Me
----------------------------
----------------------------


Needed Third Party Libraries
----------------------------

Please load and compile the following third party libraries/tool:

1. CMake (version 2.8 or higher)
2. Litmus (in this distribution; see \utilities\Litmus; if you want tests) 
3. dcmtk (with RT support - 3.6.1 or newer; if you want dicom support) 
4. boost (version 1.52.0 or higher)
5. doxygen (if you want to generate a source code documentation)

Installation Instruction
------------------------

Remark: To make sure everything runs smoothly, please make sure that all libraries and the RTToolbox
are either compiled with \MD or \MT flags.

If third party library packages cannot be found automatically, cmake will ask for them. 
Please give the location of the root folder, where the libraries where built for dcmtk, and Litmus.


[A. If you build tests - Litmus]

A.1. Configure Litmus with CMake (seperated binary folder recommended)
A.2. Build Litmus

[B. If you want DICOM support - DCMTK]

For Windows:

To compile DCMTK with \MD flags (standard for all other libs), you need to patch the CMAKE
options of DCMTK (<Path>\<to>\DCMTK\CMake\dcmtkPrepare.cmake), either by replacing "/MT" with "/MD"
or by expliciltly replacing lines 135 to 171 with the following lines:


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

  
	# settings for Borland C++
  
	IF(CMAKE_GENERATOR MATCHES "Borland Makefiles")
    
		# further settings required? not tested for a very long time!
    
		SET(CMAKE_STANDARD_LIBRARIES "import32.lib cw32md.lib")
  
	ENDIF(CMAKE_GENERATOR MATCHES "Borland Makefiles")


ENDIF(DCMTK_OVERWRITE_WIN32_COMPILER_FLAGS AND NOT BUILD_SHARED_LIBS)

B.1. Configure DCMTK with CMake
For unix-like systems: Set the CMake parameter DCMTK_FORCE_FPIC_ON_UNIX to true (It is an advanced parameter)

B.2. Build DCMTK


[C. Configure/Build RTToolbox]

C.1 Configure with CMake

C.1.1 Set BOOST_INCLUDE_DIR to the main boost directory (where "boost_build.jam" is located)
      REMARK: For the current version of the RTToolbox you do need to build boost only if you want to use the provided Helax-Dicom data support. 
      Otherwise you can use headers only.

C.2 Select all packages you like to build (Parameters "BUILD_*"; e.g. BUILD_IO_Dicom)
   
C.2.1 If you select BUILD_IO_DICOM the configuration will ask you for the DCMTK main directory (parameter DCMTK_DIR; where you have built DCMTK).
      REMARK: If you have built DCMTK as out source build you have to also set the
      CMake parameter DCMTK_SOURCE_DIR to the root directory of the DCMTK source.

C.3 Generate CMake configuration

C.4 Build RTToolbox and have fun!


[D. Documentation]

D.1 Generate the documentation using doxygen and the configuration found in "RTTB_binary_dir/documentation/doxygen.config".


Remarks
-------
R.1 Virtuos Support: Virtuos is a proprietary data format and therefore not included in the RTToolbox. The virtuos support (wrapper) for the RTToolbox is currently being restructured and will be included in the near future.

R.2 Database support: The database support for the RTToolbox is currently being restructured and will be included in the near future.
