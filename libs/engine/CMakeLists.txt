cmake_minimum_required(VERSION 3.6)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/submodules/vcpkg/scripts/buildsystems/vcpkg.cmake")
project(Engine CXX)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompilerWarnings.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/PreventInSourceBuilds.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/StandardProjectSettings.cmake")

# Project Variables
set(WARNINGS_AS_ERRORS TRUE CACHE BOOL "Treat compiler warnings as errors" FORCE)
set(ENGINE_RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")


# Vcpkg Dependencies
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/vcpkg_installed/${VCPKG_TARGET_TRIPLET})
find_package(EASTL CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")
find_package(xxHash CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
set(VCPKG_INCLUDES ${STB_INCLUDE_DIRS})
set(VCPKG_LIBRARIES EASTL glad::glad glm::glm imgui::imgui SDL2::SDL2 SDL2::SDL2main xxHash::xxhash)

# Local Dependencies
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/fast_obj")

# Generate constants file
configure_file(${SOURCE_DIR}/generated/CMakeConstants.cpp.in ${SOURCE_DIR}/generated/CMakeConstants.cpp)

# Get lazy and glob all sources
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.hpp")

file(GLOB EASTL_NATVIS "${Z_VCPKG_ROOT_DIR}/buildtrees/eastl/src/*/doc/EASTL.natvis")
file(GLOB IMGUI_NATVIS "${Z_VCPKG_ROOT_DIR}/buildtrees/imgui/src/*/misc/debuggers/imgui.natvis")
file(GLOB GLM_NATVIS "${Z_VCPKG_ROOT_DIR}/buildtrees/glm/src/*/util/glm.natvis")
message(STATUS "EASTL_NATVIS ----> ${EASTL_NATVIS}")
message(STATUS "IMGUI_NATVIS ----> ${IMGUI_NATVIS}")
message(STATUS "GLM_NATVIS ----> ${GLM_NATVIS}")
list(APPEND SOURCES 
	"${CMAKE_CURRENT_SOURCE_DIR}/Pathtracer.natvis"
	${EASTL_NATVIS}
	${IMGUI_NATVIS}
	${GLM_NATVIS})

# Glob all shader sources so that they also show in the IDEs
set(SHADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res/shaders")
file(GLOB_RECURSE SHADERS "${SHADERS_DIR}/*.frag" "${SHADERS_DIR}/*.vert")

# Create our executable
add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC fast_obj ${VCPKG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR} ${VCPKG_INCLUDES})

# Project settings
set_project_warnings(${PROJECT_NAME})
assure_out_of_source_builds()
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Source group generation for IDEs
foreach (SOURCE IN ITEMS ${SOURCES})
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    file(RELATIVE_PATH SOURCE_PATH_REL "${SOURCE_DIR}/.." "${SOURCE_PATH}")
    string(REPLACE "/" "\\" GROUP_PATH "${SOURCE_PATH_REL}")
    source_group("${GROUP_PATH}" FILES "${SOURCE}")
endforeach ()
# foreach (SOURCE IN ITEMS ${SHADERS})
#     get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
#     file(RELATIVE_PATH SOURCE_PATH_REL "${ENGINE_RESOURCES_DIR}/.." "${SOURCE_PATH}")
#     string(REPLACE "/" "\\" GROUP_PATH "${SOURCE_PATH_REL}")
#     source_group("${GROUP_PATH}" FILES "${SOURCE}")
# endforeach ()

file(GLOB_RECURSE VCPKG_SOURCES "${VCPKG_INSTALLED_DIR}/*.cpp" "${VCPKG_INSTALLED_DIR}/*.hpp" "${VCPKG_INSTALLED_DIR}/*.h" "${VCPKG_INSTALLED_DIR}/*.c")
add_custom_target(Vcpkg SOURCES ${VCPKG_SOURCES})
foreach (SOURCE IN ITEMS ${VCPKG_SOURCES})
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    file(RELATIVE_PATH SOURCE_PATH_REL "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include" "${SOURCE_PATH}")
    string(REPLACE "/" "\\" GROUP_PATH "${SOURCE_PATH_REL}")
    source_group("${GROUP_PATH}" FILES "${SOURCE}")
endforeach ()

# Copy resources to build dir
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${ENGINE_RESOURCES_DIR} "${CMAKE_BINARY_DIR}/res")


file(GLOB_RECURSE TEST_SOURCES "${TEST_DIR}/*.cc")
add_executable(${PROJECT_NAME}_TEST ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}_TEST ${PROJECT_NAME} GTest::gtest_main GTest::gtest)