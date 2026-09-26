#ifndef EDGE_DEVICE_CLIENT_HPP
#define EDGE_DEVICE_CLIENT_HPP

#include <string>

struct GreenhouseData {
    float temperature;
    float humidity;
    bool fanStatus;
};

class EdgeDevice {
private:
    float m_currentTemp = 24.0f;
    bool m_fanStatus = false;
    int m_sockFd = -1;

    GreenhouseData readSensors();

public:
    EdgeDevice() = default;
    ~EdgeDevice();

    bool connectToServer(const std::string& ip, int port);
    void loop();
};

#endif
