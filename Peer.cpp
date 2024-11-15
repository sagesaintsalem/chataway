#include "Peer.h"
#include <mutex>

Peer::Peer(int& port, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx, string& name) : port(port), name(name), ssl_socket(io_ctx, ssl_ctx) {};

boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& Peer::ssl_sock() {
    return ssl_socket;
}

std::mutex cout_mutex;
//void Peer::startConnection(string& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {
//	cout << "Host: " << name << ", on port " << port << ", address: " << peer_ip ;
//}
//
//void Peer::connectToSender(string& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {
//	cout << "Client: " << name << ", on port " << port << ", address: " << peer_ip;
//	
//	
//}

void Peer::readMessage() {

}

void Peer::sendMessage() {
    
}