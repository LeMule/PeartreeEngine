cmake_minimum_required (VERSION 3.0)
project (PearTree)
message(STATUS "Configuring ${PROJECT_NAME}...")

set_property(GLOBAL PROPERTY USE_FOLDERS ON) #this will permit logical folder groupings within visual studio

if(NOT MSVC AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Debug")
endif()

set(BIN_TARGET_SYSTEM ${CMAKE_SYSTEM_NAME}) #CMAKE_SYSTEM_NAME is an internal cmake variable to describe the system ie: Windows, Linux, OSX, etc
if(WIN32)
	set(BIN_TARGET_SYSTEM Win32)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  bin/${BIN_TARGET_SYSTEM}/${CMAKE_BUILD_TYPE})
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(LIBRARY_OUTPUT_PATH bin/${BIN_TARGET_SYSTEM}/${CMAKE_BUILD_TYPE})

#------------------------------------------------------------
# Preprocessor Definitions
#------------------------------------------------------------
set(DEFAULT_DEFINITIONS 
	$<$<CONFIG:Debug>:_DEBUG>
	$<$<CONFIG:Release>:RELEASE;>
)

#---------------------------------------------------------
# Now Create the Project
#---------------------------------------------------------
#Iterate over all subfolders to include any available
file(GLOB SUBFOLDER_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/Core)
foreach(PEARTREE_PROJECT ${SUBFOLDER_LIST})
	if(EXISTS ${PEARTREE_PROJECT}/CMakeLists.txt)
		add_subdirectory(${PEARTREE_PROJECT})
		message(STATUS "Adding Subdirectory ${PEARTREE_PROJECT}.")
	endif()
endforeach()

message(STATUS "PearTree Configurations Completed Successfully.")
