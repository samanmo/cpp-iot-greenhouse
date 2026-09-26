# Decoupled Hardware-Abstracted IoT Greenhouse System

A high-performance C++17 IoT Distributed Engine Simulation tracking asynchronous network telemetry patterns. This system isolates edge device firmware loops from the central aggregator infrastructure, eliminating physical hardware dependencies (such as an STM32F401 microcontroller) for rapid testing.

Instead of running a single monolithic loop, this repository splits execution across two distinct, multi-threaded binary executables communicating over a standardized network socket protocol.

---

## 🏗️ Architecture Design

[ EdgeDeviceClient Node ]                   [ CentralServer Gateway ]
(Simulates STM32 Hardware)                  (Cloud Gateway / Host PC)
||
| -------- 1. Dial-Out Connection --------> | (Accepts Socket on 8080)
||
| -------- 2. 1Hz JSON Telemetry Stream --> | (Parses & Logs Metrics)
||
| <------- 3. Bidirectional Actuator ------ | (Dispatches Over-The-Air)

1. **The Edge Node (`EdgeDeviceClient`):** Mirrors an on-field microcontroller operating behind an isolated local network. Configured as a TCP Client, it dials outward to a static infrastructure gateway, tracking environmental state physics natively while waiting for bidirectional actuator command words (`FAN_ON` / `FAN_OFF`).
2. **The Cloud Aggregator (`CentralServer`):** Runs as a persistent asynchronous POSIX TCP Server. It opens port `8080`, dispatches a dedicated detached operating system thread for every registering hardware device, and acts as the central command telemetry terminal.

---

## 📊 Network Data Protocol (JSON Payload)

The systems decouple completely by transmitting serialized JSON strings over live TCP streams rather than sharing internal C++ memory objects:

```json
{
  "temp": 24.10,
  "hum": 57.00,
  "fan": false
}
```

---

## 📁 Repository Structure

```text
.
├── CMakeLists.txt              # Multi-target modern CMake configuration
├── README.md                   # System operational guide
├── include/                    # Decoupled interface headers
│   ├── CentralServer.hpp
│   └── EdgeDeviceClient.hpp
└── src/                        # Modular application execution source code
    ├── CentralServer.cpp
    └── EdgeDeviceClient.cpp
```

---

## 🛠️ Prerequisites & Compilation

Ensure your Linux environment contains a modern development toolchain and compilation suite.

### Fedora / RedHat-based Distributions
```bash
sudo dnf groupinstall -y "Development Tools"
sudo dnf install -y cmake gdb
```

### Building the Project
Generate and build both target executable binaries simultaneously using standard out-of-source CMake workflows:

```bash
# 1. Access or create your local workspace build cache
mkdir -p build && cd build

# 2. Reset the cache and map project configurations
rm -rf *
cmake ..

# 3. Compile all distributed software instances 
make
```

---

## 🚀 Step-by-Step Execution Guide

To run this distributed framework, open **two distinct terminal windows or tabs**.

### 1. Fire Up the Infrastructure Gateway
Always boot up the centralized tracking receiver node first so the local socket layer bounds correctly:
```bash
cd build/
./CentralServer
```
*Console output indicates the port listener loop has locked onto active service pathways:*
> `🖥️ Central IoT Aggregator Gateway listening on port 8080...`

### 2. Boot Up the Virtual STM32 Microcontroller Node
In a separate terminal space, launch your edge hardware instance:
```bash
cd build/
./EdgeDeviceClient
```
*The client application handles automated handshake retry procedures until connectivity hooks succeed:*
> `📡 STM32 Virtual Edge Device Booting...`
> `🚀 Connected to Central Server! Telemetry streaming...`

---

## 📜 License
This project is open-source and available under the [MIT License](LICENSE).