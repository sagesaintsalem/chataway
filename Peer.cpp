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
        boost::asio::async_read_until(ssl_socket, buffer, "\n", [self = shared_from_this()](boost::system::error_code error, std::size_t len) {
            if (!error) {
                std::istream stream(&self->buffer);
                std::string message;
                std::getline(stream, message);
                std::cout << message << endl;
                self->buffer.consume(len);
                self->readMessage();
            }
            else if (error.message() == "An existing connection was forcibly closed by the remote host") {
                cout << "Connection closed by peer! Thank you for using Chataway!\n Exiting...";
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
        auto send_msg = std::make_shared<string>(name + ": " + message + "\n");  // Format message with username
        boost::asio::async_write(ssl_socket, boost::asio::buffer(*send_msg), [send_msg, this](boost::system::error_code error, std::size_t) {
            if (error) {
                cout << "Halp! Message not sended!" << error.message() << endl;
            }
            });


    }
    catch (const std::exception& error) {
        std::cerr << "Write failed: " << error.what() << endl;

    }
}