#include "get_settings_from_obs_data.h"

namespace trigger {

Settings GetSettingsFromObsData(obs_data_t* const settings) {
  return {
      .source_name = obs_data_get_string(settings, "source_name"),
      .window_title = obs_data_get_string(settings, "window_title"),
  };
}

}  // namespace trigger
