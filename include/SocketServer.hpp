#ifndef SOCKET_SERVER_HPP
#define SOCKET_SERVER_HPP

#include "EdgeDevice.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <cstring>

class SocketServer {
private:
    int m_serverFd;
    int m_port;

public:
    SocketServer(int port) : m_port(port), m_serverFd(-1) {}
    
    bool init() {
        m_serverFd = socket(AF_INET, SOCK_STREAM, 0);
        if (m_serverFd < 0) return false;

        int opt = 1;
        setsockopt(m_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(m_port);

        if (bind(m_serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) return false;
        if (listen(m_serverFd, 3) < 0) return false;
        
        return true;
    }

    void run(EdgeDevice& device) {
        std::cout << "🚀 IoT Greenhouse Server running on port " << m_port << "...\n";
        std::cout << "👉 Connect using: nc localhost " << m_port << "\n\n";
        
        while (true) {
            sockaddr_in clientAddr;
            socklen_t addrLen = sizeof(clientAddr);
            int clientSocket = accept(m_serverFd, (struct sockaddr*)&clientAddr, &addrLen);
            
            if (clientSocket >= 0) {
                std::cout << "🔌 Client connected! Streaming data...\n";
                
                // Set a timeout on the socket so recv doesn't hang forever
                struct timeval tv;
                tv.tv_sec = 1;  // 1 second timeout
                tv.tv_usec = 0;
                setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

                std::thread([this, clientSocket, &device]() {
                    this->handleClient(clientSocket, device);
                }).detach();
            }
        }
    }

private:
    void handleClient(int clientSocket, EdgeDevice& device) {
        char buffer[1024] = {0};
        
        while (true) {
            // 1. Immediately read sensors and send telemetry data
            auto data = device.readSensors();
            std::string payload = "📊 TELEMETRY -> TEMP:" + std::to_string(data.temperature).substr(0, 5) + 
                                  "°C | HUM:" + std::to_string(data.humidity).substr(0, 5) + 
                                  "% | FAN:" + (data.fanStatus ? "ON" : "OFF") + "\n";
            
            if (send(clientSocket, payload.c_str(), payload.length(), 0) <= 0) {
                std::cout << "❌ Client disconnected.\n";
                break; 
            }

            // 2. Listen for incoming control commands from netcat (e.g., "FAN_ON\n")
            memset(buffer, 0, sizeof(buffer));
            int valread = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            
            if (valread > 0) {
                std::string command(buffer);
                // Clean up newline characters from the terminal input
                if (!command.empty() && command.back() == '\n') command.pop_back();
                if (!command.empty() && command.back() == '\r') command.pop_back();

                std::cout << "🎮 Received command: " << command << "\n";
                
                if (command == "FAN_ON" || command == "FAN_OFF") {
                    device.setActuator(command);
                    std::string confirm = "✅ Actuator updated: " + command + "\n";
                    send(clientSocket, confirm.c_str(), confirm.length(), 0);
                } else {
                    std::string errorMsg = "⚠️ Unknown command. Use: FAN_ON or FAN_OFF\n";
                    send(clientSocket, errorMsg.c_str(), errorMsg.length(), 0);
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        close(clientSocket);
    }
};

#endif
