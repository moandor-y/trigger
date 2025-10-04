#include <obs-module.h>

#include "activate_obs_source.h"
#include "blog_log_sink.h"
#include "create_obs_source.h"
#include "deactivate_obs_source.h"
#include "destroy_obs_source.h"
#include "get_obs_properties.h"
#include "third_party/abseil-cpp/absl/log/log_sink_registry.h"
#include "update_obs_settings.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("trigger", "en-US")

MODULE_EXPORT const char* obs_module_description(void) { return "Trigger"; }

static const char* GetName(void* const unused) { return "Trigger"; }

bool obs_module_load() {
  absl::AddLogSink(new trigger::BlogLogSink());

  obs_source_info source_info = {};
  source_info.id = "trigger";
  source_info.type = OBS_SOURCE_TYPE_INPUT;
  source_info.get_name = &GetName;
  source_info.create = &trigger::CreateObsSource;
  source_info.destroy = &trigger::DestroyObsSource;
  source_info.get_properties = &trigger::GetObsProperties;
  source_info.update = &trigger::UpdateObsSettings;
  source_info.activate = &trigger::ActivateObsSource;
  source_info.deactivate = &trigger::DeactivateObsSource;

  obs_register_source(&source_info);

  return true;
}

void obs_module_unload() {}
