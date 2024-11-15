#pragma once
#include "allheader.h"
void startConnection(int& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx);  // Host
void connectToSender(int& port, string& name, string& peer_ip, io_context& io_ctx, ssl::context& ssl_ctx);  // Client