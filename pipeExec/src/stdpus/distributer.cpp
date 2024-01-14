#include "distributer.h"

distributer::distributer(Mesh *mesh, pipeQueue *queue, const std::vector<unsigned int>pipeLocs) {

    unsigned int i = 0;
    unsigned int x;

    PipeNode *node;

    while (i < pipeLocs.size()) {
        x = pipeLocs.at(i);
        node = (PipeNode*)mesh->twoDimPipe->getPipeNode(pipeMapper::nodeId(x,0,0));
        meshPipes_.push_back(node->in_data_queue());
        ++i;
    }

    in_data_queue(queue);
    current_pipe_ = 0;
}

void distributer::send(pipeData::dataPacket data) {

        auto queue = meshPipes_.at(current_pipe_);
        queue->Push(data);
        current_pipe_ = (++current_pipe_ % meshPipes_.size());
}