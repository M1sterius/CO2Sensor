# Air Quality sensor with live dashboard app

The project includes firmware for an esp32 based air quality sensor that measures carbon dioxide concentration as parts-per-millions as well as temperature and humidity. The centralized dashboard enables monitoring of air quality measured by the sensor in real-time as well as measurement history in graphs.

## Firmware features
- High precision carbon dioxide measurements via Sensirion SCD40 sensor
- LCD1602 Display showing running average of most recent measurements
- Circular buffer-like storage on LittleFS to cache measurements when WiFi or server aren't available
- All main firmware routines (sensor polling, display updates, networking, etc) are implemented in FreeRTOS tasks to optimize MCU usage
- Configuration from supporting app on PC via serial

## Dashboard features
- Modern reactive UI written in QML/Qt 6.10
- Asynchronous multithreaded networking with boost::asio
- Live measurements visualization with description of potential health effects of current CO2 level
- Measurements history visualization via QtGraph extension

## Dashboard UI showcase
![main_dashboard](Images/main_dashboard.png)

![date_selector](Images/date_selector.png)

![sensor_config](Images/sensor_config.png)

## Required electronics
- Sensirion SCD40
- ESP32 CH340
- LCD1602 with an I2C adapter
- A push button and wires

## Wiring schema
- SCD40
  - GND - GND
  - VDD - 3.3v
  - SCL - GPIO22
  - SDA - GPIO21
- LCD1602 (with I2C adapter)
  - GND - GND
  - VDD - 5.0v
  - SCL - GPIO22
  - SDA - GPIO21
- Configuration button
  - GPIO18 - Button - GND

## Tech used
- Modern C++20
- FreeRTOS
- boost::asio for asynchronous networking
- QML/Qt 6.10 and QtGraph extension for modern reactive UI
- serial and fmt libraries
- platformio to compile and flash the firmware

## Architecture overview
The project consists of two main parts: **Firmware** for ESP32 and **Dashboard** desktop application.
### Firmware
- A a FreeRTOS task is used to poll `Sensirion SCD40` sensor at regular intervals to obtain CO2 ppm, temperature and humidity measurements.
- Inter-task communication is handled via thread-safe FreeRTOS queues, notifying the `NetworkTask` the moment new data is ready.
- When `NetworkTask` awakes, it sends the measurement to the server via TCP or saves it to a circular buffer implemented
  on LittleFS, if connection to the server could not be established.
- When the sensor mamages to reestablish connection to the server, it will send all accumulated data to it in batches.
- Running average of last 5 measurements is printed on LCD display.

### Dashboard
- Dashboard app runs a Boost.Asio based server in the background to accept data packets coming from ESP32.
- If the server does not receive any data from the ESP32 for too long (about 2x sensor polling interval), it will
  automatically disconnect the client and start waiting for a reconnect attempt.
- All incoming data is saved to files to make sure you can always view graphs of previous day's measurements.
- The graph uses std::multiset to sort all measurements by timestamp even if they were received out of order.
- The app implements sensor configuration mode via a simple string of tokens separated by a `;` that gets sent to the sensor via serial. 


## Building and running the project
### Firmware
1. Open `Firmware` directory in VS Code or CLion with **PlatformIO** extension enabled.
2. Make sure your ESP32 board variation matches `esp32doit-devkit-v1` or change `board` field in `platformio.ini` file.
3. Connect your ESP32 via USB.
4. Click **Upload** to compile and flash.

### Dashboard
1. Ensure **Qt 6.10** with support for **QML** and **QtGraphs** is installed.
2. Open `Dashboard` directory in **QtCreator** and select a suitable kit.
3. Configure `CMakeLists.txt` if needed, then press **Build and Run**
