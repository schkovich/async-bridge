//
// Created by goran on 30/09/2025.
//

#pragma once
#include "SyncBridge.hpp"

#include <pico/mutex.h>

namespace async_bridge::platform {

    class PicoSyncBridge : public SyncBridge {
        /**
         * @brief Recursive mutex for serializing access to execute() per
         * SyncBridge instance.
         *
         * Ensures that only one thread or context can execute a synchronous
         * operation on this instance at a time. Initialized in the
         * constructor. Not shared between calls or instances.
         */
        recursive_mutex_t m_execution_mutex = {};

        protected:

            [[nodiscard]] bool isCrossCore() {
                return m_ctx.getCore() != static_cast<uint8_t>(get_core_num());
            }

            // Acquire/release the async context lock for same-core safe sections
            void ctxLock() const { m_ctx.acquireLock(); }
            void ctxUnlock() const { m_ctx.releaseLock(); }

        public:
            explicit PicoSyncBridge(const IAsyncContext &ctx) : SyncBridge(ctx) {
                recursive_mutex_init(&m_execution_mutex);
            }
            ~PicoSyncBridge() override = default;
    };

} // namespace async_bridge

