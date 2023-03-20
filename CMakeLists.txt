cmake_minimum_required(VERSION 2.8)
project(ramsey)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH out)
include_directories("include")

set(Boost_REALPATH ON)
find_package(Boost REQUIRED)
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
else()
    message(SEND_ERROR "Boost not found!")
endif()

# List of executables
add_executable( rabi src/rabi.cpp src/neutron.cpp )
target_link_libraries( rabi ${Boost_LIBRARIES})

add_executable( ramsey src/ramsey.cpp src/neutron.cpp )
target_link_libraries( ramsey ${Boost_LIBRARIES})

add_executable( blochSiegert src/blochSiegert.cpp src/neutron.cpp )
target_link_libraries( blochSiegert ${Boost_LIBRARIES})
