// ==========================================
// BLYNK IOT CONFIGURATION
// ==========================================
#define BLYNK_TEMPLATE_ID "TMPL6mcm-9kdx"
#define BLYNK_TEMPLATE_NAME "Smart Irrigation system"
#define BLYNK_AUTH_TOKEN "8bjesDWOnuoO14YMQmR64zKYjPoTNWWQ"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <FastLED.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Pixel_1019";
char pass[] = "@Kong2218";

// Pin Configurations
#define DHTPIN           4    
#define DHTTYPE          DHT22
#define SOIL_PIN         35   
#define WATER_PIN        34   
#define STATUS_LED_PIN   2    
#define RELAY_PUMP_PIN   5    
#define LED_STRIP_PIN    21   

#define BRIGHTNESS  128       
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

const uint8_t kMatrixWidth  = 16;
const uint8_t kMatrixHeight = 16;
const bool kMatrixSerpentineLayout = true;
#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
#define MAX_DIMENSION ((kMatrixWidth > kMatrixHeight) ? kMatrixWidth : kMatrixHeight)

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette(PartyColors_p);

uint16_t x, y, z;
uint16_t speed = 20;
uint16_t scale = 30;
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];
uint8_t colorLoop = 1;
bool ledStripOn = false;

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

int pumpState = LOW;
int pumpMode  = 0;          // 0 = Auto, 1 = Manual
int waterLevel = 0;
float mappedSoilMoisture = 0;

const int SOIL_MOISTURE_LOW  = 30; 
const int SOIL_MOISTURE_HIGH = 50; 

unsigned long previousMillis = 0;
const unsigned long SENSOR_INTERVAL = 1000; 

uint16_t XY(uint8_t x, uint8_t y);
void fillnoise8();
void mapNoiseToLEDsUsingPalette();
void ChangePaletteAndSettingsPeriodically();
void SetupRandomPalette();
void SetupBlackAndWhiteStripedPalette();
void SetupPurpleAndGreenPalette();
void autoControlPump(float moisture);
void controlLED(float moisture);
void readAndSendSensorData();
void updateLEDStrip();

// Blynk Handlers with Serial Print
BLYNK_WRITE(V5) { 
  if (pumpMode == 1) {
    pumpState = param.asInt();
    digitalWrite(RELAY_PUMP_PIN, pumpState);
    Serial.print("[MANUAL PUMP] Triggered by App -> State: ");
    Serial.println(pumpState == HIGH ? "ON" : "OFF");
  }
}

BLYNK_WRITE(V6) { 
  pumpMode = param.asInt();
  Serial.print("[SYSTEM MODE] Switched to: ");
  Serial.println(pumpMode == 0 ? "AUTOMATIC" : "MANUAL");
}

BLYNK_WRITE(V8) { 
  ledStripOn = (param.asInt() == 1);
  Serial.print("[LED STRIP] State: ");
  Serial.println(ledStripOn ? "ENABLED" : "DISABLED");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== ESP32 SMART IRRIGATION SYSTEM STARTING ===");

  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(RELAY_PUMP_PIN, OUTPUT);
  digitalWrite(RELAY_PUMP_PIN, LOW); 

  dht.begin();
  Serial.println("[SETUP] DHT Sensor Initialized");

  Blynk.begin(auth, ssid, pass);
  Serial.println("[SETUP] Connected to Wi-Fi & Blynk Cloud");

  Blynk.virtualWrite(V2, LOW);
  Blynk.virtualWrite(V3, "SYSTEM READY");
  Blynk.virtualWrite(V5, LOW);
  Blynk.virtualWrite(V6, LOW); 
  Blynk.virtualWrite(V8, LOW);

  FastLED.addLeds<LED_TYPE, LED_STRIP_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);

  x = random16();
  y = random16();
  z = random16();
  Serial.println("[SETUP] FastLED Initialized");
  Serial.println("===============================================\n");
}

void loop() {
  Blynk.run();
  timer.run();

  updateLEDStrip();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= SENSOR_INTERVAL) {
    previousMillis = currentMillis;

    Serial.println("--- [ SENSOR TELEMETRY READOUT ] ---");

    // 1. Water Level Sensor
    int rawWater = analogRead(WATER_PIN);
    if (rawWater < 450) {
      waterLevel = 0;
    } else {
      waterLevel = map(rawWater, 1200, 1600, 0, 100);
      waterLevel = constrain(waterLevel, 0, 100);
    }
    Serial.print("  Water Sensor Raw: "); Serial.print(rawWater);
    Serial.print(" | Water Level: "); Serial.print(waterLevel); Serial.println("%");

    // 2. Soil Moisture Sensor
    int soilRaw = analogRead(SOIL_PIN);
    mappedSoilMoisture = map(soilRaw, 2750, 1023, 0, 100);
    mappedSoilMoisture = constrain(mappedSoilMoisture, 0, 100);

    Serial.print("  Soil Sensor Raw:  "); Serial.print(soilRaw);
    Serial.print(" | Soil Moisture: "); Serial.print(mappedSoilMoisture); Serial.println("%");

    if (pumpMode == 0) {
      autoControlPump(mappedSoilMoisture);
    } else {
      Serial.println("  Auto Irrigation: DISABLED (Manual Mode)");
    }

    controlLED(mappedSoilMoisture);
    readAndSendSensorData();
    Blynk.virtualWrite(V7, waterLevel);

    Serial.println("------------------------------------\n");
  }
}

void autoControlPump(float moisture) {
  if (waterLevel < 20) {
    pumpState = LOW;
    Blynk.virtualWrite(V3, "WATER TANK LOW!");
    Blynk.setProperty(V3, "color", "#D3435C");
    Serial.println("  [PUMP SAFETY CUTOFF] Water tank level < 20%! Pump forced OFF.");
  } else if (moisture <= SOIL_MOISTURE_LOW) {
    pumpState = HIGH; 
    Serial.println("  [PUMP CONTROL] Soil is DRY (<= 40%). Turning Pump ON.");
  } else if (moisture >= SOIL_MOISTURE_HIGH) {
    pumpState = LOW;  
    Serial.println("  [PUMP CONTROL] Soil is MOIST (>= 70%). Turning Pump OFF.");
  }

  digitalWrite(RELAY_PUMP_PIN, pumpState);
  
  if (pumpMode == 0) {
    Blynk.virtualWrite(V5, pumpState);
  }
}

void controlLED(float moisture) {
  // If the pump is actively running, show status
  if (pumpState == HIGH) {
    digitalWrite(STATUS_LED_PIN, HIGH);
    Blynk.virtualWrite(V2, HIGH);
    Blynk.virtualWrite(V3, "PUMP ACTIVE");
    Blynk.setProperty(V3, "color", "#00FF00"); // Green text in app
  } 
  // If water tank is low, prioritize warning
  else if (waterLevel < 20) {
    digitalWrite(STATUS_LED_PIN, LOW);
    Blynk.virtualWrite(V2, LOW);
    Blynk.virtualWrite(V3, "WATER TANK LOW!");
    Blynk.setProperty(V3, "color", "#FF0000"); // Red warning text
  } 
  // Normal monitoring colors based on soil moisture
  else {
    digitalWrite(STATUS_LED_PIN, LOW);
    Blynk.virtualWrite(V2, LOW);

    if (moisture < 30) {
      // DRY: Red alert in Blynk App
      Blynk.virtualWrite(V3, "SOIL DRY (NEEDS WATER)");
      Blynk.setProperty(V3, "color", "#FF0000"); // Hex Red
    } 
    else if (moisture >= 30 && moisture <= 70) {
      // OPTIMAL: Green status in Blynk App
      Blynk.virtualWrite(V3, "SOIL MOISTURE OPTIMAL");
      Blynk.setProperty(V3, "color", "#00FF00"); // Hex Green
    } 
    else {
      // TOO HIGH (> 70%): Blue warning in Blynk App
      Blynk.virtualWrite(V3, "SOIL OVER-WATERED!");
      Blynk.setProperty(V3, "color", "#0000FF"); // Hex Blue
    }
  }
}

void readAndSendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);
    Serial.print("  DHT22 Temp: "); Serial.print(t); Serial.print(" °C");
    Serial.print(" | Humidity: "); Serial.print(h); Serial.println(" %");
  } else {
    Serial.println("  [DHT22 ERROR] Failed to read from temperature sensor!");
  }

  if (pumpMode == 0) {
    Blynk.virtualWrite(V4, mappedSoilMoisture);
  }
}

void updateLEDStrip() {
  if (ledStripOn) {
    ChangePaletteAndSettingsPeriodically();
    fillnoise8();
    mapNoiseToLEDsUsingPalette();
    FastLED.show();
  } else {
    FastLED.clear(true); 
  }
}

void fillnoise8() {
  uint8_t dataSmoothing = 0;
  if (speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }
  for (int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;
      uint8_t data = inoise8(x + ioffset, y + joffset, z);
      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));
      if (dataSmoothing) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8(olddata, dataSmoothing) + scale8(data, 256 - dataSmoothing);
        data = newdata;
      }
      noise[i][j] = data;
    }
  }
  z += speed;
  x += speed / 8;
  y -= speed / 16;
}

void mapNoiseToLEDsUsingPalette() {
  static uint8_t ihue = 0;
  for (int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {
      uint8_t index = noise[j][i];
      uint8_t bri   = noise[i][j];
      if (colorLoop) {
        index += ihue;
      }
      if (bri > 127) {
        bri = 255;
      } else {
        bri = dim8_raw(bri * 2);
      }
      CRGB color = ColorFromPalette(currentPalette, index, bri);
      leds[XY(i, j)] = color;
    }
  }
  ihue += 1;
}

#define HOLD_PALETTES_X_TIMES_AS_LONG 1
void ChangePaletteAndSettingsPeriodically() {
  uint8_t secondHand = ((millis() / 1000) / HOLD_PALETTES_X_TIMES_AS_LONG) % 60;
  static uint8_t lastSecond = 99;
  if (lastSecond != secondHand) {
    lastSecond = secondHand;
    if (secondHand == 0)  { currentPalette = RainbowColors_p;       speed = 20; scale = 30;  colorLoop = 1; }
    if (secondHand == 5)  { SetupPurpleAndGreenPalette();          speed = 10; scale = 50;  colorLoop = 1; }
    if (secondHand == 10) { SetupBlackAndWhiteStripedPalette();    speed = 20; scale = 30;  colorLoop = 1; }
    if (secondHand == 15) { currentPalette = ForestColors_p;       speed = 8;  scale = 120; colorLoop = 0; }
    if (secondHand == 20) { currentPalette = CloudColors_p;        speed = 4;  scale = 30;  colorLoop = 0; }
    if (secondHand == 25) { currentPalette = LavaColors_p;         speed = 8;  scale = 50;  colorLoop = 0; }
    if (secondHand == 30) { currentPalette = OceanColors_p;        speed = 20; scale = 90;  colorLoop = 0; }
    if (secondHand == 35) { currentPalette = PartyColors_p;        speed = 20; scale = 30;  colorLoop = 1; }
    if (secondHand == 40) { SetupRandomPalette();                  speed = 20; scale = 20;  colorLoop = 1; }
    if (secondHand == 45) { SetupRandomPalette();                  speed = 50; scale = 50;  colorLoop = 1; }
    if (secondHand == 50) { SetupRandomPalette();                  speed = 90; scale = 90;  colorLoop = 1; }
    if (secondHand == 55) { currentPalette = RainbowStripeColors_p; speed = 30; scale = 20;  colorLoop = 1; }
  }
}

void SetupRandomPalette() {
  currentPalette = CRGBPalette16(
    CHSV(random8(), 255, 32),
    CHSV(random8(), 255, 255),
    CHSV(random8(), 128, 255),
    CHSV(random8(), 255, 255)
  );
}

void SetupBlackAndWhiteStripedPalette() {
  fill_solid(currentPalette, 16, CRGB::Black);
  currentPalette[0]  = CRGB::White;
  currentPalette[4]  = CRGB::White;
  currentPalette[8]  = CRGB::White;
  currentPalette[12] = CRGB::White;
}

void SetupPurpleAndGreenPalette() {
  CRGB purple = CHSV(HUE_PURPLE, 255, 255);
  CRGB green  = CHSV(HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  currentPalette = CRGBPalette16(
    green,  green,  black, black,
    purple, purple, black, black,
    green,  green,  black, black,
    purple, purple, black, black
  );
}

uint16_t XY(uint8_t x, uint8_t y) {
  uint16_t i;
  if (!kMatrixSerpentineLayout) {
    i = (y * kMatrixWidth) + x;
  } else {
    if (y & 0x01) {
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      i = (y * kMatrixWidth) + x;
    }
  }
  return i;
}