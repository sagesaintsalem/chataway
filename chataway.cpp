#include "Peer.h"
#include "SSLConfig.h"
#include "Connections.h"

void createPeer(string name, string peer_ip, int port, int hostOrClient) {
	io_context io_ctx;
	ssl::context ssl_ctx(ssl::context::tlsv12);
    runSSL(ssl_ctx);
	if (hostOrClient == 1) {
		//Peer host(port, peer_ip, io_ctx, ssl_ctx, name);
		startConnection(port, name, peer_ip, io_ctx, ssl_ctx);
	}
	else if (hostOrClient == 2) {
		//Peer client(port, peer_ip, io_ctx, ssl_ctx, name);
		connectToSender(port, name, peer_ip, io_ctx, ssl_ctx);
	}
}

int main() {
    int hostOrClient;
    cout << "Welcome to Chataway! Select:\n1. Host chat\n2. Join chat\n";
    cin >> hostOrClient;
    cin.ignore();

    /*int port;
    cout << "Enter port number: ";
    cin >> port;
    cin.ignore();

    string peer_ip;
    cout << "Enter host IP address: ";
    cin >> peer_ip;
    cin.ignore();*/

    int port = 1337;
    string peer_ip = "127.0.0.1";

    cout << "Enter port number: " << port << endl;
    cout << "Enter IP address: " << peer_ip << endl;

    string name;
    cout << "Please enter your name: ";
    cin >> name;
    cin.ignore();

    if (hostOrClient == 1 || hostOrClient == 2) {
        cout << hostOrClient << " selected" << endl;
        createPeer(name, peer_ip, port, hostOrClient);
    }
    else if (hostOrClient > 2) {
        std::cerr << "Invalid choice. Exiting.\n";
      
    }
    else {
        std::cerr << "What was that??? Gurl, bye.";
    }

    return 0;
}