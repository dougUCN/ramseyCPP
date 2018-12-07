cmake_minimum_required(VERSION 2.8)
project(ramsey)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH out)
include_directories("include")

find_package(ROOT REQUIRED)
if (ROOT_FOUND)
    include_directories(${ROOT_INCLUDE_DIRS})
    link_directories(${ROOT_LIBRARY_DIRS})
else()
    message(SEND_ERROR "Root not found!")
endif()

# List of executables
add_executable( rabi src/rabi.cpp src/neutron.cpp )
target_link_libraries( rabi ${ROOT_LIBRARIES})

add_executable( ramsey src/ramsey.cpp src/neutron.cpp )
target_link_libraries( ramsey ${ROOT_LIBRARIES})
