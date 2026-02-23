#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPmDNS.h>
#include <Adafruit_BMP085.h>

constexpr auto WIFI_SSID = "Glazgen2";
constexpr auto WIFI_PASSWORD = "GlaZgeN0208";
constexpr auto SERVER_IP = "192.168.1.101";
constexpr uint16_t SERVER_PORT = 16546;

constexpr uint32_t BUTTON_DEBOUNCE_DELAY = 50;
constexpr uint8_t BUTTON_PIN = 18;

WiFiClient client;
Adafruit_BMP085 bmp;

volatile bool g_ButtonPressed = false;
volatile uint32_t g_ButtonPressedTime = 0;

void SensorTask(void* pvParameters)
{
    const TickType_t period = pdMS_TO_TICKS(1000);
    TickType_t lastWakeTime = xTaskGetTickCount();

    while (true)
    {
        Serial.println("Reading sensors...");
        // readSensors();
        // saveToFlash();

        vTaskDelayUntil(&lastWakeTime, period);
    }
}

void ARDUINO_ISR_ATTR ButtonCallback()
{
    const auto now = millis();

    if (now - g_ButtonPressedTime > BUTTON_DEBOUNCE_DELAY)
        g_ButtonPressed = true;

    g_ButtonPressedTime = now;
}

void setup()
{
    // delay(3000); // to make sure I have enough time to open serial monitor

    Serial.begin(115200);
    // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     Serial.println("Attempting to connect to WiFi!");
    //     delay(500);
    // }
    
    // Serial.println("WiFi connected!");
    // Serial.println(WiFi.localIP());

    // while (!client.connect(SERVER_IP, SERVER_PORT))
    // {
    //     Serial.println("Attempting to connect to the server!");
    //     delay(500);
    // }
    
    // Serial.println("Connected to server!");

    // while (!bmp.begin())
    // {
    //     Serial.println("Attempting to find valid BMP085 sensor!");
    //     delay(500);
    // }

    pinMode(BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(BUTTON_PIN, ButtonCallback, HIGH);

    xTaskCreatePinnedToCore(
        SensorTask,
        "SensorTask",
        4096,
        NULL,
        1,
        NULL,
        1
    );
}

void loop()
{
    if (g_ButtonPressed)
    {
        const auto temp = bmp.readTemperature();
        const auto pressure = bmp.readPressure();

        if (client.connected())
        {
            client.print("Temperature = ");
            client.print(temp);
            client.println(" C");

            client.print("Pressure = ");
            client.print(pressure);
            client.println(" Pa");
        }

        g_ButtonPressed = false;
    }
}
