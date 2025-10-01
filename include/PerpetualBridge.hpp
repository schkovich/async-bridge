// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "platform/pico/types.hpp"
#include "IEventBridge.hpp"
#include "PerpetualWorker.hpp"

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

    void run() const;

    // Optional workload entry point for derived classes
    void workload(void *data) override;

protected:
    PerpetualWorker *m_perpetual_worker = nullptr;
};

using PerpetualBridgePtr = std::unique_ptr<PerpetualBridge>;

} // namespace async_bridge

