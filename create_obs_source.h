#ifndef PLUGINS_TRIGGER_CREATE_OBS_SOURCE_H_
#define PLUGINS_TRIGGER_CREATE_OBS_SOURCE_H_

#include <obs-module.h>

namespace trigger {

void* CreateObsSource(obs_data_t* settings_obs_data, obs_source_t* source);

}  // namespace trigger

#endif  // PLUGINS_TRIGGER_CREATE_OBS_SOURCE_H_
