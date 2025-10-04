#ifndef PLUGINS_TRIGGER_TRIGGER_H_
#define PLUGINS_TRIGGER_TRIGGER_H_

#include <thread>
#include <utility>

#include "obs-frontend-api.h"
#include "settings.h"
#include "third_party/abseil-cpp/absl/base/thread_annotations.h"
#include "third_party/abseil-cpp/absl/synchronization/mutex.h"

namespace trigger {

class Trigger final {
 public:
  explicit Trigger(Settings settings);

  Trigger(const Trigger&) = delete;
  Trigger& operator=(const Trigger&) = delete;

  ~Trigger();

  void UpdateSettings(Settings settings);
  void Activate();
  void Deactivate();

 private:
  Settings settings_ = {} ABSL_GUARDED_BY(mu_);
  bool activated_ = false ABSL_GUARDED_BY(mu_);
  absl::Mutex mu_;
  std::jthread thread_;
};

}  // namespace trigger

#endif  // PLUGINS_TRIGGER_TRIGGER_H_
