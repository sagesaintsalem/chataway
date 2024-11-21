#pragma once
#include "allheader.h"

// The Peer class represents a peer in a network communication system, inheriting from std::enable_shared_from_this<Peer> to allow shared ownership of Peer objects.
class Peer : public std::enable_shared_from_this<Peer> {
public:
    // Constructor for Peer class. It initializes a Peer object with port, IP, IO context, SSL context, and name.
    Peer(int& port, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx, string& name);

    // This method returns a reference to the SSL stream (which wraps a TCP socket) for secure communication.
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& ssl_sock();

    // This method sends a message to the peer. The message is passed by reference.
    void sendMessage(string& message);

    // This method is responsible for reading incoming messages.
    void readMessage();

private:
    int port;  // Port number to use for the network connection.
    string name;  // Name or handle of the peer.
    streambuf buffer;  // Buffer used for reading and writing data to/from the network.
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;  // SSL stream (wraps the TCP socket) for secure communication.
};
