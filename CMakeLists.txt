# Check if we are building a pico-sdk based project
# (or more exactly: if we just got included in a pico-sdk based project)
if (PICO_SDK_PATH)
    # If so, load the relevant CMakeLists-file but don't do anything else
    include(${CMAKE_CURRENT_LIST_DIR}/usePicoSDK.cmake)
    return()
endif()

## TODO: Non-PicoSDK builds
