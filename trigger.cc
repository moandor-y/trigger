#include "trigger.h"

#include <thread>

#include "third_party/abseil-cpp/absl/time/clock.h"

namespace trigger {

Trigger::Trigger(Settings settings) : settings_(std::move(settings)) {
  thread_ = std::jthread([](std::stop_token stop_token) {
    while (!stop_token.stop_requested()) {
      absl::SleepFor(absl::Milliseconds(100));
    }
  });
}

Trigger::~Trigger() { thread_.request_stop(); }

}  // namespace trigger
