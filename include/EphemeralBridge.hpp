// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "async_bridge/EphemeralWorker.hpp"
#include "async_bridge/IEventBridge.hpp"

#include <memory>

namespace async_bridge {

    class EphemeralBridge : public IEventBridge {

            friend void
            ephemeral_bridging_function(async_context_t *context,
                                        async_work_on_timeout *worker);
            EphemeralWorker m_ephemeral_worker = {};
            std::unique_ptr<EphemeralBridge> m_self =
                nullptr; /**< Self-reference for automatic cleanup */
            void takeOwnership(std::unique_ptr<EphemeralBridge> self) {
                m_self = std::move(self);
            }
            std::unique_ptr<EphemeralBridge> releaseOwnership() {
                return std::move(m_self);
            }

        public:
            explicit EphemeralBridge(IAsyncContext &ctx) : IEventBridge(ctx) {}

            void initialiseBridge() override;

            void run(uint32_t run_in_ms = 0);

        protected:
            template <typename DerivedHandler>
            static void runHandler(std::unique_ptr<DerivedHandler> handler) {
                DerivedHandler *raw_ptr = handler.get();
                raw_ptr->takeOwnership(std::move(handler));
                handler.reset(); // Release unique_ptr to avoid double deletion
                raw_ptr->initialiseBridge();
                raw_ptr->run(0);
            }
    };

} // namespace async_bridge
