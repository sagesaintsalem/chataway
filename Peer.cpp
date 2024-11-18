#include "Peer.h"
#include <mutex>

Peer::Peer(int& port, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx, string& name) : port(port), name(name), ssl_socket(io_ctx, ssl_ctx) {};

boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& Peer::ssl_sock() {
    return ssl_socket;
}


void Peer::handleHandshake(boost::asio::ssl::stream_base::handshake_type htype) {
        ssl_socket.async_handshake(htype, [self = shared_from_this()](const boost::system::error_code error) {
            try {
                if (!error) {
                    self->readMessage();
                }
                else {
                    cout << "Handshake failed :(  Error: " << error.message() << endl;
                }
            }
            catch (std::exception e) {
                cout << e.what() << endl;
            }
            }
        );     
    }
  



void Peer::readMessage() {
    try {
        boost::asio::async_read_until(ssl_socket, buffer, "\0", [self = shared_from_this()](boost::system::error_code error, std::size_t len) {
            if (!error) {
                std::istream stream(&self->buffer);
                std::string message;
                std::getline(stream, message);
                std::cout << message << endl;
                self->buffer.consume(len);
                self->readMessage(); 
            }
            else if (error.message() == "An existing connection was forcibly closed by the remote host") {
                cout << "Connection closed by peer! Thank you for using Chataway!\n\nExiting...";
                exit(EXIT_SUCCESS);
            }
            else {
                cout << "Readstream failed: " << error.message() << endl;
            }
            });
        

    }
    catch (const std::exception& error) {
        std::cerr << "Read failed: " << error.what() << endl;

    }
}

void Peer::sendMessage(const string& message) {
    try {
        time_t timestamp;
        char output[50];
        struct tm datetime;

        // Get the current time
        time(&timestamp);

        // Use localtime_s to get the local time in a thread-safe way
        localtime_s(&datetime, &timestamp);

        // Format the time as a string
        strftime(output, sizeof(output), "%D %R", &datetime);

        // Convert to a C++ string
        auto stringtime = std::string(output);
        
        //auto send_deets = std::make_shared<string>("||" + stringtime + "|| " + name + ": \n");  // Format message with username
        //

        //auto send_msg = std::make_shared<string>(message + "\n");
       
        string fullMessage = "||" + stringtime + "|| " + name + ": \n" + message + "\n\n\0";

        auto fullMsgP = std::make_shared<string>(fullMessage);
        boost::asio::async_write(ssl_socket, boost::asio::buffer(*fullMsgP), [fullMsgP, this](boost::system::error_code error, std::size_t) {
            if (error) {
                cout << "Halp! Message not sended!" << error.message() << endl;
            }
            });


    }
    catch (const std::exception& error) {
        std::cerr << "Write failed: " << error.what() << endl;

    }
}