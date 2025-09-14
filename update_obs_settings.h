#ifndef PLUGINS_TRIGGER_UPDATE_OBS_SETTINGS_H_
#define PLUGINS_TRIGGER_UPDATE_OBS_SETTINGS_H_

#include <obs-module.h>

namespace trigger {

void UpdateObsSettings(void* custom_data, obs_data_t* settings_obs_data);

}

#endif  // PLUGINS_TRIGGER_UPDATE_OBS_SETTINGS_H_
