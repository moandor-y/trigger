#include "trigger.h"

#include <filesystem>
#include <string>
#include <thread>

// Windows.h must be included before Psapi.h due to a bug in Psapi.h
// clang-format off
#include "Windows.h"
#include "Psapi.h"
// clang-format on

#include "third_party/abseil-cpp/absl/log/log.h"
#include "third_party/abseil-cpp/absl/status/statusor.h"
#include "third_party/abseil-cpp/absl/strings/str_cat.h"
#include "third_party/abseil-cpp/absl/strings/str_format.h"
#include "third_party/abseil-cpp/absl/synchronization/mutex.h"
#include "third_party/abseil-cpp/absl/time/clock.h"
#include "third_party/gutil/gutil/status.h"

namespace trigger {

static absl::StatusOr<HWND> GetCurrentWindowHandle() {
  const HWND foreground = GetForegroundWindow();
  if (foreground == nullptr) {
    return absl::InternalError("failed to get foreground");
  }
  return foreground;
}

static absl::StatusOr<std::string> GetCurrentWindowTitleUtf8() {
  ASSIGN_OR_RETURN(const HWND foreground, GetCurrentWindowHandle());

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

static absl::StatusOr<std::string> GetExecutablePathUtf8(
    const HANDLE process_handle) {
  std::wstring buffer(262, L'\0');
  while (true) {
    const DWORD length_written =
        GetProcessImageFileNameW(process_handle, buffer.data(), buffer.size());

    if (length_written == 0) {
      return absl::InternalError(absl::StrCat(
          "GetProcessImageFileNameW failed, error=", GetLastError()));
    }

    if (length_written < buffer.size()) {
      buffer.resize(length_written);
      break;
    }

    if (buffer.size() > 32768) {
      return absl::InternalError(
          "GetProcessImageFileNameW buffer size exceeds limit");
    }

    buffer.resize(buffer.size() * 2);
  }

  return absl::StrFormat("%s", buffer);
}

static absl::StatusOr<std::string> GetCurrentExecutablePathUtf8() {
  ASSIGN_OR_RETURN(const HWND foreground, GetCurrentWindowHandle());

  DWORD process_id = 0;
  {
    const DWORD result = GetWindowThreadProcessId(foreground, &process_id);
    if (result == 0) {
      return absl::InternalError(
          absl::StrCat("failed to get procedd id, error=", GetLastError()));
    }
  }

  const HANDLE process_handle =
      OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION,  //
                  /*bInheritHandle=*/FALSE,           //
                  process_id);
  if (process_handle == nullptr) {
    return absl::InternalError(
        absl::StrCat("failed to open process, error=", GetLastError()));
  }

  return GetExecutablePathUtf8(process_handle);
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

        const absl::StatusOr<std::string> current_executable_path =
            GetCurrentExecutablePathUtf8();
        if (!current_executable_path.ok()) {
          LOG(ERROR) << current_executable_path.status();
          continue;
        }

        absl::MutexLock lock(&mu_);

        const bool title_match =
            !settings_.window_title.empty() &&
            settings_.window_title == *current_window_title;

        const bool exe_match =
            !settings_.exe_name.empty() &&
            std::filesystem::u8path(settings_.exe_name) ==
                std::filesystem::u8path(*current_executable_path).filename();

        SetObsSourceVisible(settings_.source_name, title_match || exe_match);
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
