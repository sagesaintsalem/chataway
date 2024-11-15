#include "Connections.h"
#include "Peer.h"

void startConnection(int& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {

	auto host = std::make_shared<Peer>(port, peer_ip, io_ctx, ssl_ctx, name);

    tcp::acceptor acceptor(io_ctx, tcp::endpoint(ip::make_address(peer_ip), port));
   
    cout << "Hi " << name << "! Waiting for peer on port " << port << "...\n";


    // Accepts connection from client, returns error message if unsuccessful
    cout << "Running acceptor..." << endl;
    acceptor.async_accept(host->ssl_sock().lowest_layer(), [&host](const boost::system::error_code& error) {
        try {
            if (error) {
                std::cerr << "Connection failed: " << error.message() << endl;

            }
            else {
                cout << "Attempting handshake...\n";
                host->handleHandshake(boost::asio::ssl::stream_base::handshake_type::server);
                cout << "Peer connected! Start chatting...\n";

            }
        }
        catch (std::exception e) {
            cout << "Acceptor error: " << e.what() << endl;
        }
        });


    cout << "Running context..." << endl;
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
    //std::this_thread::sleep_for(std::chrono::seconds(20));

    while (true) {
        try {
            string message;
            // Read the message from the user
            std::getline(cin, message);
            // Exit the chat if the user types 'exit'
            if (message == "exit") break;
            // Send the message if it is not empty
            if (!message.empty()) host->sendMessage(message);
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
        cout << "Connected! \n";
        cout << "Attempting handshake...\n";
        client->handleHandshake(boost::asio::ssl::stream_base::handshake_type::client);
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
    //std::this_thread::sleep_for(std::chrono::seconds(2));

    while (true) {
        try {
            string message;
            // Read the message from the user
            std::getline(cin, message);
            // Exit the chat if the user types 'exit'
            if (message == "exit") break;
            // Send the message if it is not empty
            if (!message.empty()) client->sendMessage(message);
        }
        catch (const std::exception& e) {
            cout << "Error sending message: " << e.what() << endl;
        }
    }
   

	
}

