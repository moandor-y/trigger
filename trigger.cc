#include "trigger.h"

#include <string>
#include <thread>

#include "Windows.h"
#include "third_party/abseil-cpp/absl/log/log.h"
#include "third_party/abseil-cpp/absl/status/statusor.h"
#include "third_party/abseil-cpp/absl/strings/str_cat.h"
#include "third_party/abseil-cpp/absl/strings/str_format.h"
#include "third_party/abseil-cpp/absl/synchronization/mutex.h"
#include "third_party/abseil-cpp/absl/time/clock.h"

namespace trigger {

static absl::StatusOr<std::string> GetCurrentWindowTitleUtf8() {
  const HWND foreground = GetForegroundWindow();
  if (foreground == nullptr) {
    return absl::InternalError("failed to get foreground");
  }

  const int length = GetWindowTextLengthW(foreground);
  if (length == 0) {
    const DWORD error = GetLastError();
    if (error != 0) {
      return absl::InternalError(
          absl::StrCat("failed to get window title length, error=", error));
    }

    return "";
  }

  std::wstring title(length + 1, L'\0');
  const int length_written =
      GetWindowTextW(foreground, title.data(), title.size());
  if (length_written == 0) {
    return absl::InternalError(
        absl::StrCat("failed to get window title, error=", GetLastError()));
  }
  title.resize(length_written);

  return absl::StrFormat("%s", title);
}

static void SetObsSourceVisible(const std::string& source_name,
                                const bool visible) {
  obs_source_t* const scene_source = obs_frontend_get_current_scene();
  obs_scene_t* const scene = obs_scene_from_source(scene_source);

  obs_sceneitem_t* const source_item =
      obs_scene_find_source_recursive(scene, source_name.c_str());

  obs_sceneitem_set_visible(source_item, visible);

  obs_source_release(scene_source);
}

Trigger::Trigger(Settings settings) : settings_(std::move(settings)) {
  thread_ = std::jthread([this](std::stop_token stop_token) {
    while (!stop_token.stop_requested()) {
      {
        const absl::StatusOr<std::string> current_window_title =
            GetCurrentWindowTitleUtf8();
        if (!current_window_title.ok()) {
          LOG(ERROR) << current_window_title.status();
          continue;
        }

        absl::MutexLock lock(&mu_);
        SetObsSourceVisible(settings_.source_name,
                            settings_.window_title == *current_window_title);
      }

      absl::SleepFor(absl::Milliseconds(100));
    }
  });
}

Trigger::~Trigger() {
  thread_.request_stop();
  thread_.join();
}

void Trigger::UpdateSettings(Settings settings) {
  absl::MutexLock lock(&mu_);

  settings_ = std::move(settings);
}

}  // namespace trigger
