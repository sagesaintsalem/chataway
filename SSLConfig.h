#pragma once
#include "allheader.h"

// Function to configure SSL context for secure communication
void runSSL(boost::asio::ssl::context& ssl_ctx) {
    try {
        // Set SSL options for the context:
        ssl_ctx.set_options(boost::asio::ssl::context::default_workarounds |// Enable common workaround options for compatibility with some SSL implementations.
            boost::asio::ssl::context::tlsv13 | // Use TLS version 1.3 as the default protocol
            boost::asio::ssl::context::tlsv12); // Use version 1.2 if 1.3 is unavailable

        // Set the server's certificate chain (it should contain the full certificate chain including the root certificate).
        ssl_ctx.use_certificate_chain_file("domain.crt");
        // Set the private key file (the server's private key). The private key is used to establish SSL handshakes.
        ssl_ctx.use_private_key_file("domain.pem", ssl::context::pem);
        // Load the trusted root CA certificate file to verify peer certificates.
        ssl_ctx.load_verify_file("rootCA.crt");

        // Set the verification mode for SSL connections:
        ssl_ctx.set_verify_mode(ssl::verify_peer | boost::asio::ssl::verify_fail_if_no_peer_cert); // This will require the peer (client or server) to present a valid certificate or it will fail.

    }
    catch (std::exception& sslerror) {
        // If any exception occurs during the setup (e.g., file loading errors), catch it and display an error message.
        std::cerr << "Error loading certificates: " << sslerror.what() << endl;
    }
}
