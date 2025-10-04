#include "activate_obs_source.h"

#include "trigger.h"

namespace trigger {

void ActivateObsSource(void* const data) {
  static_cast<Trigger*>(data)->Activate();
}

}  // namespace trigger
