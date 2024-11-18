#include "Connections.h"
#include "Peer.h"
#include "Cosmetics.h"

void startConnection(int& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {

	auto host = std::make_shared<Peer>(port, peer_ip, io_ctx, ssl_ctx, name);

    tcp::acceptor acceptor(io_ctx, tcp::endpoint(ip::make_address(peer_ip), port));
   
    cout << "Waiting for peer on port " << port << "...\n";

    // Accepts connection from client, returns error message if unsuccessful
    acceptor.async_accept(host->ssl_sock().lowest_layer(), [&host](const boost::system::error_code& error) {
        try {
            if (error) {
                std::cerr << "Connection failed: " << error.message() << endl;

            }
            else {
                host->handleHandshake(boost::asio::ssl::stream_base::handshake_type::server);
                cout << "Peer connected! Chat away!\n";

            }
        }
        catch (std::exception e) {
            cout << "Acceptor error: " << e.what() << endl;
        }
        });


    std::thread io_thread([&io_ctx]() {
        try {
            // Run the IO context
            io_ctx.run();
        }
        catch (const std::exception& e) {
            cout << "Exception in IO Host thread: " << e.what() << endl;
        }
        });
    cout << "Waiting for peer..." << endl;

    while (true) {
        try {
            string message;
            // Read the message from the user
            getline(cin, message);
            // Exit the chat if the user types 'LEAVE_CHAT'
            if (message == "LEAVE_CHAT") {
                cout << "Thank you for using Chataway!\n\nExiting...";
                io_ctx.stop();
                io_thread.join();
                break;
            }
            // Send the message if it is not empty
            if (!message.empty()) {
                host->sendMessage(message);
            };

        }
        catch (const std::exception& e) {
            cout << "Error sending message: " << e.what() << endl;
        }
    }

}

void connectToSender(int& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {
	auto client = std::make_shared<Peer>(port, peer_ip, io_ctx, ssl_ctx, name);

    cout << "Connecting...\n";

    // Connects to the server, returns an error message if unsuccessful
    try {
        client->ssl_sock().lowest_layer().connect(tcp::endpoint(ip::make_address(peer_ip), port));
        client->handleHandshake(boost::asio::ssl::stream_base::handshake_type::client);
        cout << "Connected to host! Chat away! \n";

    }
    catch (const std::exception& error) {
        std::cerr << "Connect failed: " << error.what() << endl;

    }

    std::thread io_thread([&io_ctx]() {
        try {
            // Run the IO context
            io_ctx.run();
        }
        catch (const std::exception& e) {
            cout << "Exception in IO Client thread: " << e.what() << endl;
        }
        });
  
    while (true) {
        try {
            string message;
            // Read the message from the user
            getline(cin, message);
            // Exit the chat if the user types 'LEAVE_CHAT'
            if (message == "LEAVE_CHAT") {
                cout << "Thank you for using Chataway!\n\nExiting...";
                io_ctx.stop();
                io_thread.join();
                break;
            }
            // Send the message if it is not empty
            if (!message.empty()) {
                client->sendMessage(message);
            };
            
        }
        catch (const std::exception& e) {
            cout << "Error sending message: " << e.what() << endl;
        }
    }
}