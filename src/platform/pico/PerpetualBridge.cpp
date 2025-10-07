#include "../../../include/async_bridge/PerpetualBridge.hpp"

#include "async_bridge/IAsyncContext.hpp"

namespace async_bridge {

    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    void perpetual_bridging_function(async_context_t *context,
        // ReSharper disable once CppParameterMayBeConstPtrOrRef
        async_when_pending_worker_t *worker) {
        (void)context;
        assert(worker && worker->user_data);
        static_cast<PerpetualBridge *>(worker->user_data)->doWork();
    }

    void PerpetualBridge::initialiseBridge() {
        m_perpetual_worker.setHandler(&perpetual_bridging_function);
        m_perpetual_worker.setPayload(this);
        getContext().addWorker(m_perpetual_worker);
    }

    void PerpetualBridge::run() {
        getContext().setWorkPending(m_perpetual_worker);
    }

    void PerpetualBridge::workload(void *data) {/* No-op by default */}
} // namespace async_tcp
