add_library(pipeExec
	pipeData.cpp
	memory_manager.cpp
	pipeline.cpp
	pipe_node.cpp
	semaphore.cpp
)

set(CMAKE_INSTALL_LIB_DIR $HOME/lib)
set(CMAKE_INSTALL_INCLUDE_DIR $HOME/include)

target_include_directories(pipeExec
    PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:include>
)

install(TARGETS pipeExec
	EXPORT pipeExecTargets
	ARCHIVE DESTINATION lib
)

install(FILES
	pipeExec.h
	pipeData.h
	memory_manager.h
	pipeline.h
  pipe_node.h
  profiling.h
	processing_unit_interface.h
  semaphore.h
	DESTINATION include/pipeExec
	)

