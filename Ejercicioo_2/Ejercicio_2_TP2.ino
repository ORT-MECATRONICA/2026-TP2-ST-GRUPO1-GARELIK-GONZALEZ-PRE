// Grupo 1 -- GARELIK,GONZALEZ,PRE

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <WiFi.h>
#include "time.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int SW1 = 35;
const int SW2 = 34;
const int LED = 25;
float temperatura = 0;
int SRGMT = -3;

enum Estados { MONITOR, CONFIG, DOSSW, SW1L, SW2L, DOSSW2 };
Estados estadoActual = MONITOR;

const char* ssid     = "MECA-IoT-V2";
const char* password = "IoT$2026";

const char* ntpServer = "pool.ntp.org";
int  gmtOffset_sec = -10800;
const long  daylightOffset_sec2 = 0;

void setup() {
  Serial.begin(115200);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
 
  dht.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }
  display.clearDisplay();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  configTime(daylightOffset_sec2, gmtOffset_sec, ntpServer);
  printLocalTime();

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop() {
  configTime(daylightOffset_sec2, gmtOffset_sec, ntpServer);
  temperatura = dht.readTemperature();
  switch (estadoActual) {
    case MONITOR:
      mostrarMonitor();
      if (digitalRead(SW1) == LOW && digitalRead(SW2) == LOW){
        estadoActual = DOSSW;
      }
      break;
     
    case CONFIG:
      ajustarUmbral();
    if (digitalRead(SW1) == LOW){
      estadoActual = SW1L;
    }
    if (digitalRead(SW2) == LOW){
      estadoActual = SW2L;
    }
      break;
    
    case DOSSW:
    if (digitalRead(SW1) == HIGH && digitalRead(SW2) == HIGH){
      estadoActual = CONFIG;
    }
    break;

    case SW1L:
    if (digitalRead(SW1) == HIGH){
      gmtOffset_sec = gmtOffset_sec + 3600;
      SRGMT = SRGMT + 1;
      if (SRGMT > 12){
        SRGMT = -11;
        gmtOffset_sec = -39600;
      }
      estadoActual = CONFIG;
    }
    if (digitalRead(SW2) == LOW){
      estadoActual = DOSSW2;
    }
    break;

    case SW2L:
    if (digitalRead(SW2) == HIGH){
      gmtOffset_sec = gmtOffset_sec - 3600;
      SRGMT = SRGMT - 1;
      if (SRGMT < -12){
        SRGMT = 11;
        gmtOffset_sec = 39600;
      }
      estadoActual = CONFIG;
    }
    if (digitalRead(SW1) == LOW){
      estadoActual = DOSSW2;
    }
    break;

    case DOSSW2:
    if(digitalRead(SW1) == HIGH && digitalRead(SW2) == HIGH){
      estadoActual = MONITOR;
    }
    break;
  }
}

void mostrarMonitor() { // display modo pantalla 1
struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("MODO: MONITOR");
 
  display.setTextSize(2);
  display.setCursor(0,20);
  display.print("Temp: ");
  display.print(temperatura, 1);
 
  display.setTextSize(1);
  display.setCursor(0,50);
  display.print(&timeinfo, "%H:%M:%S");
  display.display();
}

void ajustarUmbral() { // modo pantalla 2
  display.clearDisplay(); //display pantalla 2
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("MODO: CONFIG");
 
  display.setTextSize(2);
  display.setCursor(0,25);
  display.print("SET: ");
  display.print(SRGMT);
  display.display();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%H:%M:%S");
}