#include "PeersOnline.h"

int connected_peers = 0;

void loadPeerCount() {
    std::ifstream inputFile("peer_count.txt");
    if (inputFile.is_open()) {
        inputFile >> connected_peers;  // Read the current count from the file
        inputFile.close();
    }
    else {
        std::cout << "Failed to open file, starting with 0 connected peers.\n";
    }
}

void savePeerCount() {
    std::ofstream outputFile("peer_count.txt");
    if (outputFile.is_open()) {
        outputFile << connected_peers;  // Save the current count to the file
        outputFile.close();
    }
    else {
        std::cerr << "Failed to save peer count to file.\n";
    }
}

void addPeer() {
    ++connected_peers; // Increment the number of peers
    savePeerCount();  // Save the updated count to the file after adding a peer
}

void subtractPeer() {
    --connected_peers; // Decrease the number of peers
    savePeerCount();  // Save the updated count to the file after subtracting a peer   
}

void clearPeer() {
    connected_peers = 0;
    savePeerCount();
}