#include "pipeExec.h"
class Sadder : public ProcessingUnitInterface {
  public:
    Sadder();
    ~Sadder();
    bool Run(void *) override;
    void Init() override;
    void End() override;
    ProcessingUnitInterface *Clone() override;
};
