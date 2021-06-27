project(stb)
cmake_minimum_required(VERSION 3.0)

message("")
message("==== FAST OBJ ====")

# Make library
add_library(fast_obj INTERFACE)
target_sources(fast_obj INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/fast_obj.h")

# Include directories
target_include_directories(fast_obj SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
#target_compile_definitions(fast_obj INTERFACE -DFAST_OBJ_IMPLEMENTATION)

# Show files
get_target_property(INTERFACE_SOURCES fast_obj INTERFACE_SOURCES)

set(FILE_NAMES "")
foreach(FILE ${INTERFACE_SOURCES})
    get_filename_component(NAME ${FILE} NAME)
    string(APPEND FILE_NAMES "${NAME} ")
endforeach()

message(STATUS "${FILE_NAMES}")




