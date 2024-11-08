#include "Peer.h"
#include <iostream>

// Main function allows user to choose between hosting and joining a chat
int main() {
    std::string port;
    io_context io_ctx;
    ssl::context ssl_ctx( ssl::context::tlsv12 );
    std::cout << "Enter port number: ";
    std::cin >> port;
    std::cin.ignore();

    int choice;
    std::cout << "Welcome to Chataway! Select:\n1. Host chat\n2. Join chat\n";
    std::cin >> choice;
    std::cin.ignore();

    Peer peer(port, io_ctx, ssl_ctx);

    if (choice == 1) {
        peer.startConnection(); // Start as host
    }
    else if (choice == 2) {
        std::string host_ip;
        std::cout << "Enter host IP address: ";
        std::cin >> host_ip;
        std::cin.ignore();
        peer.connectToSender(host_ip); // Connect to host as client
    }
    else {
        std::cerr << "Invalid choice. Exiting.\n";
    }

    return 0;
}