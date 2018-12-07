cmake_minimum_required(VERSION 2.8)
project(ramsey)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH out)

find_package(ROOT REQUIRED)
if (ROOT_FOUND)
    include_directories(${ROOT_INCLUDE_DIRS})
    link_directories(${ROOT_LIBRARY_DIRS})
else()
    message(SEND_ERROR "Root not found!")
endif()

include_directories("include")

add_executable( circRabi src/circRabi.cpp src/neutron.cpp )
target_link_libraries( circRabi ${ROOT_LIBRARIES})
