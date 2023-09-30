/**
 * @file pipe_node.cc
 *
 * @brief Definition of the PipeNode class methods
 *
 * @author Lucas Hernández Abreu and Pablo López Ramos
 * Contact: lopez.ramos.pablo@gmail.com
 */
#include "pipe_node.h"

#include <cstdio>

/**
 * @brief Default constructor for PipeNode
 */
PipeNode::PipeNode() {}

/**
 * @brief Destructor for PipeNode
 * @details Joins all the running threads
 */
PipeNode::~PipeNode() {
  for (int it = 0; it < running_threads_.size(); ++it) {
    running_threads_[it]->join();
  }
}

/**
 * @brief Signals the end of the node's work
 *
 * This method signals the end of the node's work, and ensures that all
 * threads have finished execution before returning.
 */
void PipeNode::EndNodeWork() {
  for (int it = 0; it < running_threads_.size(); ++it) {
    running_threads_[it]->join();
  }
}

/**
 * @brief Gets the input memory manager of the current node
 *
 * @return A pointer to the input memory manager of the current node
 */
MemoryManager *PipeNode::in_data_queue() { return in_data_queue_; }

/**
 * @brief Gets the output memory manager of the current node
 *
 * @return A pointer to the output memory manager of the current node
 */
MemoryManager *PipeNode::out_data_queue() { return out_data_queue_; }

/**
 * @brief Gets whether the current node is the last node in the pipeline
 *
 * @return True if the current node is the last node in the pipeline,
 *         false otherwise
 */
bool PipeNode::last_node() { return is_last_node_; }

/**
 * @brief Gets the processing unit of the current node
 *
 * @return A pointer to the processing unit of the current node
 */
ProcessingUnitInterface *PipeNode::processing_unit() {
  return processing_unit_;
}

/**
 * @brief Gets the number of instances of the current node
 *
 * @return The number of instances of the current node
 */
int PipeNode::number_of_instances() { return number_of_instances_; }

/**
 * @brief Gets the ID of the current node
 *
 * @return The ID of the current node
 */
int PipeNode::node_id() { return node_id_; }

/**
 * @brief Gets a vector of running threads for the current node
 *
 * @return A vector of running threads for the current node
 */
std::vector<std::thread *> &PipeNode::running_threads() {
  return running_threads_;
}

/**
 * @brief Get the array of extra args
 *
 * @return A pointer to the array of extra args
 */
void **PipeNode::extra_args() { return extra_args_; }

/**
 * @brief Sets the input memory manager of the current node
 *
 * @param in_data_queue A pointer to the input memory manager of the current
 * node
 */
void PipeNode::in_data_queue(MemoryManager *data_in) {
  in_data_queue_ = data_in;
}

/**
 * @brief Sets the output memory manager of the current node
 *
 * @param in_data_queue A pointer to the output memory manager of the
 * current node
 */
void PipeNode::out_data_queue(MemoryManager *data_out) {
  out_data_queue_ = data_out;
}

/**
 * @brief Sets the boolean indicating if the node is the last in the
 * pipeline
 *
 * @param bool - a boolean indicating if the node is the last in the
 * pipeline
 */
void PipeNode::last_node(bool is_last_node) { is_last_node_ = is_last_node; }

/**
 * @brief Sets the processing unit of the node
 *
 * @param ProcessingUnitInterface * - a pointer to the processing
 * unit of the node
 */
void PipeNode::processing_unit(ProcessingUnitInterface *processing_unit) {
  processing_unit_ = processing_unit;
}

/**
 * @brief Sets the number of instances of the current node
 *
 * @param number_of_instances The number of instances of the current node
 */
void PipeNode::number_of_instances(int instances_number) {
  number_of_instances_ = instances_number;
}

/**
 * @brief Sets the ID of the current node
 *
 * @param node_id The ID of the current node
 */
void PipeNode::node_id(int new_id) { node_id_ = new_id; }

/**
 * @brief Pushes the thread to the list of running threads
 *
 * @param thread The thread to push to the list of running threads
 */
void PipeNode::PushThread(std::thread *thread) {
  running_threads_.push_back(thread);
}

/**
 * @brief Sets the extra args for the current node
 *
 * @param args The pointer to the array of args
 */
void PipeNode::extra_args(void **args) { extra_args_ = args; }

/* vim:set softtabstop=2 shiftwidth=2 tabstop=2 expandtab: */
