#ifndef PROTOMC_LAN_WORLDS_HPP
#define PROTOMC_LAN_WORLDS_HPP

#include <regex>
#include <string>
#include <iostream>
#include "client.hpp"

namespace protomc 
{
    const short multicast_port = 4445;

    const asio::ip::address listen_all = 
        asio::ip::address::from_string("0.0.0.0");
    const asio::ip::address multicast  = 
        asio::ip::address::from_string("224.0.2.60");
    
    class lan_world 
    {
        public:
        lan_world():empty(true){};
        
        bool empty;
        std::string player, world, host, port;

        void what()
        {
            if( empty )
            {
                std::cout << "Autodetection entry is empty.\n";
            }
            else
            {
                std::cout << "Autodetected " << player << "'s " << world 
                << ' ' << "on " << host << ':' << port << '\n';
            }
        }

    };

    class lan_worlds
    {
        public:
            lan_worlds()
            : socket_( client::io_service)
            {
                // Create the socket so that multiple may be bound to the same address.
                asio::ip::udp::endpoint listen_endpoint( listen_all, multicast_port );
                socket_.open( listen_endpoint.protocol() );
                socket_.set_option( asio::ip::udp::socket::reuse_address( true ) );
                socket_.bind( listen_endpoint );

                // Join the multicast group.
                socket_.set_option( asio::ip::multicast::join_group( multicast ) );
            }

            lan_world listen_once()
            {
                size_t bytes_recvd =
                socket_.receive_from( 
                    asio::buffer(data_, max_length), 
                    sender_endpoint_ );

                std::regex re{R"(\[MOTD\](\w+) - (.+)\[/MOTD\]\[AD](\d+)\[/AD])"};
                std::string sample_text( data_, bytes_recvd );
                std::smatch match;

                lan_world lan;
                if ( std::regex_match(sample_text, match, re) )
                {
                    lan.host = sender_endpoint_.address().to_string();
                    lan.port = match[3];
                    lan.world = match[2];
                    lan.player = match[1];
                    lan.empty = false;
                } 
                else throw "LAN world finder failed on regex";

                return lan;
            }
        
        private:
            asio::ip::udp::socket socket_;
            asio::ip::udp::endpoint sender_endpoint_;
            enum { max_length = 1024 };
            char data_[max_length];
    };
}

#endif // PROTOMC_LAN_WORLDS_HPP
