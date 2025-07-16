#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define MQ135_PIN A0
#define DUST_SENSOR_PIN A1

#define GREEN_LED 7
#define YELLOW_LED 8
#define RED_LED 9

void setup() {
    Serial.begin(9600); 
    dht.begin();

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED failed");
        for (;;);
    }
    display.clearDisplay();

    pinMode(GREEN_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
}

void loop() {
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    int ppm = analogRead(MQ135_PIN);
    int dust = analogRead(DUST_SENSOR_PIN);

    Serial.print(temp);
    Serial.print(",");
    Serial.print(humidity);
    Serial.print(",");
    Serial.print(ppm);
    Serial.print(",");
    Serial.println(dust);

    if (ppm < 100) {  
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(RED_LED, LOW);
    } 
    else if (ppm >= 100 && ppm < 300) { 
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(YELLOW_LED, HIGH);
        digitalWrite(RED_LED, LOW);
    } 
    else {  
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(YELLOW_LED, LOW);
        digitalWrite(RED_LED, HIGH);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Temp: " + String(temp) + "C");
    display.println("Humidity: " + String(humidity) + "%");
    display.println("PPM: " + String(ppm));
    display.println("Dust: " + String(dust));
    display.display();

    delay(2000);  
}
