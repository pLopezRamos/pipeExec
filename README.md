# pipeExec
This library is designed to oversee multithreaded data flow across processing units, operating as a micro-distributed system. In this framework, programmers strategically position processing units within a three-dimensional address space.

# Structure elements
The system is constructed by connecting nodes using queues, which function as both buffers and synchronization elements. The queue class enables concurrent access by multiple producers and consumers

The queue serves as the source of input for the processing nodes (instances of the pipeNode class), which handle responsibilities such as I/O, thread management, and execution