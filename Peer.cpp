#include "Peer.h"
#include "allheader.h"
#include "sslconfig.h"
#include <mutex>

std::mutex cout_mutex;  // Mutex for thread-safe output

// Constructor initializes port and socket
Peer::Peer(string port, io_context& io_ctx, ssl::context& ssl_ctx) : port(port), ssl_ctx(ssl::context::tlsv12), ssl_sock(io_ctx, ssl_ctx), socket(io_ctx) {}
    


// This sets up the P2P connection before running readMessage and sendMessage simultaneously
void Peer::startConnection(const string& host_ip) {  // Host function
    ssl_ctx.set_default_verify_paths(); // Lines 15 and 16 refer to the SSL context and certificates that I tried to implement. App will run with/without these lines.
    runSSL(ssl_ctx);

    tcp::acceptor acceptor(io_ctx, tcp::endpoint(tcp::v4(), std::stoi(port))); // This sets up a listener for the client peer. 

    cout << "Hello! Enter your name: ";
    getline(std::cin, name);
    cout << "Hi " << name << "! Waiting for peer on port " << port << "...\n";

    acceptor.accept(socket); // Accepts connection from client
    cout << "Peer connected! Performing handshake...\n";

    boost::asio::write(socket, boost::asio::buffer("HANDSHAKE\0")); //Writes the word "HANDSHAKE" to the socket

    char handshake_response[11];
    boost::asio::read(socket, boost::asio::buffer(handshake_response, 11)); // Reads what's on the socket, specifying that it will be 10 characters in length ("HANDSHAKE")

    if (string(handshake_response) == "HANDSHAKE\0") { // If the data on the socket reads as "HANDSHAKE", the handshake is completed and the peers can talk.
        std::cout << "Handshake successful! Start chatting...\n";
    }
    else if (!handshake_response) {
        cout << "Handshake unsuccessful! Aborting..." << endl;
    }

    std::thread readThread(&Peer::readMessage, this);  // Thread for reading messages
    std::thread sendThread(&Peer::sendMessage, this);  // Thread for sending messages

    readThread.join();
    sendThread.join();
}

// This connects to an existing P2P chat as a client
void Peer::connectToSender(const std::string& host_ip) {  // Client function
    ssl_ctx.set_default_verify_paths();
    runSSL(ssl_ctx);

    cout << "Enter your name: ";
    getline(std::cin, name);

    auto endpoints = resolver.resolve(host_ip, port);
    connect(socket, endpoints); // Connect to the host
    cout << "Connected to host at " << host_ip << " on port " << port << ". Performing handshake...\n";

    char handshake[11];
    boost::asio::read(socket, boost::asio::buffer(handshake, 11));

    if (string(handshake) == "HANDSHAKE\0") {
        cout << "Handshake received from host. Sending confirmation...\n";
        boost::asio::write(socket, boost::asio::buffer("HANDSHAKE\0"));
        cout << "Handshake successful! Start chatting...\n";
    }

    std::thread readThread(&Peer::readMessage, this);  // Thread for reading messages
    std::thread sendThread(&Peer::sendMessage, this);  // Thread for sending messages

    readThread.join();
    sendThread.join();
}

// Handles sending messages with user's name as a prefix
void Peer::sendMessage() {
    try {
        while (true) {
            std::string message;
            std::getline(std::cin, message);

            if (message.empty()) continue;

            std::string formatted_message = name + ": " + message + "\n";
            boost::system::error_code ec;

            // Check socket status before sending
            if (socket.is_open()) {
                boost::asio::write(socket, boost::asio::buffer(formatted_message), ec);
            }

            if (ec) {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cerr << "Send error: " << ec.message() << std::endl;
                break;
            }
        }
    }
    catch (const std::exception& e) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cerr << "Send exception: " << e.what() << std::endl;
    }
}

// Handles receiving messages from the peer
void Peer::readMessage() {
    try {
        while (true) {
            boost::asio::streambuf buffer;
            boost::system::error_code ec;

            // Check socket status before reading
            if (socket.is_open()) {
                boost::asio::read_until(socket, buffer, "\n", ec);
            }

            if (ec) {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cerr << "Read error: " << ec.message() << std::endl;
                break;
            }

            std::istream stream(&buffer);
            std::string message;
            std::getline(stream, message);

            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << message << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cerr << "Read exception: " << e.what() << std::endl;
    }
}
