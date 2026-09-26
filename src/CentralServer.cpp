#include "CentralServer.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <cstring>
#include <string>

CentralServer::CentralServer(int port) : m_port(port) {}

CentralServer::~CentralServer() {
    if (m_serverFd >= 0) {
        close(m_serverFd);
    }
}

bool CentralServer::init() {
    m_serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverFd < 0) return false;

    int opt = 1;
    setsockopt(m_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(m_port);

    if (bind(m_serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) return false;
    if (listen(m_serverFd, 5) < 0) return false;

    return true;
}

void CentralServer::handleClient(int clientSocket) {
    char buffer[1024] = {0};
    std::cout << "🔌 Remote Edge Device checked in. Tracking telemetry...\n";

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytesRead <= 0) {
            std::cout << "❌ Edge Device disconnected from socket.\n";
            break;
        }

        std::cout << "📥 Inbound Data: " << buffer;
    }
    close(clientSocket);
}

void CentralServer::run() {
    std::cout << "🖥️ Central IoT Aggregator Gateway listening on port " << m_port << "...\n";

    while (true) {
        sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        int clientSocket = accept(m_serverFd, (struct sockaddr*)&clientAddr, &addrLen);
        
        if (clientSocket >= 0) {
            std::thread(handleClient, clientSocket).detach();
        }
    }
}

int main() {
    CentralServer server(8080);
    if (!server.init()) {
        std::cerr << "🛑 Error: Failed to initialize socket server bindings.\n";
        return 1;
    }
    server.run();
    return 0;
}
