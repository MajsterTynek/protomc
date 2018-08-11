#ifndef PROTOMC_CLIENT_HPP
#define PROTOMC_CLIENT_HPP

#include <asio.hpp>

namespace protomc 
{
    class client 
    {
        public:
        static asio::io_service io_service;
    };
}

#endif // PROTOMC_CLIENT_HPP