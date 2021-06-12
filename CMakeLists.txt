# Check if we are building a pico-sdk based project
# (or more exactly: if we just got included in a pico-sdk based project)
if (PICO_SDK_PATH)
    # If so, load the relevant CMakeLists-file but don't do anything else
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/usePicoSDK.cmake)
    return()
endif()

############################
# for non-RPi-Pico platforms
############################
cmake_minimum_required(VERSION 3.15)

# Set the project name to your project name
project(RF24Mesh C CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)
add_compile_options(-Ofast -Wall)

# detect CPU and add compiler flags accordingly
include(cmake/detectCPU.cmake)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
    if(ENABLE_BUILD_WITH_TIME_TRACE)
        add_compile_definitions(project_options INTERFACE -ftime-trace)
    endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# get library info from Arduino IDE's required library.properties file
include(cmake/GetLibInfo.cmake) # sets the variable LibTargetName

# setup CPack options
include(cmake/CPackInfo.cmake)

find_library(RF24 rf24 REQUIRED)
message(STATUS "using RF24 library: ${RF24}")

find_library(RF24Network rf24network REQUIRED)
message(STATUS "using RF24Network library: ${RF24Network}")

###########################
# create target for bulding the RF24Log lib
###########################
add_library(${LibTargetName} SHARED RF24Mesh.cpp)
target_include_directories(${LibTargetName} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${LibTargetName} INTERFACE
    project_options
    project_warnings
    SHARED ${RF24}
    SHARED ${RF24Network}
)

set_target_properties(
    ${LibTargetName}
    PROPERTIES
    SOVERSION ${${LibName}_VERSION_MAJOR}
    VERSION ${${LibName}_VERSION_STRING}
)

###########################
# target install rules for the RF24Log lib
###########################
install(TARGETS ${LibTargetName}
    DESTINATION lib
)

install(FILES
        RF24Mesh.h
        RF24Mesh_config.h
    DESTINATION include/RF24Mesh
)

# CMAKE_CROSSCOMPILING is only TRUE when CMAKE_TOOLCHAIN_FILE is specified via CLI
if(CMAKE_HOST_UNIX AND "${CMAKE_CROSSCOMPILING}" STREQUAL "FALSE")
    install(CODE "message(STATUS \"Updating ldconfig\")")
    install(CODE "execute_process(COMMAND ldconfig)")
endif()
