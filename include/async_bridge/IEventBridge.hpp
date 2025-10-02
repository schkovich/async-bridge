// SPDX-License-Identifier: MPL-2.0
#pragma once
#include "../IAsyncContext.hpp"

namespace async_bridge {
    class IAsyncContext;

    class IEventBridge {
protected:
    IAsyncContext &m_ctx;

    explicit IEventBridge(IAsyncContext &ctx) : m_ctx(ctx) {}

    virtual void onWork() = 0;

    void doWork() { onWork(); }

    [[nodiscard]] const IAsyncContext &getContext() const { return m_ctx; }

    [[nodiscard]] IAsyncContext &getContext() noexcept { return m_ctx; }

public:
    virtual ~IEventBridge() = default;

    /**
     * Initialise any runtime structures required by the bridge. Implementations
     * should register workers with the context here.
     */
    virtual void initialiseBridge() = 0;

    virtual void workload(void *data) = 0;
};

} // namespace async_bridge


