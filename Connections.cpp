#include "Connections.h"

void startConnection(string& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {
	cout << "Host: " << name << ", on port " << port << ", address: " << peer_ip ;
}

void connectToSender(string& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {
	cout << "Client: " << name << ", on port " << port << ", address: " << peer_ip;
	
	
}