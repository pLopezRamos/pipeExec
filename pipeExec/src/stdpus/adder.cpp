#include "adder.h"
/**
 * @brief Default constructor for Adder
 */
Adder::Adder() {}

/**
 * @brief Destructor of the Adder class
 */
Adder::~Adder() {}

/**
 * @brief Adds 1 to the pointer in the data pointer
 * @param data The pointer to the data to manipulate
 */
void Adder::Run(void *data) {
  pipeData *handler = (pipeData *)data;
  *((int *)handler->data()) += 1;
}

/**
 * @brief Doesn't allocate nothing
 */
void Adder::Init(void *pre_process_args) {}

/**
 * @brief Returns a new pointer to another instance of Adder
 */
ProcessingUnitInterface *Adder::Clone() { return new Adder(); }
