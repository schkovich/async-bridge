// cpp
// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "traits.hpp"
#include <type_traits>
#include <utility>

namespace async_bridge {

    template <class Derived, class WorkerT> class WorkerBase {

        protected:
            WorkerT m_worker{}; // owned worker instance

            // ------------------------------------------------------------------
            template <class... Args>
            auto callHandler(Args &&...args)
                -> std::invoke_result_t<typename handler_traits<Derived>::type,
                                        Args...> {
                // using raw_handler_t = typename handler_traits<Derived>::type;
                using result_t = std::invoke_result_t<raw_handler_t, Args...>;

                // fetch the function pointer stored in the worker
                auto f = getCoreWorker().do_work;

                if constexpr (std::is_void_v<result_t>) {
                    if (f)
                        f(std::forward<Args>(args)...);
                    return;
                } else {
                    if (f)
                        return f(std::forward<Args>(args)...);
                    return result_t{};
                }
            }

        public:
            using raw_handler_t = typename handler_traits<Derived>::type;

            static_assert(
                std::is_pointer_v<raw_handler_t>,
                "handler_traits<Derived>::type must be a pointer to function");
            static_assert(
                std::is_function_v<std::remove_pointer_t<raw_handler_t>>,
                "handler_traits<Derived>::type must point to a function type");

            static_assert(has_user_data<WorkerT>::value,
                          "WorkerT must have a `user_data` member");

            static_assert(has_do_work<WorkerT>::value,
                          "WorkerT must have a `do_work` member");
            static_assert(
                std::is_same_v<raw_handler_t,
                               decltype(std::declval<WorkerT>().worker.do_work)>,
                "handler_traits<Derived>::type must match WorkerT::worker.do_work "
                "type");

            WorkerBase() = default;
            virtual ~WorkerBase() = default;

            // ------------------------------------------------------------------
            // Accessors – return the *worker* sub‑object, not the enclosing
            // class
            WorkerT &getWorker() noexcept { return m_worker; }
            const WorkerT &getWorker() const noexcept { return m_worker; }

            // Return the inner worker (core worker)
            auto &getCoreWorker() noexcept { return m_worker.worker; }
            const auto &getCoreWorker() const noexcept { return m_worker.worker; }

            // ------------------------------------------------------------------
            // API to install / clear the handler stored in the worker
            void setHandler(raw_handler_t h) noexcept {

                getCoreWorker().do_work = h;
            }

            void clearHandler() noexcept {
                getCoreWorker().do_work = nullptr;
            }

            // ------------------------------------------------------------------
            // Payload handling (user_data field)
            void setPayload(void *data) noexcept {
                getCoreWorker().user_data = data;
            }
            void *getPayload() noexcept { return getCoreWorker().user_data; }
            [[nodiscard]] const void *getPayload() const noexcept {
                return getCoreWorker().user_data;
            }
    };

} // namespace async_bridge
