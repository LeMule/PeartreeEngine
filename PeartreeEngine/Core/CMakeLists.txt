project(Core)
message(STATUS "Configuring PearTree/${PROJECT_NAME}...")

include_directories(
	include
	ThirdParty/assimp-5.0.1/include
	ThirdParty/glad/include
	ThirdParty/glfw-3.3.2/include
	ThirdParty/stb_image
	ThirdParty/KHR
	ThirdParty/glm
)

#---------------------------------------
# Link libraries
#---------------------------------------
link_directories(
	../Core/ThirdParty/assimp-5.0.1/bin/Debug
	../Core/ThirdParty/glfw-3.3.2/lib/Debug
)

#---------------------------------------
# Source/headers
#---------------------------------------
file(GLOB SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB PUBLIC_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
list(SORT SOURCE_FILES)

file(GLOB THIRD_PARTY_SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ThirdParty/glad/src/*.c ThirdParty/stb_image/*.cpp)

#---------------------------------------
# Create the Executable
#---------------------------------------
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${PUBLIC_HEADERS} ${THIRD_PARTY_SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} 
						PUBLIC OpenGL32
						glfw3
						assimp-vc142-mtd)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

add_custom_command(TARGET Core POST_BUILD        		 									# Adds a post-build event to Core
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  		 									# which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/ThirdParty/assimp-5.0.1/bin/Debug/assimp-vc142-mtd.dll"      # <--this is in-file
        $<TARGET_FILE_DIR:Core>)                 		 									# <--this is out-file path

message(STATUS "Done Configuring PearTree/${PROJECT_NAME}...")
