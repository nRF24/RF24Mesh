cmake_minimum_required(VERSION 3.12)

project(RF24LogExamples)
add_compile_options(-Ofast -Wall) # passing the compiler a `-pthread` flag doesn't work here

# detect the CPU make and type
include(../cmake/detectCPU.cmake) # sets the variable SOC accordingly

# detect if any additional libs need be linked to executable (ei RF24_DRIVER)
include(../cmake/AutoConfig_RF24_DRIVER.cmake)

find_library(RF24 rf24 REQUIRED)
message(STATUS "using RF24 library: ${RF24}")

find_library(RF24Network rf24network REQUIRED)
message(STATUS "using RF24Network library: ${RF24Network}")

find_library(RF24Mesh rf24mesh REQUIRED)
message(STATUS "using RF24Mesh library: ${RF24Mesh}")


# iterate over a list of examples by filename
set(EXAMPLES_LIST
    RF24Mesh_Example
    RF24Mesh_Example_Master
)

set(linked_libs 
    ${RF24}
    pthread # Notice we specify pthread as a linked lib here
    ${RF24Network}
    ${RF24Mesh}
)

# append additional libs for linking to the executable
if("${RF24_DRIVER}" STREQUAL "MRAA")
    if(NOT "${LibMRAA}" STREQUAL "LibMRAA-NOTFOUND")
        message(STATUS "linking to ${LibMRAA}")
        list(APPEND linked_libs ${LibMRAA})
    else()
        message(FATAL "Lib ${RF24_DRIVER} not found.")
    endif()
elseif("${RF24_DRIVER}" STREQUAL "wiringPi")
    if(NOT "${LibWiringPi}" STREQUAL "LibWiringPi-NOTFOUND")
        message(STATUS "linking to ${LibWiringPi}")
        list(APPEND linked_libs ${LibWiringPi})
    else()
        message(FATAL "Lib ${RF24_DRIVER} not found.")
    endif()
elseif("${RF24_DRIVER}" STREQUAL "pigpio")
    if(NOT "${LibPIGPIO}" STREQUAL "LibPIGPIO-NOTFOUND")
        message(STATUS "linking to ${LibPIGPIO}")
        list(APPEND linked_libs ${LibPIGPIO})
    else()
        message(FATAL "Lib ${RF24_DRIVER} not found.")
    endif()
endif()

foreach(example ${EXAMPLES_LIST})
    # make a target
    add_executable(${example} ${example}.cpp)
    # link the RF24 lib to the target.
    target_link_libraries(${example} PUBLIC ${linked_libs})
endforeach()

include(../cmake/enableNcursesExample.cmake)
if(BUILD_NCURSES_EXAMPLE)
    add_subdirectory(ncurses)
endif()
