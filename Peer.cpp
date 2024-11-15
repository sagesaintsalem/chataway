#include "Peer.h"
#include <mutex>

Peer::Peer(int& port, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx, string& name) : port(port), name(name), ssl_socket(io_ctx, ssl_ctx) {};

boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& Peer::ssl_sock() {
    return ssl_socket;
}

std::mutex cout_mutex;
//void Peer::startConnection(string& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {
//	cout << "Host: " << name << ", on port " << port << ", address: " << peer_ip ;
//}
//
//void Peer::connectToSender(string& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx) {
//	cout << "Client: " << name << ", on port " << port << ", address: " << peer_ip;
//	
//	
//}

void Peer::readMessage() {
    try {

        boost::asio::streambuf buffer;
        boost::asio::async_read_until(ssl_socket, buffer, "\n");
        std::istream stream(&buffer);
        std::string message;
        std::getline(stream, message);

        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << message << std::endl; // Print received message

    }
    catch (const std::exception& error) {
        std::cerr << "Read failed: " << error.what() << endl;

    }
}

void Peer::sendMessage() {
    try {
        string message;
        std::lock_guard<std::mutex> lock(cout_mutex);
        message = name + ": " + message + "\n";  // Format message with username
        boost::asio::async_write(ssl_socket, boost::asio::buffer(message));


    }
    catch (const std::exception& error) {
        std::cerr << "Write failed: " << error.what() << endl;

    }
}