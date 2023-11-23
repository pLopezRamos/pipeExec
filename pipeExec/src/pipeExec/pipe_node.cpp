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
 * @brief Signals the end of the node's work
 *
 * This method signals the end of the node's work, and ensures that all
 * threads have finished execution before returning.
 */
void PipeNode::EndNodeWork() {
  for (auto& thread : running_threads_) {
    thread->join();
  }
}

/**
 * @brief Gets the input memory manager of the current node
 *
 * @return A pointer to the input memory manager of the current node
 */
pipeQueue *PipeNode::in_data_queue() const { return in_data_queue_; }

/**
 * @brief Gets the output memory manager of the current node
 *
 * @return A pointer to the output memory manager of the current node
 */
pipeQueue *PipeNode::out_data_queue() const { return out_data_queue_; }

/**
 * @brief Gets whether the current node is the last node in the pipeline
 *
 * @return True if the current node is the last node in the pipeline,
 *         false otherwise
 */
bool PipeNode::last_node() const { return is_last_node_; }

/**
 * @brief Gets the processing unit of the current node
 *
 * @return A pointer to the processing unit of the current node
 */
ProcessingUnitInterface *PipeNode::processing_unit() const { return processing_unit_; }

/**
 * @brief Gets the number of instances of the current node
 *
 * @return The number of instances of the current node
 */
int PipeNode::number_of_instances() const { return number_of_instances_; }

/**
 * @brief Get the maximum number of proccessing units  instances allowed.
 *
 * @return The maximum number of instances.
 */
int PipeNode::max_instances() const { return max_instances_; }

/**
 * @brief Get the minimun number of proccessing units instances allowed.
 *
 * @return The minimun number of instances.
 */
int PipeNode::min_instances() const { return min_instances_; }

/**
 * @brief Get the next command in the command queue
 * 
 * @return THe next command or EMPTY
*/
PipeNode::nodeCmd PipeNode::getCmd() {
  if ( cmd_.empty() )
    return PipeNode::nodeCmd::EMPTY;
  else {
    auto cmd = cmd_.back();
    cmd_.pop_back();
    return cmd; }
}

PipeNode* PipeNode::getPrev() const { return prev_; };
PipeNode* PipeNode::getNext() const { return next_; };

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
std::vector<std::thread *> &PipeNode::running_threads() { return running_threads_; }

/**
 * @brief Get the array of extra args
 *
 * @return A pointer to the array of extra args
 */
void *PipeNode::extra_args() { return extra_args_; }

/**
 * @brief Sets the input memory manager of the current node
 *
 * @param in_data_queue A pointer to the input queue of the current node
 */
void PipeNode::in_data_queue(pipeQueue *data_in) { in_data_queue_ = data_in; }

/**
 * @brief Sets the output memory manager of the current node
 *
 * @param out_data_queue A pointer to the output queue of the current node
 */
void PipeNode::out_data_queue(pipeQueue *data_out) { out_data_queue_ = data_out; }

/**
 * @brief Sets the boolean indicating if the node is the last in the
 * pipeline
 *
 * @param bool - a boolean indicating if the node is the last in the
 * pipeline
 */
void PipeNode::last_node(bool is_last_node) { is_last_node_ = is_last_node; }

/**
 * @brief Sets the processing unit
 *
 * @param ProcessingUnitInterface * - a pointer to the processing unit
 */
void PipeNode::processing_unit(ProcessingUnitInterface *processing_unit) { processing_unit_ = processing_unit; }

/**
 * @brief Set the number of processing unit instances.
 *
 * @param instances_number The number of instances to be set.
 */
void PipeNode::number_of_instances(int instances_number) { number_of_instances_ = instances_number; }

/**
 * @brief Set the maximun number of processing unit instances.
 *
 * @param max_instances maximun number of instances to be set.
 */
void PipeNode::max_instances(int max_instances) { max_instances_ = max_instances; }

/**
 * @brief Set the minumun number of processing unit instances.
 *
 * @param min_instances The minumun number of instances to be set.
 */
void PipeNode::min_instances(int min_instances) { min_instances_ = min_instances; }

/**
 * @brief Add a command to the command queue
 *
 * @param cmd The command to be added 
 */
void PipeNode::setCmd(PipeNode::nodeCmd cmd) { cmd_.push_back(cmd); }

/**
 * @brief Set a pointer to the previous node.
 *
 * @param prev A pointer to the previousnode.
 */
void PipeNode::setPrev(PipeNode* prev) { prev_ = prev; }

/**
 * @brief Set a pointer to the previous node.
 *
 * @param prev A pointer to the previousnode.
 */
void PipeNode::setNext(PipeNode* next) { next_ = next; }

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
void PipeNode::PushThread(std::thread *thread) { running_threads_.push_back(thread); }

/**
 * @brief Sets the extra args for the current node
 *
 * @param args The pointer to the array of args
 */
void PipeNode::extra_args(void *args) { extra_args_ = args; }
