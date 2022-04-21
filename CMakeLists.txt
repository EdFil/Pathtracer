cmake_minimum_required(VERSION 3.6)
project(Pathtracer CXX)

# Download automatically, you can also just copy the conan.cmake file
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
   message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
   file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake" "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)

# CMake variables
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INSTALL_MESSAGE LAZY)
set(ENGINE_CMAKE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(FORCE_REBUILD "Should rebuild pre-built libs" OFF)

# Project Variables
set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(CONAN_IMPORT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/conan/imports")

# Compiler Flags
if (MSVC)
    set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /EHsc /W3 /experimental:external /external:anglebrackets /external:W0")
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Ob0 /Od /RTC1 /DDEBUG /DSDL_ASSERT_LEVEL=2")
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /Ox /Ob2 /DRELEASE /DSDL_ASSERT_LEVEL=0")
else ()
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -fno-exceptions")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG -DSDL_ASSERT_LEVEL=2")
    set(CMAKE_CXX_FLAGS_RELEASE "-DRELEASE -DSDL_ASSERT_LEVEL=0")
endif ()

# Conan dependencies
conan_cmake_run(REQUIRES 
    glad/0.1.34
    sdl/2.0.20
    imgui/1.87
    stb/20200203
    eastl/3.18.00
    glm/0.9.9.8
    
    IMPORTS "./res/bindings, imgui_impl_opengl3_loader.h -> ${CONAN_IMPORT_DIR}"
    IMPORTS "./res/bindings, imgui_impl_opengl3.h -> ${CONAN_IMPORT_DIR}"
    IMPORTS "./res/bindings, imgui_impl_opengl3.cpp -> ${CONAN_IMPORT_DIR}"
    IMPORTS "./res/bindings, imgui_impl_sdl.h -> ${CONAN_IMPORT_DIR}"
    IMPORTS "./res/bindings, imgui_impl_sdl.cpp -> ${CONAN_IMPORT_DIR}"
    
    BASIC_SETUP CMAKE_TARGETS
    BUILD missing
)

# Dependencies not in Conan
add_subdirectory("libs/fast_obj")

# Generate constants file
configure_file(${SOURCE_DIR}/generated/CMakeConstants.cpp.in ${SOURCE_DIR}/generated/CMakeConstants.cpp)

# Get lazy and glob all sources
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp" "${SOURCE_DIR}/*.hpp" "${CONAN_IMPORT_DIR}/*.h" "${CONAN_IMPORT_DIR}/*.cpp")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Pathtracer.natvis")

# Glob all shader sources so that they also show in the IDEs
set(SHADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/res/shaders")
file(GLOB_RECURSE SHADERS "${SHADERS_DIR}/*.frag" "${SHADERS_DIR}/*.vert")

# Create our executable
add_executable(${PROJECT_NAME} ${SOURCES} ${SHADERS})
target_link_libraries(${PROJECT_NAME} fast_obj ${CONAN_TARGETS})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR} ${CONAN_IMPORT_DIR})

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

# Transform all Conan dependencies into custom targets so the public includes show up in ty
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
foreach(TARGET IN ITEMS ${CONAN_TARGETS})
    string(REPLACE "CONAN_PKG::" "" TARGET_NAME ${TARGET})
    string(TOUPPER ${TARGET_NAME} TARGET_NAME_UPPER)
    
    list (GET CONAN_INCLUDE_DIRS_${TARGET_NAME_UPPER} 0 HEAD)
    set(CONAN_VAR "CONAN_INCLUDE_DIRS_${TARGET_NAME_UPPER}")
    file(GLOB_RECURSE SOURCES "${HEAD}/*.cpp" "${HEAD}/*.hpp" "${HEAD}/*.h" "${HEAD}/*.c")
    add_custom_target(${TARGET_NAME} SOURCES ${SOURCES})
    set_target_properties(${TARGET_NAME} PROPERTIES FOLDER ConanDependencies)

    # Pretify folder structure inside for the target
    foreach (SOURCE IN ITEMS ${SOURCES})
        get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
        file(RELATIVE_PATH SOURCE_PATH_REL "${CONAN_${TARGET_NAME_UPPER}_ROOT}" "${SOURCE_PATH}")
        string(REPLACE "/" "\\" GROUP_PATH "${SOURCE_PATH_REL}")
        source_group("${GROUP_PATH}" FILES "${SOURCE}")
    endforeach ()
endforeach()

# Copy resources to build dir
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${RESOURCES_DIR} "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res")