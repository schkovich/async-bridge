// SPDX-License-Identifier: MPL-2.0
#pragma once

#include "IAsyncContext.hpp"

#include <type_traits>
#include <utility>

namespace async_bridge {

    class EphemeralBridge;

    /**
     * Minimal factory used to create ephemeral handlers. During migration this
     * can be replaced by a DI-friendly factory; for now a small singleton
     * helper is provided for convenience in tests and examples.
     */
    class HandlerFactory {
            IAsyncContext &m_ctx;
            inline static HandlerFactory *s_instance =
                nullptr; /**< Singleton instance pointer */
            explicit HandlerFactory(IAsyncContext &ctx) : m_ctx(ctx) {}

        public:
            /**
             * @brief Initializes the singleton instance
             *
             * Must be called once during application startup before any
             * handlers are created.
             *
             * @param ctx Reference to the AsyncCtx used by all handlers
             */
            static void initialise(AsyncCtx &ctx) {
                s_instance = new HandlerFactory(ctx);
            }

            /**
             * @brief Gets the singleton instance
             *
             * @return Reference to the singleton HandlerFactory instance
             * @throws assertion failure if called before initialise()
             */
            static HandlerFactory &instance() {
                assert(s_instance &&
                       "HandlerFactory must be initialised before use");
                return *s_instance;
            }
            template <typename HandlerType, typename... Args>
            void run(Args &&...args) {
                static_assert(std::is_base_of_v<EphemeralBridge, HandlerType>,
                              "HandlerType must derive from EphemeralBridge");
                HandlerType::create(m_ctx, std::forward<Args>(args)...);
            }
    };

} // namespace async_bridge
