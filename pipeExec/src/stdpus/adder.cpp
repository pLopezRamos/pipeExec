#include "adder.h"

void Adder::Init(void *data) {
  incValue_ = 1;

  if (data != nullptr)
  {
    int* val = static_cast<int*>(data);
    if (val != nullptr)
    {
      incValue_ = *val;
    }
  }
}

/**
 * @brief Adds 1 to the pointer in the data pointer
 * @param data The pointer to the data to manipulate
 */
// should be void * Adder::Run(void *data) ... return data;
void Adder::Run(void *data) {
  if (data != nullptr)
  {
    int *val = static_cast<int*>(getData(data));
    int *incVal = static_cast<int*>(getExtraData(data, getKey()));

    if (incVal == nullptr )
    {
      std::cout << "from init - increment = " << incValue_ << std::endl;
      *val += incValue_;
    }
    else
    {
      //          std::cout << "from data - sleeping " << *time << "s" << std::endl;
      *val += *incVal;
    }
  }
  else
  {
    throw std::runtime_error("Has to be called from pipeExec.");
  }
}

