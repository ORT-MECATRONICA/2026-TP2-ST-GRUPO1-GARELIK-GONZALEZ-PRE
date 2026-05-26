// Grupo 1 -- GARELIK,GONZALEZ,PRE

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include "time.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int SW1 = 35;
const int SW2 = 34;
float temperatura = 0;

unsigned long startMillis;
unsigned long currentMillis;
unsigned long tiempoparasegundos = 1000;
unsigned long segundos = 0;
unsigned long minutos = 19;
unsigned long horas = 15;

enum Estados { MONITOR, CONFIG, DOSSW, SW1L, SW2L, DOSSW2 };
Estados estadoActual = MONITOR;

void setup() {
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
 
  dht.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }
  display.clearDisplay();
  Serial.begin(115200);
  startMillis = millis();
  
}

void loop() {
  currentMillis = millis();
  if (currentMillis - startMillis >= tiempoparasegundos){
    segundos++;
    startMillis = currentMillis;
  }
  if (segundos >= 60)
  {
    minutos++;
    segundos = 0;
  }
  if (minutos >= 60)
  {
    horas++;
    minutos = 0;
  }
  if (horas >= 24){
    horas = 0;
  }
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
      estadoActual = CONFIG;
      horas++;
    }
    if (digitalRead(SW2) == LOW){
      estadoActual = DOSSW2;
    }
    break;

    case SW2L:
    if (digitalRead(SW2) == HIGH){
      estadoActual = CONFIG;
      minutos++;
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
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("MODO: MONITOR");
 
  display.setTextSize(1);
  display.setCursor(0,20);
  display.print("Hora: " );
  display.print(horas);
  display.print(":");
  display.println(minutos);

  display.setTextSize(1);
  display.setCursor(0,50);
  display.print("Temp: ");
  display.print(temperatura, 1);
  display.display();
  
}


void ajustarUmbral() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("MODO: CONFIG");
 
  display.setTextSize(1);
  display.setCursor(0,25);
  display.print("" );
  display.print(horas);
  display.print(":");
  display.print(minutos);
  display.display();
}
