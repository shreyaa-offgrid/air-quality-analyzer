#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "NSUT_WIFI"
#define WIFI_PASS ""

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "hidden"
#define AIO_KEY "hidden"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish tempFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temp");
Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
Adafruit_MQTT_Publish ppmFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/PPM");
Adafruit_MQTT_Publish dustFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Dust"); 

void setup() {
    Serial.begin(9600);  
    Serial2.begin(9600, SERIAL_8N1, 16, 17); // RX2 = 16, TX2 = 17

    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nConnected to WiFi!");

    connectToMQTT();
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi lost... Reconnecting!");
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        delay(5000);
        return;
    }

    if (!mqtt.connected()) {
        connectToMQTT();
    }
    mqtt.processPackets(5000);

    if (Serial2.available()) {
        String data = Serial2.readStringUntil('\n');
        data.trim();

        Serial.print("Received: ");
        Serial.println(data);

        float temp, humidity, dust;
        int ppm;
        sscanf(data.c_str(), "%f,%f,%d,%f", &temp, &humidity, &ppm, &dust);

        if (!tempFeed.publish(temp)) Serial.println("Temp send failed!");
        if (!humidityFeed.publish(humidity)) Serial.println("Humidity send failed!");
        if (!ppmFeed.publish((int32_t)ppm)) Serial.println("PPM send failed!");
        if (!dustFeed.publish(dust)) Serial.println("Dust send failed!"); 

        Serial.println("Data Sent to Adafruit!");
    }

    delay(10000); 
}

void connectToMQTT() {
    Serial.print("Connecting to Adafruit IO...");
    while (mqtt.connect() != 0) {
        Serial.println("Failed, retrying...");
        delay(2000);
    }
    Serial.println("Connected to Adafruit IO!");
}

