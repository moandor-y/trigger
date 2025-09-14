#ifndef PLUGINS_TRIGGER_GET_SETTINGS_FROM_OBS_DATA_H_
#define PLUGINS_TRIGGER_GET_SETTINGS_FROM_OBS_DATA_H_

#include <obs-module.h>

#include "settings.h"

namespace trigger {

Settings GetSettingsFromObsData(obs_data_t* const data);

}

#endif  // PLUGINS_TRIGGER_GET_SETTINGS_FROM_OBS_DATA_H_
