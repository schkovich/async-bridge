// SPDX-License-Identifier: MPL-2.0
/**
 * @file SyncBridge.cpp
 * @brief Implementation of SyncBridge for thread-safe, context-aware resource
 * access
 *
 * This file implements the SyncBridge pattern, providing a mechanism for safely
 * executing operations on shared resources across different execution contexts
 * (e.g., multiple threads or cores).
 *
 * The implementation uses heap-allocated workers and semaphores for each
 * synchronous operation, ensuring reentrancy and thread safety. All
 * synchronization is per-call and per-instance.
 *
 * @author goran
 * @date 2025-02-21
 */

#include "SyncBridge.hpp"
#include "async_bridge/SyncWorker.hpp"
#include "platform/pico/SyncBridgeTb.hpp"

namespace async_bridge {

    SyncBridge::~SyncBridge() = default;

    void SyncBridge::initialiseBridge() {}

    void SyncBridge::onWork() {}

    SyncBridge::SyncBridge(IAsyncContext &ctx)
        : IEventBridge(ctx), pImpl(std::make_unique<Impl>(ctx, this)),
          m_ctx(ctx) {}

    uint32_t SyncBridge::execute(std::unique_ptr<SyncPayload> payload) {
        return pImpl->execute(std::move(payload));
    }

    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    void SyncBridge::bridgingFunction(async_context_t *context,
                                      async_when_pending_worker_t *worker) {
        (void)context;
        assert(worker && worker->user_data);
        const auto sync_worker = reinterpret_cast<sync_worker_t *>(worker);
        const auto sync_bridge = static_cast<SyncBridge *>(worker->user_data);
        assert(sync_bridge && sync_worker && sync_worker->callback);
        const auto callback = sync_worker->callback;
        sync_worker->result = (sync_bridge->*callback)(
            std::unique_ptr<SyncPayload>(sync_worker->payload));
        sem_release(&sync_worker->semaphore);
    }

    bool SyncBridge::isCrossCore() const {
        return getContext().getCore() != get_core_num();
    }

} // namespace async_bridge
