cmake_minimum_required(VERSION 3.0.0)

project(sunstealer.iso-13818-1-file VERSION 0.1.0 LANGUAGES CXX DESCRIPTION "sunstealer.iso-13818-1-file")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
set(CMAKE_CXX_EXTENSIONS OFF)
#
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")
#
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
enable_testing()

# library MODULE | SHARED | STATIC 
add_library(${PROJECT_NAME} SHARED main.cpp)
set_target_properties(sunstealer.iso-13818-1-file PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(sunstealer.iso-13818-1-file "${CMAKE_SOURCE_DIR}/../sunstealer.iso-13818-1/build/libsunstealer.iso-13818-1.a")

# -fpic => export class
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

#
#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)

INSTALL (
	TARGETS ${PROJECT_NAME}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)