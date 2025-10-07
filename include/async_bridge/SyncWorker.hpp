// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "../platform/pico/types.hpp"
#include "WorkerBase.hpp"

namespace async_bridge {

    class SyncWorker final : public WorkerBase<SyncWorker, sync_worker_t> {

        public:
            SyncWorker() = default;
            SyncWorker(const SyncWorker&) = delete;
            SyncWorker& operator=(const SyncWorker&) = delete;
            SyncWorker(SyncWorker&&) = delete;
            SyncWorker& operator=(SyncWorker&&) = delete;
            ~SyncWorker() override = default;

            semaphore_t *getSemaphore() { return &m_worker.semaphore; }
            void setSyncPayload(SyncPayloadPtr p) { m_worker.payload = p.release(); }
            SyncPayloadPtr getSyncPayload() { return SyncPayloadPtr(std::move(m_worker.payload)); }
            void setSyncCallback(const sync_callback_t cb) { m_worker.callback = cb; }
            [[nodiscard]] sync_callback_t getSyncCallback() const { return m_worker.callback; }
            [[nodiscard]] std::uint32_t getResult() const { return m_worker.result; }
            void setResult(const uint32_t r) { m_worker.result = r; }
            void initSemaphore() { sem_init(&m_worker.semaphore, 0, 1); }
            void semaphoreAcquireBlocking() { sem_acquire_blocking(&m_worker.semaphore); }
            void semaphoreRelease() { sem_release(&m_worker.semaphore); }
    };

} // namespace async_bridge
