#include "distributerCube.h"


/**
 * @brief Construct a new distributer Cube object
 * 
 * This function constructs a new distributer Cube object. It takes a pointer to a Cube object,
 * a pointer to a pipeQueue object, and a vector of inputMesh objects containing the locations
 * of the pipes as parameters.
 * 
 * @param cube pointer to the Cube object
 * @param queue pointer to the pipeQueue object
 * @param pipeLocs vector of inputMesh objects containing the locations of the pipes
 * 
 */
distributerCube::distributerCube(Cube *cube, pipeQueue *queue, const std::vector<inputMesh>& pipeLocs) {

    for (const auto& l : pipeLocs) {
        PipeNode* node = (PipeNode*) cube->threeDimPipe->getPipeNode(pipeMapper::nodeId(l.x, l.y, 0));
        if (node != nullptr) {
            cubePipes_.emplace_back(node->in_data_queue());
        }
    }

    if (queue != nullptr) {
        in_data_queue(queue);
    }
    current_pipe_ = 0;
}

/**
 * @brief Sends a data packet to the next pipe in the distribution cube.
 * 
 * This function sends the provided data packet to the next pipe in the distribution cube.
 * The current pipe is determined by the 'current_pipe_' variable, which is incremented
 * after each send operation. If the 'current_pipe_' reaches the end of the 'cubePipes_'
 * vector, it wraps around to the beginning.
 * 
 * @param data The data packet to be sent.
 */
void distributerCube::send(pipeData::dataPacket data) {
    auto& queue = cubePipes_.at(current_pipe_);
    queue->Push(data);
    current_pipe_ = (current_pipe_ + 1) % cubePipes_.size();
}
