//
// Created by goran on 28/09/2025.
//

#pragma once
#include <Arduino.h>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <pico/async_context.h>

namespace async_bridge {

    class SyncBridge;
    struct SyncPayload;
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

    using ephemeral_bridging_function_t = void (*)(async_context_t *,
                                                   async_work_on_timeout *);

    /**
     * @struct SyncPayload
     * @brief Base type for synchronous work payloads
     *
     * This structure serves as a polymorphic base for all payload types that
     * can be passed to SyncBridge for thread-safe execution. It defines a
     * common interface for different types of work data.
     *
     * @note Derived payload types must be final to prevent slicing issues
     * during polymorphic use.
     */
    struct SyncPayload {
        SyncPayload() noexcept = default;
        virtual ~SyncPayload() noexcept = default;
    };


    /**
     * @typedef SyncPayloadPtr
     * @brief Convenience alias for a unique pointer to a SyncPayload
     */
    using SyncPayloadPtr = std::unique_ptr<SyncPayload>;

    using sync_callback_t = uint32_t (SyncBridge::*)(SyncPayloadPtr payload);

    typedef struct sync_worker {
            async_when_pending_worker_t worker{}; // supertype C‑style field
            semaphore_t semaphore{};
            sync_callback_t callback{};
            SyncPayload *payload{nullptr};
            uint32_t result{0};

            // Non-copyable
            sync_worker() = default;
            sync_worker(const sync_worker &) = delete;
            sync_worker &operator=(const sync_worker &) = delete;
    } sync_worker;

    using sync_worker_t = sync_worker;

    static_assert(std::is_standard_layout_v<sync_worker_t>,
                  "sync_worker must be standard-layout for owner pointer "
                  "arithmetic to be valid");

    static_assert(offsetof(sync_worker_t, worker) == 0,
                  "sync_worker::worker must be at offset 0 so "
                  "async_when_pending_worker_t* == sync_worker*");

    typedef struct perpetual_worker {
            async_when_pending_worker_t worker{}; // supertype C‑style field

            // Non-copyable
            perpetual_worker() = default;
            perpetual_worker(const perpetual_worker &) = delete;
            perpetual_worker &operator=(const perpetual_worker &) = delete;
    } perpetual_worker;

    using perpetual_worker_t = perpetual_worker;

    static_assert(std::is_standard_layout_v<perpetual_worker_t>,
                  "perpetual_worker must be standard-layout for owner pointer "
                  "arithmetic to be valid");

    static_assert(offsetof(perpetual_worker_t, worker) == 0,
                  "perpetual_worker_t::worker must be at offset 0 so "
                  "async_when_pending_worker_t* == perpetual_worker_t*");

    typedef struct ephemeral_worker {
            async_at_time_worker_t worker{}; // supertype C‑style field

            // Non-copyable
            ephemeral_worker() = default;
            ephemeral_worker(const ephemeral_worker &) = delete;
            ephemeral_worker &operator=(const ephemeral_worker &) = delete;
    } ephemeral_worker;

    using ephemeral_worker_t = ephemeral_worker;

    static_assert( std::is_standard_layout_v<ephemeral_worker_t>,
                  "ephemeral_worker must be standard-layout for owner pointer "
                  "arithmetic to be valid");

    static_assert(offsetof(ephemeral_worker_t, worker) == 0,
                  "ephemeral_worker_t::worker must be at offset 0 so "
                  "async_at_time_worker_t* == ephemeral_worker_t*");
} // namespace async_bridge