//
// Created by goran on 08/10/2025.
//
#include "async_bridge/AlarmPoolFactory.hpp"
#include <pico/time.h>

namespace async_bridge {

    alarm_pool* AlarmPoolFactory::make(const uint8_t max_timers) {
        // Pico SDK will pick the next available hardware alarm ≥ argument.
        return alarm_pool_create_with_unused_hardware_alarm(max_timers);
    }

    void AlarmPoolFactory::destroy(alarm_pool* pool) {
        alarm_pool_destroy(pool);
    }
} // namespace async_bridge
