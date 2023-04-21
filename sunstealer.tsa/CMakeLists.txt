cmake_minimum_required(VERSION 3.0.0)

project(sunstealer VERSION 0.1.0 LANGUAGES CXX DESCRIPTION "sunstealer")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
set(CMAKE_CXX_EXTENSIONS OFF)
#
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")
#
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
include(CTest)
enable_testing()

#
add_executable(sunstealer ./sunstealer.cpp/main.cpp)

target_link_libraries(sunstealer "${CMAKE_SOURCE_DIR}/sunstealer.iso-13818-1/build/libsunstealer.iso-13818-1.a")

# static link shared library
# target_include_directories(sunstealer PUBLIC "/sunstealer.logging.so")
# target_link_directories(sunstealer PUBLIC "./sunstealer.logging.so/build")
# target_link_libraries(sunstealer sunstealer.logging)

# ajm: boost
# set(Boost_INCLUDE_DIR "/home/ajm/Documents/boost_1_81_0")
# set(Boost_LIBRARY_DIR "/home/ajm/Documents/boost_1_81_0/stage/lib")
# find_package(Boost 1.81 COMPONENTS thread REQUIRED)
# include_directories(${Boost_INCLUDE_DIR})
# link_directories(${Boost_LIBRARY_DIRS})
# set(Boost_USE_STATIC_LIBS OFF)
# set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)
# message(STATUS "Boost_VERSION: ${Boost_VERSION}")
# message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
# message(STATUS "Boost_INCLUDE_DIR: ${Boost_INCLUDE_DIR}")
# message(STATUS "Boost_LIBRARY_DIR: ${Boost_LIBRARY_DIR}")
# target_link_libraries(sunstealer PUBLIC ${Boost_LIBRARIES}) 

# ajm: 
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
