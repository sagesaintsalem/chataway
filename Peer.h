#pragma once
#include "allheader.h"

// Peer class handles the functionality of the host and client for P2P communication
class Peer : public std::enable_shared_from_this<Peer> {
public:
    Peer(string port, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx,string& name);

    //void startConnection(string& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx);  // Host
    //void connectToSender(string& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx);  // Client
    /*void sendMessage();
    void readMessage();*/
    

private:
    string port; // Port number for connection
    string name; // User's handle or name
    streambuf buffer;
    ssl::stream<ip::tcp::socket>ssl_sock;

};