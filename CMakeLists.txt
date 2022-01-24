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
include(${CMAKE_CURRENT_LIST_DIR}/cmake/StandardProjectSettings.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/PreventInSourceBuilds.cmake)

# get library info from Arduino IDE's required library.properties file
include(${CMAKE_CURRENT_LIST_DIR}/cmake/GetLibInfo.cmake) # sets the variable LibTargetName

# allow using CMake options to adjust RF24Network_config.h without modiying source code
option(MESH_NOMASTER "exclude compiling code that is strictly for master nodes" OFF)
option(MESH_DEBUG "enable/disable debugging output" OFF)
option(MESH_DEBUG_MINIMAL "enable/disable minimal debugging output" OFF)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(${LibTargetName}_project_options INTERFACE)
target_compile_features(${LibTargetName}_project_options INTERFACE cxx_std_17)
add_compile_options(-Ofast -Wall)

# detect CPU and add compiler flags accordingly
include(cmake/detectCPU.cmake)

# detect any applicable external libs (like pigpio)
include(cmake/AutoConfig_RF24_DRIVER.cmake)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
    if(ENABLE_BUILD_WITH_TIME_TRACE)
        add_compile_definitions(${LibTargetName}_project_options INTERFACE -ftime-trace)
    endif()
endif()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(${LibTargetName}_project_warnings INTERFACE)

# enable cache system
include(${CMAKE_CURRENT_LIST_DIR}/cmake/Cache.cmake)

# standard compiler warnings
include(${CMAKE_CURRENT_LIST_DIR}/cmake/CompilerWarnings.cmake)
set_project_warnings(${LibTargetName}_project_warnings)

# setup CPack options
include(${CMAKE_CURRENT_LIST_DIR}/cmake/CPackInfo.cmake)
if(NOT DEFINED USE_RF24_LIB_SRC)
    find_library(RF24 rf24 REQUIRED)
    message(STATUS "using RF24 library: ${RF24}")

    find_library(RF24Network rf24network REQUIRED)
    message(STATUS "using RF24Network library: ${RF24Network}")
endif()

###########################
# create target for bulding the RF24Mesh lib
###########################
add_library(${LibTargetName} SHARED RF24Mesh.cpp)
target_include_directories(${LibTargetName} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# python wrapper builds from source
if(DEFINED USE_RF24_LIB_SRC OR pybind11_FOUND OR SKBUILD)
    message(STATUS "Building lib from RF24 source")
    target_compile_definitions(${LibTargetName} PUBLIC USE_RF24_LIB_SRC)

    if(NOT DEFINED RF24_LIB_PATH)
        target_include_directories(${LibTargetName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../RF24)
    else()
        target_include_directories(${LibTargetName} PUBLIC ${RF24_LIB_PATH})
    endif()

    if(NOT DEFINED RF24NETWORK_LIB_PATH)
        target_include_directories(${LibTargetName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../RF24)
    else()
        target_include_directories(${LibTargetName} PUBLIC ${RF24NETWORK_LIB_PATH})
    endif()

    target_link_libraries(${LibTargetName}
        INTERFACE ${LibTargetName}_project_options
        INTERFACE ${LibTargetName}_project_warnings
    )
else()

    target_link_libraries(${LibTargetName}
        INTERFACE ${LibTargetName}_project_options
        INTERFACE ${LibTargetName}_project_warnings
        SHARED ${RF24}
        SHARED ${RF24Network}
    )
endif()

set_target_properties(
    ${LibTargetName}
    PROPERTIES
    SOVERSION ${${LibName}_VERSION_MAJOR}
    VERSION ${${LibName}_VERSION_STRING}
)
# assert the appropriate preprocessor macros for RF24Network_config.h
if(MESH_NOMASTER)
    message(STATUS "MESH_NOMASTER asserted")
    target_compile_definitions(${LibTargetName} PUBLIC MESH_NOMASTER)
endif()
if(MESH_DEBUG)
    message(STATUS "MESH_DEBUG asserted")
    target_compile_definitions(${LibTargetName} PUBLIC MESH_DEBUG)
endif()
if(MESH_DEBUG_MINIMAL)
    message(STATUS "MESH_DEBUG_MINIMAL asserted")
    target_compile_definitions(${LibTargetName} PUBLIC MESH_DEBUG_MINIMAL)
endif()
# for the following, we let the default be configured in source code
if(DEFINED MESH_MAX_CHILDREN)
    message(STATUS "MESH_MAX_CHILDREN set to ${MESH_MAX_CHILDREN}")
    target_compile_definitions(${LibTargetName} PUBLIC MESH_MAX_CHILDREN=${MESH_MAX_CHILDREN})
endif()
if(DEFINED MESH_DEFAULT_CHANNEL)
    message(STATUS "MESH_DEFAULT_CHANNEL set to ${MESH_DEFAULT_CHANNEL}")
    target_compile_definitions(${LibTargetName} PUBLIC MESH_DEFAULT_CHANNEL=${MESH_DEFAULT_CHANNEL})
endif()
if(DEFINED MESH_RENEWAL_TIMEOUT)
    message(STATUS "MESH_RENEWAL_TIMEOUT set to ${MESH_RENEWAL_TIMEOUT}")
    target_compile_definitions(${LibTargetName} PUBLIC MESH_RENEWAL_TIMEOUT=${MESH_RENEWAL_TIMEOUT})
endif()
if(DEFINED MESH_MEM_ALLOC_SIZE)
    message(STATUS "MESH_MEM_ALLOC_SIZE set to ${MESH_MEM_ALLOC_SIZE}")
    target_compile_definitions(${LibTargetName} PUBLIC MESH_MEM_ALLOC_SIZE=${MESH_MEM_ALLOC_SIZE})
endif()
if(DEFINED MESH_LOOKUP_TIMEOUT)
    message(STATUS "MESH_LOOKUP_TIMEOUT set to ${MESH_LOOKUP_TIMEOUT}")
    target_compile_definitions(${LibTargetName} PUBLIC MESH_LOOKUP_TIMEOUT=${MESH_LOOKUP_TIMEOUT})
endif()
if(DEFINED MESH_WRITE_TIMEOUT)
    message(STATUS "MESH_WRITE_TIMEOUT set to ${MESH_WRITE_TIMEOUT}")
    target_compile_definitions(${LibTargetName} PUBLIC MESH_WRITE_TIMEOUT=${MESH_WRITE_TIMEOUT})
endif()
# conditionally disable interruot support (a pigpio specific feature)
if("${LibPIGPIO}" STREQUAL "LibPIGPIO-NOTFOUND" OR DEFINED RF24_NO_INTERRUPT)
    message(STATUS "Disabling IRQ pin support")
    target_compile_definitions(${LibTargetName} PUBLIC RF24_NO_INTERRUPT)
endif()


###########################
# target install rules for the RF24Mesh lib
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
