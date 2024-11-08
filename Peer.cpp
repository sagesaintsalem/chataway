#include "Peer.h"
#include "allheader.h"
#include <mutex>

std::mutex cout_mutex;  // Mutex for thread-safe output

// Constructor initializes port and socket
Peer::Peer(string port, io_context& io_ctx, ssl::context& ssl_ctx) : port(port), ssl_ctx(ssl::context::tlsv12), ssl_sock(io_ctx, ssl_ctx) {}
    


// This sets up the P2P connection before running readMessage and sendMessage simultaneously
void Peer::startConnection() {  // Host function
    ssl_ctx.set_default_verify_paths();

    // This loads up the relevant SSL files
    try {
        ssl_ctx.set_options(boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::tlsv12 |
            boost::asio::ssl::context::tlsv12);
        ssl_ctx.use_certificate_chain_file("tcpdomain.pem");  // Server certificate, omitted by .gitignore for security reasons.
        ssl_ctx.use_private_key_file("tcpkey.pem", ssl::context::pem);  // Server private key omitted by .gitignore for security reasons.
        ssl_ctx.load_verify_file(
            "tcpdomain.crt");
        ssl_ctx.set_verify_mode(ssl::verify_peer | boost::asio::ssl::verify_fail_if_no_peer_cert);  // No verification for simplicity
    }
    catch (std::exception& sslerror) {
        std::cerr << "Error loading certificates: " << sslerror.what() << endl;
    }

    auto host_peer = std::make_shared<Peer>(port, io_ctx, ssl_ctx);


    tcp::acceptor acceptor(io_ctx, tcp::endpoint(tcp::v4(), std::stoi(port)));
    cout << "Hello! Enter your name: ";
    getline(cin, name);
    cout << "Hi " << name << "! Waiting for peer on port " << port << "...\n";


    // Accepts connection from client, returns error message if unsuccessful
   
    acceptor.async_accept(host_peer->ssl_sock.lowest_layer(), [&host_peer](const boost::system::error_code& error) {
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
    


    //ssl_sock.handshake(ssl::stream_base::server); //This line causes a runtime error

    cout << "Attempting handshake...\n";
    // This starts the handshake from the server side
    try {
        ssl_sock.async_handshake(ssl::stream_base::server, [this](boost::system::error_code error) {
            try {
                if (!error) {
                    cout << "Hands shaken!\n";
                }
                else {
                    cout << "Handshake failed :(  Error: " << error.message() << endl;
                }
            }
            catch (std::exception e) {
                cout << e.what() << endl;
            }}
        );
    }
    catch (std::exception e) {
        cout << e.what() << endl;
    }
    cout << "Hand shaken\n\0";

    
    
    std::thread readThread(&Peer::readMessage, this);  // Thread for reading messages
    std::thread sendThread(&Peer::sendMessage, this);  // Thread for sending messages
        
    readThread.join();
    sendThread.join();
    }

    


// This connects to an existing P2P chat as a client
void Peer::connectToSender(const string& host_ip) {  // Client function
    ssl_ctx.set_default_verify_paths();

    // This loads up the relevant SSL files
    try {
        ssl_ctx.set_options(boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::tlsv12 |
            boost::asio::ssl::context::tlsv12);
        ssl_ctx.use_certificate_chain_file("tcpdomain.pem");  // Security certificate, omitted by .gitignore for security reasons.
        ssl_ctx.use_private_key_file("tcpkey.pem", ssl::context::pem);  // Private key omitted by .gitignore for security reasons.
        ssl_ctx.load_verify_file(
            "tcpdomain.crt");
        ssl_ctx.set_verify_mode(ssl::verify_peer | boost::asio::ssl::verify_fail_if_no_peer_cert);  // No verification for simplicity
    }
    catch (std::exception& sslerror) {
        std::cerr << "Error loading certificates: " << sslerror.what() << endl;
    }

    cout << "Enter your name: ";
    getline(std::cin, name);

    cout << "Resolving...\n";
    auto endpoints = resolver.resolve(host_ip, port);

    cout << "Connecting...\n";

    // Connects to the server, returns an error message if unsuccessful
    try {
        connect(ssl_sock.lowest_layer(), endpoints);
        cout << "Connected! \n";

    }
    catch (const std::exception& error) {
        std::cerr << "Connect failed: " << error.what() << endl;

    }

    //ssl_sock.handshake(ssl::stream_base::client); //This line causes a runtime error.

    cout << "Attempting handshake...\n";
    // This starts the handshake from the client side.
    try {
        ssl_sock.async_handshake(ssl::stream_base::client, [this](const boost::system::error_code& error) {
            try {
                if (!error) {
                    cout << "Hands shaken!\n";
                }
                else {
                    cout << "Handshake failed :(  Error: " << error.message() << endl;
                }
            }
            catch (std::exception e) {
                cout << e.what() << endl;
            }}
        );

    }
    catch (std::exception e) {
    cout << e.what() << endl;
}
    cout << "Hand shaken\n\0";

    
    std::thread readThread(&Peer::readMessage, this);  // Thread for reading messages
    std::thread sendThread(&Peer::sendMessage, this);  // Thread for sending messages

    readThread.join();
    sendThread.join();

  }

// Handles sending messages with user's name as a prefix
void Peer::sendMessage() {
    try {
        while (true) {
            string message;
            getline(std::cin, message);

            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                message = name + ": " + message + "\n";  // Format message with username
                auto msg = std::make_shared<string>(name + ": " + message);
                boost::asio::async_write(ssl_sock, boost::asio::buffer(*msg), [msg, this](boost::system::error_code err, size_t) {
                    if (err) {
                        cout << "Message not sent. Error: " << err.message() << endl;
                    }
                    });
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "Write failed: " << error.what() << endl;

    }
}

// Handles receiving messages from the peer
void Peer::readMessage() {
    try {
        boost::asio::async_read_until(ssl_sock, buffer, "\n", [this](boost::system::error_code err, size_t len) {
            cout << "Pointer pointed \n\0";
            if (!err) {
                while (true) {

                    std::istream stream(&this->buffer);
                    string message;
                    getline(stream, message);

                    this->buffer.consume(len);
                    cout << message << endl; // Print received message
                }
            }
            else  {
                cout << "Halp: " << err.message() << endl;
            }
         });
    }
    catch (const std::exception& error) {
        std::cerr << "Read failed: " << error.what() << endl;

    }
}
