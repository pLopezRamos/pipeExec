cmake_minimum_required(VERSION 3.12)
project(pipeExec)

set(CMAKE_INSTALL_PREFIX "$ENV{HOME}")

include(CMakePackageConfigHelpers)

add_subdirectory(src/pipeExec)
add_subdirectory(src/stdpus)

install(EXPORT pipeExecTargets
	FILE pipeExecTargets.cmake
	DESTINATION lib/cmake/pipeExec
)

install(EXPORT stdpusTargets
	FILE stdpusTargets.cmake
	DESTINATION lib/cmake/stdpus
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/libraryConfig.cmake
    INSTALL_DESTINATION lib/cmake/library
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/libraryConfigVersion.cmake
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/libraryConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/libraryConfigVersion.cmake
    DESTINATION lib/cmake/library
)
