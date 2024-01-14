//#include "pipe_node.h"

#pragma once
#include "processing_unit_interface.h"
#include <map>


/// @brief  A map of nodes
///
/// @details A map of nodes that allows for direct I/O and proccessing
/// unit manipulation
///
class pipeMapper
{

public:
    /// @brief Constructor
    /// @param  unsigned int  Number of dimensions. Can be 1,2 or 3
    /// @param  unsigned int  The size in x. No limit if not given
    /// @param  unsigned int  The size in y. No limit if not given
    /// @throws std::out_of_range if dimension is not 1, 2, or 3
    pipeMapper(unsigned int  = 1, unsigned int  = 0, unsigned int  = 0);

    struct nodeId
    {
        uint x, y, z;
        nodeId() : x(0), y(0), z(0) {}  // Add a default constructor
        nodeId(uint x_, uint y_, uint z_) : x(x_), y(y_), z(z_) {}
        bool operator<(const nodeId &id) const
        {
            return x < id.x || (x == id.x && y < id.y) || (x == id.x && y == id.y && z < id.z);
        }
    };

    /// @brief Add a node to the map
    /// @details A node is added to the map. If no name is given, the string
    ///          representation of of the x,y,z coordinates of the nodeId is given.
    /// @param void* a pipe node, if no more parameters given,
    ///        the id will be the next available id
    /// @param string The node name
    /// @returns the node id
    /// @throws invalid_value if node id already exists
    nodeId addNode(void *, std::string = "");

    /// @brief Add a node to the map
    /// @details A node is added to the map. If no name is given, the string
    ///          representation of of the x,y,z coordinates of the nodeId is given.
    /// @param void* a pipe node, if no more parameters given,
    ///        the id will be the next available id
    /// @returns the x,y,z node id
    /// @throws bad_alloc if node id already exists
    nodeId addNode(void *, std::string , nodeId);

    /// @brief Return the node with a given id
    /// @param  nodeId the x,y,z coordinates of the node
    /// @return a pointer to a void
    /// @throws std::out_of_range if nodeId does not exist
    void* getPipeNode(nodeId) const;
    void* getPipeNode(std::string) const;

    /// @brief Checks if the given node id is already assigned
    /// @param  nodeId The node id
    /// @return True if already assigned, false otherwise
    bool nodeExists(nodeId) const;

    /// @brief Checks if the given node name is already assigned
    /// @param  std::string The node name
    /// @return True if already assigned, false otherwise
    bool nodeExists(std::string) const;

    /// @brief Send the data packet to the given node
    /// @param  nodeId the x,y,zs coordinates of the node
    /// @param  pipeData* the data packet
    /// @return true if the node exists, false otherwise
    /// @throws std::out_of_range if nodeId does not exist
    void sendToNode(nodeId, pipeData *);

    /// @brief return the list of node ids
    /// @return a vector containing all the ids
    std::vector<pipeMapper::nodeId> getNodeIds() const { return idList_; };

    unsigned int getXrange() const  { return xRange_; };
    unsigned int getYrange() const  { return yRange_; };
    unsigned int getZrange() const  { return zRange_; };

private:
    uint dim_, xRange_, yRange_, zRange_;
    uint x_, y_, z_;
    std::map<nodeId, void *> nodes_;
    std::vector<nodeId> idList_;
    std::map<std::string, std::vector<nodeId>> ids_;
};