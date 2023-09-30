cmake_minimum_required(VERSION 3.10)
project(examples)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Set the source files
set(SOURCES ${CMAKE_SOURCE_DIR}/src/sleeper_main.cpp)

# Set the include directories
include_directories(
#    ${CMAKE_HOME_DIRECTORY}/include/pipeExec
#    ${CMAKE_HOME_DIRECTORY}/include/stdpus
    $ENV{HOME}/include/pipeExec
    $ENV{HOME}/include/stdpus
)

# Set the library directories
link_directories(
#    ${CMAKE_HOME_DIRECTORY}/lib
    $ENV{HOME}/lib
)

# Create the executable
add_executable(sleeper ${SOURCES})

# Link against the libraries (replace with your library names)
target_link_libraries(sleeper
    pipeExec
    stdpus
)

# Set the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)

