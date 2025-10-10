// SPDX-License-Identifier: MPL-2.0
#pragma once
#include <cassert>
#include <type_traits>
#include <utility>

namespace async_bridge {

    class EphemeralBridge;
    class IAsyncContext;

    class HandlerFactory {
        IAsyncContext& m_ctx;

        inline static HandlerFactory* s_core_zero_ctx = nullptr;
        inline static HandlerFactory* s_core_one_ctx = nullptr;

        explicit HandlerFactory(IAsyncContext& ctx) : m_ctx(ctx) {}

        static void init_with_core_zero_ctx(IAsyncContext& ctx) {
            assert(!s_core_zero_ctx && "HandlerFactory::zero already initialised");
            s_core_zero_ctx = new HandlerFactory(ctx);
        }

        static void init_with_core_one_ctx(IAsyncContext& ctx) {
            assert(!s_core_one_ctx && "HandlerFactory::one already initialised");
            s_core_one_ctx = new HandlerFactory(ctx);
        }

        public:
            HandlerFactory(const HandlerFactory&) = delete;
            HandlerFactory& operator=(const HandlerFactory&) = delete;

            static void init_with_context(IAsyncContext& ctx) {
                if (ctx.getCore() == 0)
                    init_with_core_zero_ctx(ctx);
                else
                    init_with_core_one_ctx(ctx);
            }

            static HandlerFactory& zero() {
                assert(s_core_zero_ctx && "HandlerFactory::zero not initialised");
                return *s_core_zero_ctx;
            }

            static HandlerFactory& one() {
                assert(s_core_one_ctx && "HandlerFactory::one not initialised");
                return *s_core_one_ctx;
            }

            template <typename HandlerType, typename... Args>
            void run(Args&&... args) {
                static_assert(std::is_base_of_v<EphemeralBridge, HandlerType>,
                              "HandlerType must derive from EphemeralBridge");
                HandlerType::create(m_ctx, std::forward<Args>(args)...);
            }
    };

} // namespace async_bridge
