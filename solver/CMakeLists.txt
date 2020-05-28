cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(StrategicSolver LANGUAGES C CXX)

# Collect all files that need to be compiled
file(GLOB target_files "*.h" "*.cpp" "*.cpp")

# add the executable
FIND_PACKAGE ( Threads REQUIRED )
add_executable(StrategicSolver ${target_files})

# include threads
TARGET_LINK_LIBRARIES ( StrategicSolver ${CMAKE_THREAD_LIBS_INIT} )

# include boost
target_include_directories(StrategicSolver PRIVATE ${PROJECT_SOURCE_DIR}/../boost_1_67)

# Configure output path
set_target_properties(StrategicSolver PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set_target_properties(StrategicSolver PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR})