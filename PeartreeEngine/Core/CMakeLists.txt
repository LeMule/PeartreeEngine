cmake_minimum_required(VERSION 3.0)
project(Core)
message(STATUS "Configuring PearTree/${PROJECT_NAME}...")

set(OpenGL_GL_PREFERENCE "GLVND")

include_directories(
	include
	ThirdParty/glad/include
	ThirdParty/stb_image
	ThirdParty/KHR
	ThirdParty/glm
)

if (UNIX)
	find_package(OpenGL REQUIRED)
	find_package(SDL2 REQUIRED)
	include_directories(Core ${SDL2_INCLUDE_DIRS})
	include_directories(Core ${OPENGL_INCLUDE_DIRS})
endif (UNIX)

#---------------------------------------
# Link libraries
#---------------------------------------
link_directories(
if (WIN32)
	${CMAKE_CURRENT_SOURCE_DIR}/../Core/ThirdParty/SDL2/lib/x64
endif (WIN32)
	${CMAKE_CURRENT_SOURCE_DIR}/../Core/ThirdParty/Vulkan/Lib
)

#---------------------------------------
# Source/headers
#---------------------------------------
file(GLOB SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB PUBLIC_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
list(SORT SOURCE_FILES)
list(SORT PUBLIC_HEADERS)

file(GLOB THIRD_PARTY_CPP_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ThirdParty/stb_image/*.cpp)

if (WIN32)
	file(GLOB THIRD_PARTY_C_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ThirdParty/glad/src/*.c)
endif (WIN32)
if (UNIX)
	file(GLOB THIRD_PARTY_C_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ThirdParty/glad/glad/src/*.c)
endif (UNIX)


#---------------------------------------
# Create the Executable
#---------------------------------------
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PUBLIC_HEADERS} ${THIRD_PARTY_CPP_SOURCE_FILES} ${THIRD_PARTY_C_SOURCE_FILES})
target_link_libraries(
	${PROJECT_NAME}
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARIES}
	${CMAKE_DL_LIBS}
)

if (WIN32)
	add_custom_command(TARGET Core POST_BUILD
	    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        	"${PROJECT_SOURCE_DIR}/ThirdParty/SDL2/lib/x64/SDL2.dll"
	        $<TARGET_FILE_DIR:Core>)
endif (WIN32)

message(STATUS "Done Configuring PearTree/${PROJECT_NAME}...")
