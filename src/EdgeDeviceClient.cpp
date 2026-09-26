#include "EdgeDeviceClient.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <cstdlib>

EdgeDevice::~EdgeDevice() {
    if (m_sockFd >= 0) {
        close(m_sockFd);
    }
}

GreenhouseData EdgeDevice::readSensors() {
    m_currentTemp += ((rand() % 10) - 5) * 0.1f; 
    if (m_fanStatus) m_currentTemp -= 0.3f; 
    return { m_currentTemp, static_cast<float>(55 + (rand() % 5)), m_fanStatus };
}

bool EdgeDevice::connectToServer(const std::string& ip, int port) {
    m_sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockFd < 0) return false;

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);

    struct timeval tv{1, 0}; 
    setsockopt(m_sockFd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof(tv));

    if (connect(m_sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(m_sockFd);
        m_sockFd = -1;
        return false;
    }
    return true;
}

void EdgeDevice::loop() {
    char buffer[256] = {0};
    
    while (true) {
        auto data = readSensors();

        std::string payload = "{\"temp\":" + std::to_string(data.temperature).substr(0, 5) + 
                              ",\"hum\":" + std::to_string(data.humidity).substr(0, 5) + 
                              ",\"fan\":" + (data.fanStatus ? "true" : "false") + "}\n";
        
        if (send(m_sockFd, payload.c_str(), payload.length(), 0) <= 0) {
            std::cerr << "❌ Connection lost to server. Attempting reconnect...\n";
            break;
        }

        int bytesReceived = recv(m_sockFd, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::string command(buffer);
            
            if (command.find("FAN_ON") != std::string::npos) {
                m_fanStatus = true;
                std::cout << "⚙️ Hardware Actuator: Turning Fan ON\n";
            } else if (command.find("FAN_OFF") != std::string::npos) {
                m_fanStatus = false;
                std::cout << "⚙️ Hardware Actuator: Turning Fan OFF\n";
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    EdgeDevice device;
    std::cout << "📡 STM32 Virtual Edge Device Booting...\n";
    
    while (!device.connectToServer("127.0.0.1", 8080)) {
        std::cerr << "⚠️ Central Server offline. Retrying in 3 seconds...\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    
    std::cout << "🚀 Connected to Central Server! Telemetry streaming...\n";
    device.loop();
    return 0;
}
