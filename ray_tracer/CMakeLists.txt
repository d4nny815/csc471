cmake_minimum_required(VERSION 3.4)

# Name of the project
project(raytracer)

# Enforce use of C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use glob to get the list of all source files
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "src/*.h")

# Set the executable
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})

# OS-specific options and libraries
message(STATUS "Adding GCC style compiler flags")
add_compile_options("-Wall" "-pedantic" "-Werror=return-type")

find_package(SDL2 REQUIRED)

if(SDL2_FOUND)
    message(STATUS "SDL2 found: ${SDL2_INCLUDE_DIRS}")
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL2::SDL2)
else()
    message(FATAL_ERROR "SDL2 not found!")
endif()
