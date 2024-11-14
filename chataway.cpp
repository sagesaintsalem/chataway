#include "Peer.h"
#include "SSLConfig.h"

void createPeer(string name, string peer_ip, string port, int hostOrClient) {
	io_context io_ctx;
	ssl::context ssl_ctx(ssl::context::tlsv12);
   /* runSSL(ssl_ctx);*/
	if (hostOrClient == 1) {
		Peer host(port, peer_ip, io_ctx, ssl_ctx, name);
		host.startConnection(port, name, peer_ip);
	}
	else if (hostOrClient == 2) {
		Peer client(port, peer_ip, io_ctx, ssl_ctx, name);
		client.connectToSender(port, name, peer_ip);
	}
}

int main() {
    int hostOrClient;
    std::cout << "Welcome to Chataway! Select:\n1. Host chat\n2. Join chat\n";
    std::cin >> hostOrClient;
    std::cin.ignore();

    std::string port;
    std::cout << "Enter port number: ";
    std::cin >> port;
    std::cin.ignore();

    std::string peer_ip;
    std::cout << "Enter host IP address: ";
    std::cin >> peer_ip;
    std::cin.ignore();

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