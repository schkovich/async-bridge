#include "async_bridge/ContextFactory.hpp"
#include "async_bridge/AlarmPoolFactory.hpp"
#include "platform/pico/ContextManager.hpp"

// #include <pico/async_context_threadsafe_background.h>
#include <pico/time.h>
#include <cassert>
#include <memory>

namespace async_bridge {

static std::unique_ptr<ContextManager> s_ctx_core0;
static std::unique_ptr<ContextManager> s_ctx_core1;
static alarm_pool* s_pool_core0 = nullptr;
static alarm_pool* s_pool_core1 = nullptr;

static bool create_context_for_current_core(std::unique_ptr<ContextManager>& out_ctx,
                                            alarm_pool*& out_pool,
                                const uint8_t max_timers) {

    auto ctx = std::make_unique<ContextManager>();
    auto cfg = async_context_threadsafe_background_default_config();
    if (max_timers > 0) {
        out_pool = AlarmPoolFactory::make(max_timers);
        if (!out_pool) return false;
        cfg.custom_alarm_pool = out_pool;
    }

    if (!ctx->initDefaultContext(cfg)) {
        if (max_timers > 0) AlarmPoolFactory::destroy(out_pool);
        return false;
    }

    out_ctx = std::move(ctx);
    return true;
}

// ---------------------------------------------------------------------------
// Context for Core 0
// ---------------------------------------------------------------------------
bool init_context_zero(const uint8_t max_timers) {
    if (s_ctx_core0) return true;
    assert(get_core_num() == 0 && "init_context_zero must be called from core 0");
    return create_context_for_current_core(s_ctx_core0, s_pool_core0, max_timers);
}

IAsyncContext& get_context_zero() {
    assert(s_ctx_core0 && "context_zero not initialized");
    return *s_ctx_core0;
}

void de_init_context_zero() {
    if (!s_ctx_core0) return;
    s_ctx_core0.reset();
    if (s_pool_core0) {
        AlarmPoolFactory::destroy(s_pool_core0);
        s_pool_core0 = nullptr;
    }
}

bool is_context_zero_initialized() { return static_cast<bool>(s_ctx_core0); }

// ---------------------------------------------------------------------------
// Context for Core 1
// ---------------------------------------------------------------------------
bool init_context_one(const uint8_t max_timers) {
    if (s_ctx_core1) return true;
    assert(get_core_num() == 1 && "init_context_one must be called from core 1");
    return create_context_for_current_core(s_ctx_core1, s_pool_core1, max_timers);
}

IAsyncContext& get_context_one() {
    assert(s_ctx_core1 && "context_one not initialized");
    return *s_ctx_core1;
}

void de_init_context_one() {
    if (!s_ctx_core1) return;
    s_ctx_core1.reset();
    if (s_pool_core1) {
        AlarmPoolFactory::destroy(s_pool_core1);
        s_pool_core1 = nullptr;
    }
}

bool is_context_one_initialized() { return static_cast<bool>(s_ctx_core1); }

} // namespace async_bridge
