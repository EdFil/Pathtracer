cmake_minimum_required(VERSION 3.6)

project(Pathtracer CXX)
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)
include(cmake/StaticAnalyzers.cmake)

# CMake variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INSTALL_MESSAGE LAZY)
set(ENGINE_CMAKE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Project Variables
set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Dependencies
add_subdirectory("libs/glad")
add_subdirectory("libs/sdl2")
add_subdirectory("libs/imgui")
add_subdirectory("libs/stb")
set(BUILD_STATIC_LIBS TRUE)
add_subdirectory("libs/glm")

# Generate constants file
configure_file(${SOURCE_DIR}/generated/Constants.cpp.in ${SOURCE_DIR}/generated/Constants.cpp)

# Get lazy and glob all sources
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.hpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Pathtracer.natvis")

# Glob all shader sources so that they also show in the IDEs
set(SHADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res/shaders")
file(GLOB_RECURSE SHADERS "${SHADERS_DIR}/*.frag" "${SHADERS_DIR}/*.vert")

# Create our executable
add_executable(${PROJECT_NAME} ${SOURCES} ${SHADERS} src/generated/Constants.cpp.in)
target_link_libraries(${PROJECT_NAME} glad SDL2::Core SDL2::Main imgui::imgui stb::stb glm::glm_static)
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})

# Warnings and project settings
set_project_warnings(${PROJECT_NAME})
enable_sanitizers(${PROJECT_NAME})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Copy resources to build dir
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCES_DIR} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res")

# Source group generation for IDEs
foreach(SOURCE IN ITEMS ${SOURCES})
  get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
  file(RELATIVE_PATH SOURCE_PATH_REL "${SOURCE_DIR}/.." "${SOURCE_PATH}")
  string(REPLACE "/" "\\" GROUP_PATH "${SOURCE_PATH_REL}")
  source_group("${GROUP_PATH}" FILES "${SOURCE}")
endforeach()
foreach(SOURCE IN ITEMS ${SHADERS})
  get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
  file(RELATIVE_PATH SOURCE_PATH_REL "${RESOURCES_DIR}/.." "${SOURCE_PATH}")
  string(REPLACE "/" "\\" GROUP_PATH "${SOURCE_PATH_REL}")
  source_group("${GROUP_PATH}" FILES "${SOURCE}")
endforeach()