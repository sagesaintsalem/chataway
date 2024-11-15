#include "Connections.h"
#include "Peer.h"

void startConnection(int& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {
	//cout << "Host: " << name << ", on port " << port << ", address: " << peer_ip ;

	auto host = std::make_shared<Peer>(port, peer_ip, io_ctx, ssl_ctx, name);

    tcp::acceptor acceptor(io_ctx, tcp::endpoint(ip::make_address(peer_ip), port));
   
    cout << "Hi " << name << "! Waiting for peer on port " << port << "...\n";


    // Accepts connection from client, returns error message if unsuccessful
    cout << "Running acceptor..." << endl;
    acceptor.async_accept(host->ssl_sock().lowest_layer(), [&host](const boost::system::error_code& error) {
        try {
            if (error) {
                std::cerr << "Connection failed: " << error.message() << endl;
                //If handshake doesn't work, error message shows on screen
            }
            else {

                cout << "Peer connected! Start chatting...\n";

            }
        }
        catch (std::exception e) {
            cout << e.what() << endl;
        }
        });


    cout << "Attempting handshake...\n";

}

void connectToSender(int& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {
	//cout << "Client: " << name << ", on port " << port << ", address: " << peer_ip;
	
	auto client = std::make_shared<Peer>(port, peer_ip, io_ctx, ssl_ctx, name);

    cout << "Connecting...\n";

    // Connects to the server, returns an error message if unsuccessful
    try {
        client->ssl_sock().lowest_layer().connect(tcp::endpoint(ip::make_address(peer_ip), port));
        cout << "Connected! \n";

    }
    catch (const std::exception& error) {
        std::cerr << "Connect failed: " << error.what() << endl;

    }


    cout << "Attempting handshake...\n";

	
}