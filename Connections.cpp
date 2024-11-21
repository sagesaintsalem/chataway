#include "Connections.h"
#include "PeersOnline.h"

// This function initiates the SSL handshake with the given handshake type (either client or server).
void handleHandshake(std::shared_ptr<Peer> peer, boost::asio::ssl::stream_base::handshake_type htype) {
    // Perform the async handshake. When the handshake is complete, the lambda function is called - peer calls readMessage().
    peer->ssl_sock().async_handshake(htype, [peer](const boost::system::error_code error) {
        try {
            if (!error) {  // If no error occurred during handshake
                peer->readMessage();  // Start reading messages after successful handshake
            }
            else {  // If handshake failed, print the error message
                cout << "Handshake failed :(  Error: " << error.message() << endl;
            }
        }
        catch (std::exception e) {  // Catch any exceptions that might occur and display the error message
            cout << e.what() << endl;
        }
        });
}

// Function to start a connection as a server
void startConnection(int& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {

    // Create a shared pointer to a Peer object, passing the port, IP, SSL context, and name for initialization
    auto host = std::make_shared<Peer>(io_ctx, ssl_ctx, name);

    // Create a TCP acceptor that listens for incoming connections on the specified IP and port
    tcp::acceptor acceptor(io_ctx, tcp::endpoint(ip::make_address(peer_ip), port));

    // Output message indicating that the server is waiting for a peer to connect
    cout << "Waiting for peer on port " << port << "...\n";

    // Start accepting a connection asynchronously. If a connection is accepted, it triggers handleHandshake().
    acceptor.async_accept(host->ssl_sock().lowest_layer(), [&host](const boost::system::error_code& error) {
        try {
            // If an error occurred during the connection attempt, print the error message
            if (error) {
                std::cerr << "Connection failed: " << error.message() << endl;
            }
            else {
                // If connection succeeded, initiate SSL handshake as the server
                handleHandshake(host, boost::asio::ssl::stream_base::handshake_type::server);
                cout << "Peer connected! Chat away!\n";  // Confirm successful peer connection
            }
        }
        catch (std::exception e) {  // Catch any exceptions during the accept operation
            cout << "Acceptor error: " << e.what() << endl;
        }
        });

    // Start a separate thread to run the IO context. This is required for asynchronous operations.
    std::thread io_thread([&io_ctx]() {
        try {
            // Run the IO context, which processes asynchronous events like readMessage and sendMessage
            io_ctx.run();
        }
        catch (const std::exception& e) {
            // Catch and display any exceptions that occur during IO processing
            cout << "Exception in IO Host thread: " << e.what() << endl;
        }
        });

    cout << "Waiting for peer..." << endl;  // Indicate the program is waiting for user input

    // Main message loop, continuously read and send messages until the user exits
    while (true) {
        try {
            string message;
            // Read the message typed by the user
            getline(cin, message);

            // If the user types 'LEAVE_CHAT', stop io_ctx and io_thread then close the application.
            if (message == "LEAVE_CHAT") {
                cout << "Thank you for using Chataway!\n\nExiting...";
                io_ctx.stop();  // Stop the IO context to halt asynchronous operations
                io_thread.join();  // Wait for the IO thread to finish
                clearPeer();
                break;  // End application
            }

            // If the message is not empty, send it to the peer
            if (!message.empty()) {
                host->sendMessage(message);
                cout << "\n";  // Print an empty line for formatting
            }
        }
        catch (const std::exception& e) {
            // If an error occurs while sending a message, display the error message
            cout << "Error sending message: " << e.what() << endl;
        }
    }
}

// Function to connect to a server as a client
void connectToSender(int& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {
    // Create a shared pointer to a Peer object, passing the port, IP, SSL context, and name for initialization
    auto client = std::make_shared<Peer>(io_ctx, ssl_ctx, name);

    cout << "Connecting...\n";  // Output message indicating that the client is trying to connect

    // Try to connect to the server. If successful, initiate the SSL handshake as a client
    try {
        // Connect the SSL socket's underlying TCP layer to the server, specified by IP and port
        client->ssl_sock().lowest_layer().connect(tcp::endpoint(ip::make_address(peer_ip), port));
        // After successful connection, initiate the SSL handshake as a client
        handleHandshake(client, boost::asio::ssl::stream_base::handshake_type::client);
        cout << "Connected to host! Chat away! \n";  // Output message confirming the connection
    }
    catch (const std::exception& error) {
        // If connection fails, catch the exception and print the error message
        std::cerr << "Connect failed: " << error.what() << endl;
    }

    // Start a separate thread to run the IO context. This is required for asynchronous operations.
    std::thread io_thread([&io_ctx]() {
        try {
            // Run the IO context, which processes asynchronous events
            io_ctx.run();
        }
        catch (const std::exception& e) {
            // Catch and display any exceptions that occur during IO processing
            cout << "Exception in IO Client thread: " << e.what() << endl;
        }
        });

    // Main message loop, continuously read and send messages until the user exits
    while (true) {
        try {
            string message;
            // Read the message typed by the user
            getline(cin, message);

            // If the user types 'LEAVE_CHAT', stop io_ctx and io_thread then close the application.
            if (message == "LEAVE_CHAT") {
                cout << "Thank you for using Chataway!\n\nExiting...";
                io_ctx.stop();  // Stop the IO context to halt asynchronous operations
                io_thread.join();  // Wait for the IO thread to finish
                clearPeer();
                break;  // End application
            }

            // If the message is not empty, send it to the server
            if (!message.empty()) {
                client->sendMessage(message);
                cout << "\n";  // Print an empty line for formatting
            }
        }
        catch (const std::exception& e) {
            // If an error occurs while sending a message, display the error message
            cout << "Error sending message: " << e.what() << endl;
        }
    }
}