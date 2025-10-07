// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "async_bridge/IEventBridge.hpp"
#include "async_bridge/PerpetualWorker.hpp"
#include "platform/pico/types.hpp"

#include <memory>

namespace async_bridge {

class PerpetualWorker; // forward-declared handle

class PerpetualBridge : public IEventBridge {
    friend void perpetual_bridging_function(async_context_t *context,
                                       async_when_pending_worker_t *worker);
public:
    explicit PerpetualBridge(IAsyncContext &ctx) : IEventBridge(ctx) {}

    // Implementations should remove worker in the destructor via context
    ~PerpetualBridge() override = default;

    void initialiseBridge() override;

    void run();

    // Optional workload entry point for derived classes
    void workload(void *data) override;

protected:
    PerpetualWorker m_perpetual_worker = {};
};

using PerpetualBridgePtr = std::unique_ptr<PerpetualBridge>;

} // namespace async_bridge

