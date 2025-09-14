cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR OFF)

project(trigger)

set(ARCH x86_64)

add_subdirectory(third_party/abseil-cpp)

add_library(trigger MODULE
    plugin_main.cc
    create_obs_source.cc
    get_settings_from_obs_data.cc
    update_obs_settings.cc
    trigger.cc
)

target_link_libraries(trigger
    PRIVATE
        OBS::libobs
        OBS::frontend-api
        absl::synchronization
        absl::time
)

set_target_properties(trigger PROPERTIES FOLDER "plugins/trigger")
