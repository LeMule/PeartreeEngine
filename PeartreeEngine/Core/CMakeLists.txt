cmake_minimum_required(VERSION 3.0)
project(Core)
message(STATUS "Configuring PearTree/${PROJECT_NAME}...")

include_directories(
	include
	ThirdParty/assimp-5.0.1/include
	ThirdParty/glad/include
	ThirdParty/SDL2/include
	ThirdParty/stb_image
	ThirdParty/KHR
	ThirdParty/glm
)

#---------------------------------------
# Link libraries
#---------------------------------------
link_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../Core/ThirdParty/assimp-5.0.1/bin/Debug
	${CMAKE_CURRENT_SOURCE_DIR}/../Core/ThirdParty/SDL2/lib/x64
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
file(GLOB THIRD_PARTY_C_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ThirdParty/glad/src/*.c)

#---------------------------------------
# Create the Executable
#---------------------------------------
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PUBLIC_HEADERS} ${THIRD_PARTY_CPP_SOURCE_FILES} ${THIRD_PARTY_C_SOURCE_FILES})
target_link_libraries(
	${PROJECT_NAME} 
	PUBLIC OpenGL32
	sdl2
	assimp-vc142-mtd
)

add_custom_command(TARGET Core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/ThirdParty/assimp-5.0.1/bin/Debug/assimp-vc142-mtd.dll"
        $<TARGET_FILE_DIR:Core>)
		
add_custom_command(TARGET Core POST_BUILD        		 								
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  		 									
        "${PROJECT_SOURCE_DIR}/ThirdParty/SDL2/lib/x64/SDL2.dll"
        $<TARGET_FILE_DIR:Core>) 

message(STATUS "Done Configuring PearTree/${PROJECT_NAME}...")
