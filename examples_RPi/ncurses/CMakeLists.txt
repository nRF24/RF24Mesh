# this example needs the ncurses package installed
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

set(example RF24Mesh_Ncurses_Master)

# make a target
add_executable(${example} ${example}.cpp)

# link the RF24 lib to the target. Notice we specify pthread as a linked lib here
if("${LibPIGPIO}" STREQUAL "LibPIGPIO-NOTFOUND")
    target_link_libraries(${example} PUBLIC
        ${RF24}
        pthread
        ${RF24Network}
        ${RF24Mesh}
        ${CURSES_LIBRARIES}
    )
else()
    target_link_libraries(${example} PUBLIC
        ${RF24}
        pthread
        ${RF24Network}
        ${RF24Mesh}
        ${CURSES_LIBRARIES}
        ${LibPIGPIO}
    )
endif()
