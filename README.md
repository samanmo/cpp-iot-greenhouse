<<<<<<< HEAD
# cpp-iot-greenhouse
A high-performance C++17 IoT Edge simulation that runs hardware-free using POSIX sockets. Simulates a smart greenhouse streaming real-time sensor metrics (Temp, Humidity) to a TCP server with bidirectional actuator controls. Built with modern CMake, multi-threading, and zero physical dependencies—perfect for rapid edge testing.
=======
# Hardware-Abstracted IoT Virtual Greenhouse System

A high-performance, professional **C++17 IoT Edge Engine Simulation** that runs seamlessly on any Linux ( possibly macOS - Not tested ) environment without needing physical microcontrollers (like the STM32F401). 

This project demonstrates a production-ready, multi-threaded TCP socket architecture that decouples physical sensor registers from edge computing logic. It allows teams to test edge business rules, data formatting, and remote actuator control pipelines inside a completely virtualized hardware layer.

---

## ✨ Features
* **Hardware Co-Simulation Layer:** Object-oriented mock layer simulating dynamic environmental changes (Temperature, Humidity) using dynamic state models rather than hardware blocks.
* **Multi-Threaded Socket Server:** High-performance POSIX networking layer capable of streaming metrics to multiple remote dashboards or client interfaces simultaneously.
* **Bidirectional Actuator Controls:** Fully interactive runtime commands allow remote network endpoints to pass signals down to the virtual physical engine.
* **Modern Build Configuration:** Powered by CMake for easy cross-platform compilation.

---

## 🛠️ Prerequisites & Installation

### Fedora / RedHat-based Linux
```bash
sudo dnf groupinstall -y "Development Tools"
sudo dnf install -y cmake gdb
```

### Ubuntu / Debian-based Linux
```bash
sudo apt update
sudo apt install -y build-essential cmake gdb
```

---

## ⚙️ Compilation & Build

Compile the production binary cleanly using the native CMake toolchain:

```bash
# 1. Configure the project and prepare the build environment
cmake -B build -S .

# 2. Compile the binaries
cmake --build build
```

---

## 🚀 Live Demonstration Guide

### 1. Launch the IoT Server Node
Start the core application node on your system:
```bash
./build/IoT_Greenhouse
```
*The server will initialize and begin listening for TCP dashboard client handshakes on port `8080`.*

### 2. Connect Your Client Dashboard
Simulate an active client connection or dashboard monitoring tool in a separate terminal using `netcat`:
```bash
nc localhost 8080
```

### 3. Interactive Actuator Control
While telemetry data is streaming live, you can send manual over-the-network commands straight to the edge device. Type the following commands into your active `netcat` session and press **Enter**:
* `FAN_ON`  — Turns on the greenhouse cooling mechanism. You will see the temperature stream begin dropping step-by-step.
* `FAN_OFF` — Shuts down the ventilation system, allowing internal environmental temperatures to rise again.
>>>>>>> 26e4361 (Initial commit: Complete C++ greenhouse network engine)
