#pragma once
#include "allheader.h"

// Peer class handles the functionality of the host and client for P2P communication
class Peer : public std::enable_shared_from_this<Peer> {
public:
    Peer(string port,io_context& io_ctx, ssl::context& ssl_ctx);

    void startConnection(const string& host_ip);  // Host
    void connectToSender(const string& host_ip);  // Client
    void sendMessage();
    void readMessage();

private:
    string port; // Port number for connection
    string name; // User's handle or name
    io_context io_ctx; // Provides input/output functionality
    tcp::socket socket{ io_ctx };
    tcp::resolver resolver{io_ctx};
    streambuf buffer;
    ssl::context ssl_ctx;
    ssl::stream<ip::tcp::socket>ssl_sock{ io_ctx, ssl_ctx };

};
