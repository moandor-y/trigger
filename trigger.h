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

  void UpdateSettings(Settings settings) {
    absl::MutexLock lock(&mu_);

    settings_ = std::move(settings);

    obs_source_t* const scene_source = obs_frontend_get_current_scene();
    obs_scene_t* const scene = obs_scene_from_source(scene_source);

    obs_sceneitem_t* const source_item =
        obs_scene_find_source_recursive(scene, settings_.source_name.c_str());

    obs_sceneitem_set_visible(source_item, false);

    obs_source_release(scene_source);
  }

 private:
  Settings settings_ = {} ABSL_GUARDED_BY(mu_);
  absl::Mutex mu_;
  std::jthread thread_;
};

}  // namespace trigger

#endif  // PLUGINS_TRIGGER_TRIGGER_H_
