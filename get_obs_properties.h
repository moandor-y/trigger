#ifndef PLUGINS_TRIGGER_GET_OBS_PROPERTIES_H_
#define PLUGINS_TRIGGER_GET_OBS_PROPERTIES_H_

#include <obs-module.h>

namespace trigger {

obs_properties_t* GetObsProperties(void* data) {
  obs_properties_t* properties = obs_properties_create();

  obs_properties_add_text(properties, "source_name", "Source name",
                          OBS_TEXT_DEFAULT);
  obs_properties_add_text(properties, "window_title", "Window title",
                          OBS_TEXT_DEFAULT);

  return properties;
}

}  // namespace trigger

#endif  // PLUGINS_TRIGGER_GET_OBS_PROPERTIES_H_
