#include "Peer.h"
#include <mutex>

Peer::Peer(string port, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx, string& name) : port(port), name(name), ssl_sock(io_ctx, ssl_ctx) {};

void Peer::startConnection(string& port, string& name, string& peer_ip) {
	cout << "Host: " << name << ", on port " << port << ", address: " << peer_ip ;
}

void Peer::connectToSender(string& port, string& name, string& peer_ip) {
	cout << "Client: " << name << ", on port " << port << ", address: " << peer_ip;

}

//void Peer::readMessage() {
//
//}
//
//void Peer::sendMessage() {
//
//}