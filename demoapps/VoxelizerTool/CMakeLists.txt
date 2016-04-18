MESSAGE (STATUS "generating demo app: VoxelizerTool - writing files of voxelized structures")
SET(RTTB_Boost_ADDITIONAL_COMPONENT program_options)
RTTB_CREATE_APPLICATION(VoxelizerTool DEPENDS RTTBITKIO RTTBOTBMask RTTBBoostMask RTTBDicomIO RTTBMasks RTTBCore PACKAGE_DEPENDS ITK BoostBinaries)
IF (NOT WIN32)
	#CMake 3.1 provides target_compile_features(RTTB_Interpolation cxx_auto_type cxx_nullptr cxx_override) to 			automatically add required compiler flags
	set(CMAKE_CXX_FLAGS "-std=c++11 -fpermissive")
ENDIF()
