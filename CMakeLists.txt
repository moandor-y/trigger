cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(trigger)

set(ARCH x86_64)

add_library(trigger MODULE
    plugin_main.cc
    create_obs_source.cc
    get_settings_from_obs_data.cc
    update_obs_settings.cc
)

target_link_libraries(trigger
    PRIVATE
        OBS::libobs
        OBS::frontend-api
)

set_target_properties(trigger PROPERTIES FOLDER "plugins/trigger")
