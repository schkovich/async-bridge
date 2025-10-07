// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "WorkerBase.hpp"

namespace async_bridge {
    class EphemeralWorker final : public WorkerBase<EphemeralWorker, ephemeral_worker_t> {

        public:
            EphemeralWorker() = default;
            ~EphemeralWorker() override = default;
    };

} // namespace async_bridge
