cmake_minimum_required(VERSION 3.6)

project(Pathtracer CXX)

# CMake variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INSTALL_MESSAGE LAZY)
set(ENGINE_CMAKE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(FORCE_REBUILD "Should rebuild pre-built libs" OFF)

# Project Variables
set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Compiler Flags
if (MSVC)
    set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /EHsc /W4 /experimental:external /external:anglebrackets /external:W0")
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Ob0 /Od /RTC1 /DDEBUG /DSDL_ASSERT_LEVEL=2")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /Ox /Ob2 /DRELEASE /DSDL_ASSERT_LEVEL=0")
else ()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -fno-exceptions")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG -DSDL_ASSERT_LEVEL=2")
    set(CMAKE_CXX_FLAGS_RELEASE "-DRELEASE -DSDL_ASSERT_LEVEL=0")
endif ()

# Dependencies
include(${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/glad.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/libs/sdl/sdl2.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/libs/eastl/eastl.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/libs/glm/glm.cmake)
#add_subdirectory("libs/fast_obj")

# Generate constants file
configure_file(${SOURCE_DIR}/generated/CMakeConstants.cpp.in ${SOURCE_DIR}/generated/CMakeConstants.cpp)

# Get lazy and glob all sources
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.hpp")
set (SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Pathtracer.natvis")

# Glob all shader sources so that they also show in the IDEs
set(SHADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res/shaders")
file(GLOB_RECURSE SHADERS "${SHADERS_DIR}/*.frag" "${SHADERS_DIR}/*.vert")

# Create our executable
add_executable(${PROJECT_NAME} ${SOURCES} ${SHADERS})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main glm::glm EASTL)
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})

# Project settings
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Source group generation for IDEs
foreach (SOURCE IN ITEMS ${SOURCES})
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    file(RELATIVE_PATH SOURCE_PATH_REL "${SOURCE_DIR}/.." "${SOURCE_PATH}")
    string(REPLACE "/" "\\" GROUP_PATH "${SOURCE_PATH_REL}")
    source_group("${GROUP_PATH}" FILES "${SOURCE}")
endforeach ()
foreach (SOURCE IN ITEMS ${SHADERS})
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    file(RELATIVE_PATH SOURCE_PATH_REL "${RESOURCES_DIR}/.." "${SOURCE_PATH}")
    string(REPLACE "/" "\\" GROUP_PATH "${SOURCE_PATH_REL}")
    source_group("${GROUP_PATH}" FILES "${SOURCE}")
endforeach ()

# Copy resources to build dir
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCES_DIR} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res")