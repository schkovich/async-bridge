// ------------------------------------------------------------
// async_bridge/traits.hpp
#pragma once
#include <type_traits>
#include "platform/pico/types.hpp"
namespace async_bridge {

    class PerpetualWorker;
    class EphemeralWorker;
    class SyncWorker;
    template <class T> struct handler_traits {}; // primary template
    template <> struct handler_traits<EphemeralWorker> {
        using type = ephemeral_bridging_function_t;
    };
    template <> struct handler_traits<PerpetualWorker> {
        using type = perpetual_bridging_function_t;
    };
    template <> struct handler_traits<SyncWorker> {
        using type = perpetual_bridging_function_t;
    };

    template <class T, class = void> struct has_do_work : std::false_type {};

    template <class T>
    struct has_do_work<
        T,
        std::void_t<decltype(std::declval<T>().worker), // type of worker
                    decltype(std::declval<std::remove_reference_t<
                                 decltype(std::declval<T>().worker)>>()
                                 .do_work) // data member
                    >> : std::true_type {};

    template <class T, class = void> struct has_user_data : std::false_type {};

    template <class T>
    struct has_user_data<
        T, std::void_t<decltype(std::declval<T>().worker.user_data)>>
        : std::true_type {};

} // namespace async_bridge
