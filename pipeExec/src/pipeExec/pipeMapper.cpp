#include "pipeMapper.h"
#include <iostream>
#include <stdexcept>

pipeMapper::pipeMapper(uint dimensions, uint xRange, uint yRange)
{
    if (dimensions > 3)
    {
        throw std::out_of_range("Dimension has to be 1, 2, or 3. " + std::to_string(dimensions) + " given.");
    }

    dim_ = dimensions;

    if (dim_ > 1 && (xRange == 0 || yRange == 0))
    {

        throw std::out_of_range("For dimension > 1 - ranges have to be given.");
    }
    xRange_ = xRange;
    yRange_ = yRange;
    x_ = y_ = z_ = 0;
}

pipeMapper::nodeId pipeMapper::addNode(void *node, std::string nodeName)
{

    uint x, y, z;
    auto id = pipeMapper::nodeId(0, 0, 0);

//    auto string_id = "[" + std::to_string(id.x) + ":" + std::to_string(id.y) + ":" +
//                     std::to_string(id.z) + "]";
//    std::cout << __func__ << " : " << __LINE__ << " address = " << string_id << std::endl;

    // Build a node address until a free address is found.
    // for 1 dimension only increment x and leave y = z = 0
    // for 2 dimensions increment x, y and if it reaches the range,
    // start incement the z
    while (pipeMapper::nodeExists(id))
    {
        ++x_;
        if (dim_ > 1)
        {
            if (x_ == xRange_)
            {
                x_ = 0;
                ++y_;
            }
        }
        if (dim_ > 2)
        {
            if (y_ == yRange_)
            {
                y_ = 0;
                ++z_;
            }
        }

        id.x = x_;
        id.y = y_;
        id.z = z_;
//    auto string_id = "[" + std::to_string(id.x) + ":" + std::to_string(id.y) + ":" +
//                     std::to_string(id.z) + "]";
//    std::cout << __func__ << " : " << __LINE__ << " address = " << string_id << std::endl;
    }


    if (nodeName == "")
    {
        nodeName = "[" + std::to_string(id.x) + ":" + std::to_string(id.y) + ":" + std::to_string(id.z) + "]";
    }

//    std::cout << __func__ << " : " << __LINE__ << " address = " << nodeName << std::endl;
    return pipeMapper::addNode(node, nodeName, id);
}

pipeMapper::nodeId pipeMapper::addNode(void *node, std::string nodeName, pipeMapper::nodeId id)
{
    auto string_id = "[" + std::to_string(id.x) + ":" + std::to_string(id.y) + ":" +
                     std::to_string(id.z) + "]";

//    std::cout << __func__ << " : " << __LINE__ << "address = " << string_id << std::endl;
    // Check if the key exists
    if (pipeMapper::nodeExists(id))
    {
        throw std::invalid_argument(string_id + " - is in use.");
    }

    if ((id.x >= xRange_ && xRange_ != 0) || (id.y >= yRange_ && yRange_ != 0))
    {
        throw std::invalid_argument(string_id + " - is  aout of range [" +
                                    std::to_string(xRange_) + ", " + std::to_string(yRange_) + "].");
    }

    if (nodeName == "")
    {
        nodeName = string_id;
    }

    nodes_[id] = node;
    ids_[nodeName].push_back(id);

    return id;
}

void *pipeMapper::getPipeNode(pipeMapper::nodeId id) const
{
    if (!pipeMapper::nodeExists(id))
    {
        auto string_id = std::to_string(id.x) + ":" + std::to_string(id.y) + ":" +
                         std::to_string(id.z);
        throw std::out_of_range("[" + string_id + "] - does not exists.");
    }
    return nodes_.at(id);
}

bool pipeMapper::nodeExists(pipeMapper::nodeId id) const
{
/*    for (const auto &pair : nodes_)
    {
        const nodeId &key = pair.first;
        std::cout << "Key: (" << key.x << ", " << key.y << ", " << key.z << ")\n";
    }*/


    return (nodes_.find(id) != nodes_.end() ) ? true : false;
}

bool pipeMapper::nodeExists(std::string id) const
{
    return (ids_.find(id) != ids_.end()) ? true : false;
}

void pipeMapper::sendToNode(pipeMapper::nodeId id, pipeData *data)
{

    auto node = pipeMapper::getPipeNode(id);
//    node->in_data_queue()->Push(data);

    return;
}
