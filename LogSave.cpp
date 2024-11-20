#include "LogSave.h"

void saveToLog(string fullMessage) {
    // Create and open file "chatlog.txt" in append mode to keep adding new messages
    std::ofstream myfile("chatlog.txt", std::ios::app);
    if (!myfile.is_open()) {
        throw std::runtime_error("Failed to open chatlog.txt"); //Error message if file doesn't open
    }

    // Write the message to the file
    myfile << fullMessage;

    // Close the file
    myfile.close();
}