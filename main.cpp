
#include <string>
#include <cstring>
#include <iostream>

#include "protomc.hpp"
using namespace protomc;

int main( int argc, char* argv[] )
{
    try
    {
        std::string host, port;
        
        bool autodetect = // check for "-a" parameter
            (argc == 2) ? ( !strcmp( argv[1], "-a") ) : false;
        
        if ( autodetect )  // autodetect mode
        {
            protomc::lan_worlds listener;
            protomc::lan_world lan = listener.listen_once();
            
            lan.what(); // tells what got autodetected
            host = lan.host;
            port = lan.port;
        } 
        else switch ( argc )
        {
            case 2: host = argv[1], port  = "25565"; break;
            case 3: host = argv[1], port  = argv[2]; break;
            default: std::cout << "Usage: Connector <host> [port]"; return 1;
        }

        /*
        protomc::client Client( host, port );
        std::string status = Client.status();

        // displaying results
		std::cout << '\n' << status << '\n' << '\n'
			<< "\tping pong is:\t" << (Client.ping_pong_OK ? "OK!\n" : "broken!\n")
			<< "\tbytes sent:\t" << Client.bytes_sent << '\n'
			<< "\tbytes readen:\t" << Client.bytes_readen << '\n'; */
    }
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const char* str)
	{
	    std::cerr << str;
	}
}