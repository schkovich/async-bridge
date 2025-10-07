#include "EphemeralBridge.hpp"

#include "IAsyncContext.hpp"
#include "platform/pico/types.hpp"

namespace async_bridge {

    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    void ephemeral_bridging_function(
        async_context_t *context,
        // ReSharper disable once CppParameterMayBeConstPtrOrRef
        async_work_on_timeout *worker) {
        (void)context;
        assert(worker && worker->user_data);
        const auto local_bridge =
            static_cast<EphemeralBridge *>(worker->user_data)
                ->releaseOwnership();
        worker->user_data = nullptr;
        local_bridge->doWork();
    }

    void EphemeralBridge::initialiseBridge() {
        m_ephemeral_worker.setHandler(&ephemeral_bridging_function);
        m_ephemeral_worker.setPayload(this);
    }

    void EphemeralBridge::run(const uint32_t run_in_ms) {
        if (const auto result =
                getContext().addWorker(m_ephemeral_worker, run_in_ms);
            !result) {
            DEBUGCORE("[c%d][%llu][ERROR] EphemeralBridge::run - Failed to "
                      "add ephemeral worker: %p, error: %lu\n",
                      rp2040.cpuid(), time_us_64(), this, result);
        }
    }

} // namespace async_bridge
