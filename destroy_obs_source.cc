#include "destroy_obs_source.h"

#include <thread>

namespace trigger {

void DestroyObsSource(void* const data) {
  std::thread([data]() { delete static_cast<Trigger*>(data); }).detach();
}

}  // namespace trigger
