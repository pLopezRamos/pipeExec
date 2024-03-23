#include "pipe_node.h"
#include "pipeQueue.h"
#include "cube.h"
#include "pipeData.h"

#pragma once

class distributerCube {

public:

    struct inputMesh {
        unsigned int x;
        unsigned int y;
    } ;

    distributerCube(Cube *cube, pipeQueue *queue,  const std::vector<inputMesh>& pipesLocations);

    /**
     * @brief Getter function that returns the value of the in_queue_ member variable.
     * 
     * @return pipeQueue* Pointer to a pipeQueue object.
     */
    pipeQueue* in_data_queue() const {
        return in_queue_;
    }
    /**
    * @brief Setter function for the in_data_queue member variable.
    * 
    * @param queue Pointer to a pipeQueue object.
    */
    void in_data_queue(pipeQueue* queue) {
        in_queue_ = queue;
    }
    void send(pipeData::dataPacket data);

private:
    std::vector<pipeQueue*> cubePipes_;
    unsigned int current_pipe_;
    pipeQueue *in_queue_;
};