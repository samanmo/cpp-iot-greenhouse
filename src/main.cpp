#include "SocketServer.hpp"
#include "EdgeDevice.hpp"
#include <iostream>

int main() {
    EdgeDevice greenhouse;
    SocketServer server(8080); // Server runs on port 8080

    if (!server.init()) {
        std::cerr << "🛑 Error: Failed to initialize socket server.\n";
        return 1;
    }

    server.run(greenhouse);
    return 0;
}
