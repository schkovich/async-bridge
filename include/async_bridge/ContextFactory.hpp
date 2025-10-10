#pragma once
#include "IAsyncContext.hpp"
#include <cstdint>

namespace async_bridge {

    /**
     * Initialize async_context for core 0.
     * @param max_timers Maximum number of timers for the custom alarm pool.
     *                  If 0, uses the default alarm pool. If > 0, creates a custom
     *                  alarm pool with the specified number of timers.
     */
    bool init_context_zero(uint8_t max_timers = 0);

    /**
     * Initialize async_context for core 1.
     * @param max_timers Maximum number of timers for the custom alarm pool.
     *                  If 0, uses the default alarm pool. If > 0, creates a custom
     *                  alarm pool with the specified number of timers.
     */
    bool init_context_one(std::uint8_t max_timers = 0);

    /** Accessors. The corresponding init_* must have been called. */
    IAsyncContext& get_context_zero();
    IAsyncContext& get_context_one();

    /** Optional cleanup (must be called from the same core, which did init). */
    void de_init_context_zero();
    void de_init_context_one();

    /** Query helpers. */
    bool is_context_zero_initialized();
    bool is_context_one_initialized();

} // namespace async_bridge
