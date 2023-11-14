#include "adder.h"

void Adder::Init(void *data) {
  pipeData *handler = (pipeData *)data;
  int* val = (int*)handler->data();

  incValue_ = *val;
}

/**
 * @brief Adds 1 to the pointer in the data pointer
 * @param data The pointer to the data to manipulate
 */
// should be void * Adder::Run(void *data) ... return data;
void Adder::Run(void *data) {
  pipeData *handler = (pipeData *)data;
  int* val = (int*)handler->data();

  *val += incValue_;
  //  *((int *)handler->data()) += 1;
}

