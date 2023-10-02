add_library(stdpus
	adder.cpp
	indexer.cpp
	null_unit.cpp
	number_printer.cpp
	simple_indexer.cpp
	sleeper.cpp
)

# Set the include directories
include_directories(
    ${CMAKE_HOME_DIRECTORY}/src/pipeExec
)

target_include_directories(stdpus
    PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

install(TARGETS stdpus
	EXPORT stdpusTargets
	ARCHIVE DESTINATION lib
)

install(FILES
	adder.h
	double_pipe.h
	indexer.h
	null_unit.h
	number_printer.h
	simple_indexer.h
	sleeper.h

	DESTINATION include/stdpus
)
