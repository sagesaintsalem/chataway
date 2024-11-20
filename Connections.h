#pragma once
#include "allheader.h"
#include "Peer.h"
void startConnection(int& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx);  // Host connection
void connectToSender(int& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx);  // Client connection
void handleHandshake(std::shared_ptr<Peer> peer, boost::asio::ssl::stream_base::handshake_type htype); // SSL Handshake between instances to ensure secure communication