include("../../cmake/Utils.cmake")

message("")
message("==== SDL2 ====")

if(WIN32)
  set(SDL2_DIR ${CMAKE_CURRENT_LIST_DIR}/windows)
elseif (UNIX AND NOT APPLE)
  set(SDL2_DIR ${CMAKE_CURRENT_LIST_DIR}/linux)
else()
  message(FATAL_ERROR "NOT IMPLEMENTED")
endif()

set(SDL2_INCLUDE_DIR ${SDL2_DIR}/include)

# Find everything we need
engine_find_library(SDL2_LIB "SDL2" ${SDL2_DIR})
engine_find_library(SDL2MAIN_LIB "SDL2main" ${SDL2_DIR})
if(WIN32)
  engine_find_dll(SDL2_RUNTIME_LIB "SDL2.dll" ${SDL2_DIR})
  file(INSTALL ${SDL2_RUNTIME_LIB} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# Create SDL2:Core lib
add_library(SDL2::Core SHARED IMPORTED GLOBAL ${INCLUDE_FILES})
set_target_properties(SDL2::Core PROPERTIES 
    IMPORTED_LOCATION "${SDL2_LIB}" 
    INTERFACE_INCLUDE_DIRECTORIES "${SDL2_INCLUDE_DIR}"
)

# Create SDL2::Main lib
add_library(SDL2::Main STATIC IMPORTED GLOBAL)
set_target_properties(SDL2::Main PROPERTIES 
    IMPORTED_LOCATION "${SDL2MAIN_LIB}")


# Read SDL2 version
if(SDL2_INCLUDE_DIR AND EXISTS "${SDL2_INCLUDE_DIR}/SDL_version.h")
  file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL2_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL_MAJOR_VERSION[ \t]+[0-9]+$")
  file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL2_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL_MINOR_VERSION[ \t]+[0-9]+$")
  file(STRINGS "${SDL2_INCLUDE_DIR}/SDL_version.h" SDL2_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL_PATCHLEVEL[ \t]+[0-9]+$")
  string(REGEX REPLACE "^#define[ \t]+SDL_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_VERSION_MAJOR "${SDL2_VERSION_MAJOR_LINE}")
  string(REGEX REPLACE "^#define[ \t]+SDL_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_VERSION_MINOR "${SDL2_VERSION_MINOR_LINE}")
  string(REGEX REPLACE "^#define[ \t]+SDL_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL2_VERSION_PATCH "${SDL2_VERSION_PATCH_LINE}")
  set(SDL2_VERSION_STRING ${SDL2_VERSION_MAJOR}.${SDL2_VERSION_MINOR}.${SDL2_VERSION_PATCH})
  unset(SDL2_VERSION_MAJOR_LINE)
  unset(SDL2_VERSION_MINOR_LINE)
  unset(SDL2_VERSION_PATCH_LINE)
  unset(SDL2_VERSION_MAJOR)
  unset(SDL2_VERSION_MINOR)
  unset(SDL2_VERSION_PATCH)
endif()

# Print out relevant information
message(STATUS "Version: ${SDL2_VERSION_STRING}")
message(STATUS "Includes: ${SDL2_INCLUDE_DIR}")
message(STATUS "SDL2_LIB: ${SDL2_LIB}")
message(STATUS "SDL2MAIN_LIB: ${SDL2MAIN_LIB}")
message(STATUS "SDL2_RUNTIME_LIB: ${SDL2_RUNTIME_LIB}")