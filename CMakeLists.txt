cmake_minimum_required(VERSION 3.3)
# Parameters
if (EXISTS ${CMAKE_SOURCE_DIR}/cmake/parameters.txt)
    include(${CMAKE_SOURCE_DIR}/cmake/parameters.txt)
endif()

# Project name
set(NAME homecenter_server)
set(EXECUTABLE ${NAME})

# Files
file(GLOB_RECURSE FILES_CPP "src/*.cpp")
file(GLOB_RECURSE FILES_H "src/*.h")
set(SOURCE_FILES main.cpp ${FILES_CPP} ${FILES_H})

# Project
project(${NAME})
add_executable(${EXECUTABLE} ${SOURCE_FILES})

# Add src directory to "include path"
target_include_directories(${EXECUTABLE} PUBLIC ./src)

# Compiler
set_property(TARGET ${EXECUTABLE} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${EXECUTABLE} PROPERTY CXX_STANDARD_REQUIRED ON)

## Dependencies
# Common
add_subdirectory(./vendors/common/)
target_link_libraries(${EXECUTABLE} common)
# Soci (installed manually)
#target_link_libraries(${EXECUTABLE} soci_core)
#target_link_libraries(${EXECUTABLE} soci_mysql)
# MySQL client (installed manually)
if (MYSQL_LIBRARY_PATH)
    target_include_directories(${EXECUTABLE} PUBLIC ${MYSQL_LIBRARY_PATH}/include/)
    target_link_libraries(${EXECUTABLE} ${MYSQL_LIBRARY_PATH}/lib/libmysqlclient.a)
endif()
