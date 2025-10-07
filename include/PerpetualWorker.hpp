// SPDX-License-Identifier: MPL-2.0
#pragma once
#include "platform/pico/types.hpp"
#include "WorkerBase.hpp"

namespace async_bridge {

    class PerpetualWorker final : public WorkerBase<PerpetualWorker, perpetual_worker_t> {

        public:
            PerpetualWorker() = default;
            ~PerpetualWorker() override = default;
    };

} // namespace async_bridge
