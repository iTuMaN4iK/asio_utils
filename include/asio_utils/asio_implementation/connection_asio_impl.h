﻿#pragma once

#include "asio_utils/generic/generic_impl.h"
#include "asio_utils/traits.h"

#include <memory>

#include <asio.hpp>

namespace eld
{
    namespace impl_asio
    {
        namespace traits
        {
        }

        namespace detail
        {
            // TODO: make SFINAE overload that uses composed asio::async_write
            // TODO: implement
            template<typename ConnectionT>
            class async_write
            {
            public:
                async_write(ConnectionT &connection)   //
                  : pState_(std::make_shared<state>(connection))
                {
                }

                template<typename ConstBuffer, typename WriteHandler>
                auto operator()(ConstBuffer &&constBuffer, WriteHandler &&writeHandler)
                {
                }

            private:
                struct state
                {
                    state(ConnectionT &connection, size_t bytesToSend) : connection_(connection) {}

                    ConnectionT &connection_;
                    size_t bytesToSend_;
                };

                std::shared_ptr<state> pState_;
            };
        }

        struct connection_asio_impl
        {
            using error_type = asio::error_code;
            using buffer_type = std::array<uint8_t, 2048>;

            struct error
            {
                constexpr static auto operation_aborted() { return asio::error::operation_aborted; }

                constexpr static auto already_started() { return asio::error::already_started; }
            };

            template<typename ConnectionT>
            static constexpr bool remote_host_has_disconnected(const ConnectionT &,
                                                               const error_type &errorCode)
            {
                return errorCode == asio::error::eof ||   //
                       errorCode == asio::error::connection_reset;
            }

            template<typename ConfigTL, typename ConfigTR>
            static constexpr bool compare_configs(const ConfigTL &lhv, const ConfigTR &rhv)
            {
                return impl::default_false_compare<ConfigTL, ConfigTR>::value(lhv, rhv);
            }

            //////////////////////////////////////////////
            // class methods
            //////////////////////////////////////////////

            template<typename ConnectionT>
            static constexpr typename eld::traits::connection<ConnectionT>::config_type get_config(
                const ConnectionT &connection)
            {
                return connection.get_config();
            }

            template<typename T>
            static constexpr void cancel(T &t)
            {
                t.cancel();
            }

            template<typename T, typename ConfigT>
            static void configure(T &t, ConfigT &&config)
            {
                t.configure(std::forward<ConfigT>(config));
            }

            // TODO: check completion signature
            template<typename Connection, typename MutableBuffer, typename CompletionT>
            static auto async_receive(Connection &connection,
                                      MutableBuffer &&mutableBuffer,
                                      CompletionT &&completion)
            {
                return connection.async_receive(
                    asio::buffer(std::forward<MutableBuffer>(mutableBuffer)),
                    std::forward<CompletionT>(completion));
            }

            // TODO: check completion signature
            template<typename Connection, typename ConstBuffer, typename CompletionT>
            static auto async_send(Connection &connection,
                                   ConstBuffer &&constBuffer,
                                   CompletionT &&completion)
            {
                //                return asio::async_write(connection,
                //                                         std::forward<ConstBuffer>(constBuffer),
                //                                         std::forward<CompletionT>(completion));

                // TODO: implement and use detail::async_write

                return connection.async_send(std::forward<ConstBuffer>(constBuffer),
                                             std::forward<CompletionT>(completion));
            }
        };
    }
}
