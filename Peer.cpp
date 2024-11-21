#include "Peer.h"
#include "LogSave.h"

// Constructor for the Peer class, initializing port, name and ssl_socket.
Peer::Peer(io_context& io_ctx, ssl::context& ssl_ctx, string& name) : name(name), ssl_socket(io_ctx, ssl_ctx) {};

// This function returns the ssl_socket (SSL stream), which is a wrapper around the TCP socket.
boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& Peer::ssl_sock() {
    return ssl_socket;  // Return the SSL socket object.
}
  
// This function reads a message from the network stream asynchronously.
void Peer::readMessage() {
    try {
        // Asynchronously read data from the SSL socket until a null byte '\0' is found.
        boost::asio::async_read_until(ssl_socket, buffer, "\0", [self = shared_from_this()](boost::system::error_code error, std::size_t len) {
            if (!error) {  // If no error occurred
                std::istream stream(&self->buffer);  // Create an input stream from the buffer
                std::string message;
                std::getline(stream, message);  // Read the message from the stream
                std::cout << message << "\n";  // Print the received message
                self->buffer.consume(len);  // Consume the bytes read from the buffer
                self->readMessage();  // Continue reading more data asynchronously
            }
            else if (error.message() == "An existing connection was forcibly closed by the remote host") { // This error showed when a peer force closed the chat
                // Handle the case where the connection was closed by the peer
                cout << "Connection closed by peer! Thank you for using Chataway!\n\nExiting...";
                exit(EXIT_SUCCESS);  // Exit the program gracefully
            }
            else {  // Handle other read errors
                cout << "Readstream failed: " << error.message() << endl;
            }
            });
    }
    catch (const std::exception& error) {  // Catch any exceptions that might occur during the read operation
        std::cerr << "Read failed: " << error.what() << endl;
    }
}

// This function is responsible for sending a message over the SSL socket.
void Peer::sendMessage(string& message) {
    try {
        time_t timestamp;  // Declare a variable to hold the timestamp for the message
        char output[50];  // Declare a buffer for the timestamp as a string
        struct tm datetime;  // Declare a tm struct (time structure) to hold the broken-down time info

        time(&timestamp);// Get the current time
        localtime_s(&datetime, &timestamp);// Use localtime_s to get the local time in a thread-safe way
        strftime(output, sizeof(output), "%D %R", &datetime);// Format the time as a string
        auto stringtime = std::string(output);// Convert to a C++ string

        // Create the full message
        string fullMessage = "||" + stringtime + "|| " + name + ": \n" + message + "\n\n\0";

        // Create a shared pointer 'fullMsgP' that holds a copy of the 'fullMessage' string.
        auto fullMsgP = std::make_shared<string>(fullMessage);

        // Async write operation sending data over a secured connection (ssl_socket).The message is sent as a buffer and 'fullMsgP' is passed in the lambda.
        boost::asio::async_write(ssl_socket, boost::asio::buffer(*fullMsgP),
            // Lambda function that will be executed once the write operation is complete with error code if needed.
            [fullMsgP, this](boost::system::error_code error, std::size_t len) {
            if (error) {
                cout << "Message not sent!" << error.message() << endl;
            }
            });

        // Save timestamped message to log using saveToLog from LogSave.cpp.
        saveToLog(fullMessage);
    }
    catch (const std::exception& error) {
        std::cerr << "Write failed: " << error.what() << std::endl;
    }
}