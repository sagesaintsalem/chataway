#include "Peer.h"
#include "SSLConfig.h"
#include "Connections.h"




void createPeer(string name, string peer_ip, int port, int hostOrClient) {
	io_context io_ctx;
	ssl::context ssl_ctx(ssl::context::tlsv12);
    runSSL(ssl_ctx);
	if (hostOrClient == 1) {
        cout << "You have chosen to host a chat!" << endl;
		startConnection(port, name, peer_ip, io_ctx, ssl_ctx);
	}
	else if (hostOrClient == 2) {
        cout << "You have chosen to join a chat!" << endl;
		connectToSender(port, name, peer_ip, io_ctx, ssl_ctx);
	}
}


int main() {

    cout << "--------------------\n\n";
    cout << "Welcome to Chataway!\n\n";
    cout << "--------------------\n\n";

    /*int port;
    cout << "Enter port number: ";
    cin >> port;
    cin.ignore();

    string peer_ip;
    cout << "Enter host IP address: ";
    cin >> peer_ip;
    cin.ignore();*/

    int port = 1337;  // Initialize the port number
    string peer_ip = "127.0.0.1";  // Set the IP address 

    // Display the current port number (1337)
    cout << "Enter port number: " << port << endl;
    // Display the current IP address (127.0.0.1)
    cout << "Enter IP address: " << peer_ip << endl;

    string name;  // Declare a string variable to hold the user's name
    cout << "Please enter your name: ";  // Prompt the user to enter their name
    cin >> name;  // Read the user's input for their name
    cin.ignore();  // Ignore any leftover newline characters in the input buffer (to avoid issues with further input)

    // Declare an integer to store the user's choice (host or client)
    int hostOrClient;
    cout << "Hi " << name << "! Please select:\n1. Host chat\n2. Join chat\n";
    // Prompt the user to choose between hosting a chat or joining an existing chat
    cin >> hostOrClient;  // Read the user's choice (1 for host, 2 for client)
    cin.ignore();  // Again, ignore any leftover newline characters in the input buffer

    // Check if the user's choice is invalid (anything other than 1 or 2)
    if (hostOrClient > 2) {
        std::cerr << "Invalid choice. Exiting.\n";  // Print an error message
        exit(EXIT_SUCCESS);  // Exit the program with a success status (exit with an error condition)
    }
    // If the choice is valid (1 or 2), proceed to create a Peer object and start the connection
    else if (hostOrClient == 1 || hostOrClient == 2) {
        createPeer(name, peer_ip, port, hostOrClient);  // Call the createPeer function with the given parameters
    }
    // If the choice is not valid (anything other than 1 or 2), print an error message
    else {
        std::cerr << "Choice invalid. Exiting.";  // Print an error message
        exit(EXIT_SUCCESS);  // Exit the program with a success status (indicating invalid choice)
    }


    return 0;
}