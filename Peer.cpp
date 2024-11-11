#include "Peer.h"
#include "allheader.h"
#include <mutex>

std::mutex cout_mutex;  // Mutex for thread-safe output

// Constructor initializes port and socket
Peer::Peer(string port, io_context& io_ctx, ssl::context& ssl_ctx) : port(port), ssl_ctx(ssl::context::tlsv12), ssl_sock(io_ctx, ssl_ctx) {}
    


// This sets up the P2P connection before running readMessage and sendMessage simultaneously
void Peer::startConnection(const string& host_ip) {  // Host function
    ssl_ctx.set_default_verify_paths();
    io_context io_ctx;
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


    tcp::acceptor acceptor(io_ctx, tcp::endpoint(ip::make_address(host_ip), stoi(port)));
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
    std::this_thread::sleep_for(std::chrono::seconds(15));
    cout << "Connected!" << endl;
    while (true) {
        try {
            string message;
            readMessage();
            getline(cin, message);
            if (message == "quit") {
                break;
            }
            else if (!message.empty()) {
                sendMessage(message);
            }
        }
        catch (const std::exception& e) {
            cout << "Exception in host: " << e.what() << endl;
        }
    }
    
    //std::thread readThread(&Peer::readMessage, this);  // Thread for reading messages
    //std::thread sendThread(&Peer::sendMessage, this);  // Thread for sending messages   
    //readThread.join();
    //sendThread.join();
    }

    


// This connects to an existing P2P chat as a client
void Peer::connectToSender(const string& host_ip) {  // Client function
    ssl_ctx.set_default_verify_paths();
    io_context io_ctx;

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
    auto client_peer = std::make_shared<Peer>(port, io_ctx, ssl_ctx);


    cout << "Enter your name: ";
    getline(std::cin, name);

    cout << "Resolving...\n";
    int stoiport = stoi(port);

    cout << "Connecting...\n";

    // Connects to the server, returns an error message if unsuccessful
    try {
        client_peer->ssl_sock.lowest_layer().connect(tcp::endpoint(ip::make_address(host_ip), stoiport));
        cout << "Connected! \n";

    }
    catch (const std::exception& error) {
        std::cerr << "Connect failed: " << error.what() << endl;

    }


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

    std::thread io_thread([&io_ctx]() {
        try {
            // Run the IO context
            io_ctx.run();
        }
        catch (const std::exception& e) {
            cout << "Exception in IO Host thread: " << e.what() << endl;
        }
        });
    cout << "Connecting to host..." << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "Connected!" << endl;
    while (true) {
        try {
            string message;
            readMessage();
            getline(cin, message);
            if (message == "quit") {
                break;
            }
            else if (!message.empty()) {
                sendMessage(message);
            }
        }
        catch (const std::exception& e) {
            cout << "Exception in host: " << e.what() << endl;
        }
    }

    
    //std::thread readThread(&Peer::readMessage, this);  // Thread for reading messages
    //std::thread sendThread(&Peer::sendMessage, this);  // Thread for sending messages
    //readThread.join();
    //sendThread.join();

  }

// Handles sending messages with user's name as a prefix
void Peer::sendMessage(string message) {
    try {
       
        std::lock_guard<std::mutex> lock(cout_mutex);
        message = name + ": " + message + "\n";  // Format message with username
        boost::asio::async_write(ssl_sock, boost::asio::buffer(message));
        
        
    } catch (const std::exception& error) {
        std::cerr << "Write failed: " << error.what() << endl;

    }
}

// Handles receiving messages from the peer
void Peer::readMessage() {
    try {
        
        boost::asio::streambuf buffer;
        boost::asio::async_read_until(ssl_sock, buffer, "\n");
        std::istream stream(&buffer);
        std::string message;
        std::getline(stream, message);

        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << message << std::endl; // Print received message
        
    }
    catch (const std::exception& error) {
        std::cerr << "Read failed: " << error.what() << endl;

    }
}
