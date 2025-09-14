#include "create_obs_source.h"

#include <obs-frontend-api.h>
#include <obs-module.h>

#include <string>

#include "get_settings_from_obs_data.h"
#include "settings.h"
#include "trigger.h"

namespace trigger {

void* CreateObsSource(obs_data_t* const settings_obs_data,
                      obs_source_t* const source) {
  const Settings settings = GetSettingsFromObsData(settings_obs_data);

  obs_source_t* const scene_source = obs_frontend_get_current_scene();
  obs_scene_t* const scene = obs_scene_from_source(scene_source);

  obs_sceneitem_t* const source_item =
      obs_scene_find_source_recursive(scene, settings.source_name.c_str());

  obs_sceneitem_set_visible(source_item, false);

  obs_source_release(scene_source);

  return new Trigger(settings);
}

}  // namespace trigger
