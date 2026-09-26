# IoT Greenhouse Simulation (Decoupled C++ Architecture)

This project simulates an IoT smart greenhouse by separating the edge device hardware from the central tracking server. They run as two separate programs and talk over a network socket on Port 8080.

## 📁 Project Layout
* **`include/`**: Header files (`CentralServer.hpp`, `EdgeDeviceClient.hpp`)
* **`src/`**: Implementation files (`CentralServer.cpp`, `EdgeDeviceClient.cpp`)
* **`CMakeLists.txt`**: Build configuration script

---

## 🛠️ How to Compile
Open your terminal and run these commands to build the project:
```bash
cd build
cmake ..
make
```

---

## 🚀 How to Run and Control the Fan

Open **two separate terminal windows**.

### 1. Start the Server (Terminal 1)
```bash
./CentralServer
```
The server will boot up and start listening for connections.

### 2. Start the Edge Client (Terminal 2)
```bash
./EdgeDeviceClient
```
The client will connect to the server and begin streaming telemetry data every second.

### 3. Turn the Fan ON/OFF Manually
Go back to **Terminal 1 (CentralServer)**. While the data is scrolling on your screen, type either command and press **Enter**:
* Type **`FAN_ON`** to turn the fan on (you will watch the temperature start dropping).
* Type **`FAN_OFF`** to turn the fan off.
