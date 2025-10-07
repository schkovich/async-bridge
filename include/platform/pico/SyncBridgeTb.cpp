// SPDX-License-Identifier: MPL-2.0
/**
 * @file SyncBridgeTb.cpp
 * @brief Private implementation details for SyncBridge on Pico platform
 */

#include "SyncBridgeTb.hpp"

#include "../../async_bridge/SyncWorker.hpp"

namespace async_bridge {

    SyncBridge::Impl::Impl(const IAsyncContext &ctx, SyncBridge *bridge)
        : m_bridge(bridge), m_ctx(ctx) {
        recursive_mutex_init(&m_execution_mutex);
    }

    void SyncBridge::Impl::lockBridge() {
        recursive_mutex_enter_blocking(&m_execution_mutex);
    }

    void SyncBridge::Impl::unlockBridge() {
        recursive_mutex_exit(&m_execution_mutex);
    }

    uint32_t SyncBridge::Impl::executeWork(SyncPayloadPtr payload) {
        SyncWorker syncWorker{};
        syncWorker.initSemaphore();
        syncWorker.setHandler(bridgingFunction);
        syncWorker.setSyncCallback(&SyncBridge::onExecute);
        syncWorker.setSyncPayload(std::move(payload));
        syncWorker.setPayload(this);
        m_ctx.addWorker(syncWorker);
        m_ctx.setWorkPending(syncWorker);
        syncWorker.semaphoreAcquireBlocking();
        const auto result = syncWorker.getResult();
        syncWorker.semaphoreRelease();
        m_ctx.removeWorker(syncWorker);
        return result;
    }

    uint32_t SyncBridge::Impl::execute(SyncPayloadPtr payload) {
        lockBridge();
        const auto result = executeWork(std::move(payload));
        unlockBridge();
        return result;
    }

} // namespace async_bridge
