#include "pipe_node.h"
#include "pipeQueue.h"
#include "mesh.h"
#include "pipeData.h"

#pragma once

class distributer {

public:

    distributer(Mesh *mesh, pipeQueue *queue,  const std::vector<unsigned int>pipesLocations);

    pipeQueue *in_data_queue() const { return in_queue_; };
    void in_data_queue(pipeQueue *queue) { in_queue_ = queue; };
    void send(pipeData::dataPacket data);

private:
    std::vector<pipeQueue*> meshPipes_;
    unsigned int current_pipe_;
    pipeQueue *in_queue_;
};