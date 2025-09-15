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
  return new Trigger(GetSettingsFromObsData(settings_obs_data));
}

}  // namespace trigger
