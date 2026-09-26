#ifndef EDGE_DEVICE_HPP
#define EDGE_DEVICE_HPP

#include <string>
#include <cstdlib>

struct GreenhouseData {
    float temperature;
    float humidity;
    bool fanStatus;
};

class EdgeDevice {
private:
    float m_currentTemp = 24.0f;
    bool m_fanStatus = false;

public:
    EdgeDevice() = default;
    
    GreenhouseData readSensors() {
        m_currentTemp += ((rand() % 10) - 5) * 0.1f; 
        if (m_fanStatus) m_currentTemp -= 0.3f; 
        return { m_currentTemp, static_cast<float>(55 + (rand() % 5)), m_fanStatus };
    }

    void setActuator(const std::string& command) {
        if (command == "FAN_ON") m_fanStatus = true;
        else if (command == "FAN_OFF") m_fanStatus = false;
    }
};

#endif
