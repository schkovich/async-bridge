//
// Created by goran on 28/09/2025.
//

#pragma once
#include <cstdint>
#include <pico/async_context.h>

namespace async_bridge {
    /**
     * @typedef uint8_t byte
     * @brief Defines a byte as an unsigned 8-bit integer.
     *
     * This type alias improves code readability by providing a clear
     * representation of a byte, which is commonly used in low-level data
     * manipulation and communication protocols.
     */
    using byte = std::uint8_t;

    /**
     * @brief Function pointer type for asynchronous work handlers.
     *
     * This type represents a pointer to a function that takes a void pointer as
     * its argument and returns an uint32_t. It is used as the callback
     * signature for executing work synchronously through the async_context
     * system.
     *
     * @note The void* parameter allows passing arbitrary data to the handler,
     * and the uint32_t return value can be used to indicate success, failure
     * or other status information.
     */
    using handler_function_t = uint32_t (*)(void *param);

    using perpetual_bridging_function_t =
        void (*)(async_context_t *, async_when_pending_worker_t *);

    using ephemeral_bridging_function_t =
        void (*)(async_context_t *,
                                  async_work_on_timeout *);

    using perpetual_worker_t = async_when_pending_worker_t;
    using ephemeral_worker_t = async_at_time_worker_t;

    extern "C" {
        // Forward declaration of the perpetual bridging function
        void perpetual_bridging_function(async_context_t *context,
                                         async_when_pending_worker_t *worker);

        // Forward declaration of the ephemeral bridging function
        void ephemeral_bridging_function(async_context_t *context,
                                         async_work_on_timeout *worker);
        /**
         * @brief Synchronous bridging function for async context worker execution (C
         * linkage).
         *
         * This function is registered as the handler for PerpetualWorker in
         * SyncBridge::execute(). It is always called in the correct async context,
         * executes the user operation, and signals completion by releasing the
         * stack-allocated semaphore in the ExecutionContext.
         *
         * @param context Pointer to the async context (unused).
         * @param worker  Pointer to the async_when_pending_worker_t containing the
         * ExecutionContext.
         */
        void sync_bridging_function(async_context_t *context,
                                     async_when_pending_worker_t *worker);

    }
} // namespace async_bridge