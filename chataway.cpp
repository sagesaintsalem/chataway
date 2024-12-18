#include "Peer.h"
#include "SSLConfig.h"
#include "Connections.h"
#include "PeersOnline.h"


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

    int port;
    cout << "Enter port number: ";
    cin >> port;
    cin.ignore();

    string peer_ip;
    cout << "Enter host IP address: ";
    cin >> peer_ip;
    cin.ignore();

    string name;  // Declare a string variable to hold the user's name
    cout << "Please enter your name: ";  // Prompt the user to enter their name
    cin >> name;  // Read the user's input for their name
    cin.ignore();  // Ignore any leftover newline characters in the input buffer (to avoid issues with further input)

    loadPeerCount();
    if (connected_peers > 1){
        cout << "Hi " << name << "! There are " << connected_peers << " peers currently online!\n";
    }
    else if (connected_peers == 1) {
        cout << "Hi " << name << "! There is " << connected_peers << " peer currently online!\n";
    }
    else {
        cout << "Hi " << name << "! Nobody else online yet..." << "\n";
    }

    // Declare an integer to store the user's choice (host or client)
    int hostOrClient;
    // Prompt the user to choose between hosting a chat or joining an existing chat
    cout<<"Please select : \n1.Host chat\n2.Join chat\n";
    
    cin >> hostOrClient;  // Read the user's choice (1 for host, 2 for client)
    cin.ignore();  // Again, ignore any leftover newline characters in the input buffer

    // Check if the user's choice is invalid (anything other than 1 or 2)
    if (hostOrClient > 2) {
        std::cerr << "Invalid choice. Exiting.\n";  // Print an error message
        exit(EXIT_SUCCESS);  // Exit the program with a success status (exit with an error condition)
    }
    // If the choice is valid (1 or 2), proceed to create a Peer object and start the connection
    else if (hostOrClient == 1 || hostOrClient == 2) {
        addPeer();
        createPeer(name, peer_ip, port, hostOrClient);  // Call the createPeer function with the given parameters
    }
    // If the choice is not valid (anything other than 1 or 2), print an error message
    else {
        std::cerr << "Choice invalid. Exiting.";  // Print an error message
        exit(EXIT_SUCCESS);  // Exit the program with a success status (indicating invalid choice)
    }

    return 0;
}