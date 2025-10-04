#include "deactivate_obs_source.h"

#include "trigger.h"

namespace trigger {

void DeactivateObsSource(void* const data) {
  static_cast<Trigger*>(data)->Deactivate();
}

}  // namespace trigger
