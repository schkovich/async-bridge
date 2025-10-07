// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "async_bridge/IAsyncContext.hpp"
#include "async_bridge/IEventBridge.hpp"

#include <memory>

namespace async_bridge {
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
