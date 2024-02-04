cmake_minimum_required(VERSION 3.10)
project(examples)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Set the source files
set(SOURCES ${CMAKE_SOURCE_DIR}/src/sleeper_main.cpp)
set(MESH_SOURCES ${CMAKE_SOURCE_DIR}/src/mesh_sleep.cpp)
set(CUBE_SOURCES ${CMAKE_SOURCE_DIR}/src/cube_sleep.cpp)

#set(SLEEPDS ${CMAKE_SOURCE_DIR}/src/sleeperD.cpp
#            ${CMAKE_SOURCE_DIR}/src/sleeper_data.cpp)

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
add_executable(sleeperMesh ${MESH_SOURCES})
add_executable(sleeperCube ${CUBE_SOURCES})
#add_executable(sleeperD ${SLEEPDS})

# Link against the libraries (replace with your library names)
target_link_libraries(sleeper
    pipeExec
    stdpus
    pthread
)

target_link_libraries(sleeperMesh
    pipeExec
    stdpus
    pthread
)

target_link_libraries(sleeperCube
    pipeExec
    stdpus
    pthread
)

# Link against the libraries (replace with your library names)
#target_link_libraries(sleeperD
#    pipeExec
#    stdpus
#    pthread
#)

# Set the output directory for the executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_HOME_DIRECTORY}/bin)

