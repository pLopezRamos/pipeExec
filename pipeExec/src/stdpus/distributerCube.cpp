#include "distributerCube.h"

distributerCube::distributerCube(Cube *cube, pipeQueue *queue, const std::vector<inputMesh>pipeLocs) {

    unsigned int i = 0;
    inputMesh l;

    PipeNode *node;

    while (i < pipeLocs.size()) {
        l = pipeLocs.at(i);
        node = (PipeNode*)cube->threeDimPipe->getPipeNode(pipeMapper::nodeId(l.x,l.y,0));
        cubePipes_.push_back(node->in_data_queue());
        ++i;
    }

    in_data_queue(queue);
    current_pipe_ = 0;
}

void distributerCube::send(pipeData::dataPacket data) {

        auto queue = cubePipes_.at(current_pipe_);
        queue->Push(data);
        current_pipe_ = (++current_pipe_ % cubePipes_.size());
}