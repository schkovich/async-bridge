// SPDX-License-Identifier: MPL-2.0
#pragma once

#include <pico/mutex.h>

#include "async_bridge/SyncBridge.hpp"
#include "platform/pico/types.hpp"

namespace async_bridge {

    class SyncBridge::Impl {
            SyncBridge *const m_bridge;
            const IAsyncContext &m_ctx;
            recursive_mutex_t m_execution_mutex;

            void lockBridge();
            void unlockBridge();
            [[nodiscard]] std::uint32_t
            executeWork(SyncPayloadPtr payload);

        public:
            Impl(const IAsyncContext &ctx, SyncBridge *bridge);
            ~Impl() = default;
            uint32_t execute(SyncPayloadPtr payload);
    };
} // namespace async_bridge
