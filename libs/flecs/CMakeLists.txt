message("")
message("==== FLECS ====")

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/source)
set(BUILD_DIR ${CMAKE_BINARY_DIR}/flecs_build)
if(WIN32)
  set(INSTALL_DIR ${CMAKE_CURRENT_LIST_DIR}/.prebuilt/${CMAKE_BUILD_TYPE}/windows)
elseif (UNIX AND NOT APPLE)
  set(INSTALL_DIR ${CMAKE_CURRENT_LIST_DIR}/.prebuilt/${CMAKE_BUILD_TYPE}/linux)
else()
  message(FATAL_ERROR "NOT IMPLEMENTED")
endif()

# Try to find prebuilt package
set(CMAKE_PREFIX_PATH ${INSTALL_DIR})
find_package(flecs QUIET)

# If we don't have pre-built libs, generate new ones
if (NOT ${flecs_FOUND} OR FORCE_REBUILD)
  message(STATUS "Need to generate new prebuilt libs...")
  execute_process(COMMAND ${CMAKE_COMMAND} -B${BUILD_DIR} ${SOURCE_DIR} -DFLECS_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} OUTPUT_QUIET)
  message(STATUS "Building to ${BUILD_DIR}...")
  execute_process(COMMAND ${CMAKE_COMMAND} --build "${BUILD_DIR}" --config ${CMAKE_BUILD_TYPE} OUTPUT_QUIET)
  message(STATUS "Installing to  ${INSTALL_DIR}......")
  execute_process(COMMAND ${CMAKE_COMMAND} --install "${BUILD_DIR}" --config ${CMAKE_BUILD_TYPE} OUTPUT_QUIET)
  
  # Hack... since install command doesn't install all the header files :(
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/source/include" ${INSTALL_DIR}/include)
  find_package(flecs REQUIRED)
endif()

# Make static lib found by find_package available outside this scope
message(STATUS "Using: ${CMAKE_BUILD_TYPE} ${INSTALL_DIR}")

# Hack to make headers visible on IDEs
file(GLOB_RECURSE HEADERS "${INSTALL_DIR}/include/*.h")
add_custom_target(flecs SOURCES ${HEADERS})
set_target_properties(flecs::flecs_static PROPERTIES 
  IMPORTED_GLOBAL TRUE
  INTERFACE_LINK_LIBRARIES flecs
)
foreach (SOURCE IN ITEMS ${HEADERS})
    get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
    file(RELATIVE_PATH SOURCE_PATH_REL "${INSTALL_DIR}/include/.." "${SOURCE_PATH}")
    string(REPLACE "/" "\\" GROUP_PATH "${SOURCE_PATH_REL}")
    source_group("${GROUP_PATH}" FILES "${SOURCE}")
endforeach ()