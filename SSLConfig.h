#pragma once
#include "allheader.h"

void runSSL(boost::asio::ssl::context& ssl_ctx) {
    try {
        ssl_ctx.set_options(boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::tlsv13 |
            boost::asio::ssl::context::tlsv12);
        ssl_ctx.use_certificate_chain_file("domain.crt");  // Server certificate, omitted by .gitignore for security reasons.
        ssl_ctx.use_private_key_file("domain.pem", ssl::context::pem);  // Server private key omitted by .gitignore for security reasons.
        ssl_ctx.load_verify_file(
            "rootCA.crt");
        ssl_ctx.set_verify_mode(ssl::verify_peer | boost::asio::ssl::verify_fail_if_no_peer_cert);  // Mode set to verify peer
    }
    catch (std::exception& sslerror) {
        std::cerr << "Error loading certificates: " << sslerror.what() << endl;
    }
}