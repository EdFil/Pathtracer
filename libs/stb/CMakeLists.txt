project(stb)
cmake_minimum_required(VERSION 3.0)

message("")
message("==== STB ====")

# Make library
add_library(stb INTERFACE)
add_library(stb::stb ALIAS stb)
target_sources(stb INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/stb_image.h")

# Include directotories
target_include_directories(stb SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Show files
get_target_property(INTERFACE_SOURCES stb INTERFACE_SOURCES)

set(FILE_NAMES "")
foreach(FILE ${INTERFACE_SOURCES})
        get_filename_component(NAME ${FILE} NAME)
        string(APPEND FILE_NAMES "${NAME} ")
endforeach()

message(STATUS "${FILE_NAMES}")




