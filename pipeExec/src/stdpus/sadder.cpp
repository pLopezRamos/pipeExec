#include "sadder.h"
#include <unistd.h>
Sadder::Sadder() { this->unit_identifier_ = ProcessingUnitIdentifiers::kAdder; }
Sadder::~Sadder() {}

bool
Sadder::Run(void *data) {
    long *handler = (long *)data;
    *handler += 1;
//    usleep(100);
    sleep(1);
    return true;
}

void
Sadder::Init() {}

void
Sadder::End() {}

ProcessingUnitInterface *
Sadder::Clone() {
    return new Sadder();
}
