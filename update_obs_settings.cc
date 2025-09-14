#include "update_obs_settings.h"

#include "get_settings_from_obs_data.h"
#include "trigger.h"

namespace trigger {

void UpdateObsSettings(void* const custom_data,
                       obs_data_t* const settings_obs_data) {
  Trigger& trigger = *static_cast<Trigger*>(custom_data);
  trigger.UpdateSettings(GetSettingsFromObsData(settings_obs_data));
}

}  // namespace trigger
