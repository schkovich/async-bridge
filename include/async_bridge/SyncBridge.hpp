// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "IAsyncContext.hpp"
#include "IEventBridge.hpp"
#include "platform/pico/types.hpp"

#include <memory>

namespace async_bridge {

    class SyncBridge : public IEventBridge {
            class Impl;
            std::unique_ptr<Impl> pImpl{};
            const IAsyncContext &m_ctx;

            /**
             * @brief Pure virtual method to be implemented by derived classes
             * for actual resource operation.
             *
             * This method is called in the correct execution context by the
             * SyncBridge machinery. It receives a unique payload for the
             * operation and must return a result code.
             *
             * @param payload A unique pointer to operation data for this
             * execution.
             * @return uint32_t Result code from the operation.
             */
            virtual uint32_t onExecute(SyncPayloadPtr payload) = 0;

        public:
            explicit SyncBridge(IAsyncContext &ctx);
            ~SyncBridge() override;

            void initialiseBridge() override;

            // Execute synchronously on the context; blocks until completion
            [[nodiscard]] uint32_t execute(std::unique_ptr<SyncPayload> payload);

        protected:
            static void bridgingFunction(async_context_t *context,
                                         async_when_pending_worker_t *worker);

            void onWork() override;

            [[nodiscard]] bool isCrossCore() const;

            // Acquire/release the async context lock for same-core safe
            // sections
            void ctxLock() const { m_ctx.acquireLock(); }
            void ctxUnlock() const { m_ctx.releaseLock(); }
    };

} // namespace async_bridge
