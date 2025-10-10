#pragma once
#include <cstdint>

struct alarm_pool; // forward declaration (from Pico SDK)

namespace async_bridge {

    /**
     * @brief Encapsulates creation of Pico SDK alarm pools.
     */
    class AlarmPoolFactory {
        public:
            /**
             * @brief Create a new alarm pool using an unused hardware alarm.
             *
             * @param max_timers  Maximum number of concurrent timers (default 16).
             * @return Pointer to the created pool, or nullptr on failure.
             *
             * The caller (or the library) is responsible for destroying the pool later
             * via alarm_pool_destroy(), or by delegating it to the context that owns it.
             */
            static alarm_pool* make(uint8_t max_timers = 16);

            /**
             * @brief Destroy an alarm pool.
             *
             * @param pool Pointer to the alarm pool to destroy.
             */
            static void destroy(alarm_pool* pool);
    };

} // namespace async_bridge
