

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
using response_type = http::response< http::string_body >;
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
        auto const json = "";
		
		
		response_type res;
        // The io_context is required for all I/O with the boost library
        net::io_context ioc;
        // These objects perform our I/O
        tcp::resolver resolver{ioc};
        websocket::stream<tcp::socket> ws{ioc};

        // Look up the domain name
        auto const results = resolver.resolve(host, port);
        // Make the connection on the IP address we get from a lookup
        net::connect(ws.next_layer(), results.begin(), results.end());
		std::cout << "res";
        // Perform the websocket handshake spe_ed?key=B6TF63VYQO3HA6NVRI7XJZMEHEE4IXAGXPVJX5FBRSXOE7V4ODFEZWUL
        ws.handshake(res, host,"/spe_ed?key=B6TF63VYQO3HA6NVRI7XJZMEHEE4IXAGXPVJX5FBRSXOE7V4ODFEZWUL");
		std::cout << res;
        // Send the Move to the Server
		ws.write(net::buffer(std::string(json)));
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
