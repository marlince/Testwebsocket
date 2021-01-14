

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/beast/ssl.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

// Sends a WebSocket message and prints the response
int main(int argc, char** argv)
{
    try
    {
        // Check command line arguments.
       //**if(argc != 0)
        //{
        //   std::cerr <<
        //     "Usage: websocket-client-sync <host> <port> <text>\n" <<
        //       "Example:\n" <<
       //    "    websocket-client-sync echo.websocket.org 80 \"Hello, world!\"\n";
       //    return EXIT_FAILURE;
       //}
        auto const host = "msoll.de";
        auto const port = "80";
        auto const text = "action";

        // The io_context is required for all I/O with the boost library
        net::io_context ioc;
        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};


		/**
		Dies ist ein Test für den SSL Handshake
	**/
		ssl::context ctx{ssl::context::tlsv12_client};
        // Look up the domain name
        auto const results = resolver.resolve(host, port);
        // Make the connection on the IP address we get from a lookup
        net::connect(ws.next_layer(), results.begin(), results.end());

		
		// Perform the SSL handshake
        ws.next_layer().handshake(ssl::stream_base::client);

        // Set a decorator to change the User-Agent of the handshake
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::request_type& req)
            {
                req.set(http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-client-coro");
            }));

		/**
		Dies ist ein Test für den SSL Handshake
		**/
        // Perform the websocket handshake in our 
        ws.handshake(host,"/spe_ed?key=B6TF63VYQO3HA6NVRI7XJZMEHEE4IXAGXPVJX5FBRSXOE7V4ODFEZWUL");
		
        // Send the Move to the Server
		ws.write(net::buffer(std::string(text)));
		
        // This buffer will hold the incoming message
		beast::multi_buffer buffer;

        // Read a message into our buffer 
        ws.read(buffer);
		
        // Close the WebSocket connection when game is over "Frage: wenn game für den Spieler over, oder allgemein gamestate"
       // if(gamestate == over){
       ws.close(websocket::close_code::normal);
      //  }
        // If we get here then the connection is closed gracefully

        // The make_printable() function helps print a ConstBufferSequence
        std::cout << beast::make_printable(buffer.data()) << std::endl;
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
