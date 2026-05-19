// Grupo 1 -- GARELIK,GONZALEZ,PRE

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int SW1 = 35;
const int SW2 = 34;

enum Estados { MONITOR, CONFIG, DOSSW, SW1L, SW2L, DOSSW2 };
Estados estadoActual = MONITOR;

float temperatura = 0;
int segundos = 1;
int minuto = 30;
int hora = 12;

void setup() {
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
 
  dht.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);
  }
  display.clearDisplay();
}

void loop() {
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
      minuto++;
      estadoActual = CONFIG;
    }
    if (digitalRead(SW2) == LOW){
      estadoActual = DOSSW2;
    }
    break;

    case SW2L:
    if (digitalRead(SW2) == HIGH){
      hora++;
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
void mostrarMonitor() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("MODO: MONITOR");
 
  display.setTextSize(1);
  display.setCursor(0,20);
  display.print("Segundos: ");
  display.print(segundos);
 
  display.setTextSize(1);
  display.setCursor(0,30);
  display.print("Minuto: ");
  display.print(minuto);
  display.display();

  display.setTextSize(1);
  display.setCursor(0,40);
  display.print("Hora: ");
  display.print(hora);
  display.display();
}


void ajustarUmbral() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println("MODO: CONFIG");
 
  display.setTextSize(1);
  display.setCursor(0,20);
  display.print("SET: ");
  display.print(hora);
  display.display();

  display.setTextSize(1);
  display.setCursor(0,40);
  display.print("SET: ");
  display.print(minuto);
  display.display();
}