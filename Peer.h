#pragma once
#include "allheader.h"

// Peer class handles the functionality of the host and client for P2P communication
class Peer : public std::enable_shared_from_this<Peer> {
public:
    Peer(int& port, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx,string& name);

    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_sock();

    //void startConnection(string& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx);  // Host
    //void connectToSender(string& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx);  // Client
    void sendMessage();
    void readMessage();
    

private:
    int port; // Port number for connection
    string name; // User's handle or name
    streambuf buffer;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;
};