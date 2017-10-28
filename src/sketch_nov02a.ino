#include <Arduino.h>

// Program odczytuje temperaturę z czujnika

#include <OneWire.h>
#include <DS18B20.h>

// Numer pinu do którego podłaczasz czujnik
#define ONEWIRE_PIN 2

// Adres czujnika
byte address[8] = {0x28, 0x20, 0xF8, 0x0, 0x8, 0x0, 0x0, 0x15};
int ledPin = 13;
int histereza = 5;
int start = 40;
bool stateOn = false;

OneWire onewire(ONEWIRE_PIN);
DS18B20 sensors(&onewire);
class Controller
{
  public:
  void updateTemp(float temp)
  {
    if(temp >=start+histereza)
    {
      digitalWrite(ledPin, LOW);
      stateOn = true;
    }
    else if(temp < start)
    {
        digitalWrite(ledPin, HIGH);
        stateOn = false;
    }
  }
 };

Controller controller;
long unsigned int timestamp(0);
void setup() {
  while(!Serial);
  Serial.begin(9600);

  sensors.begin();
  sensors.request(address);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  if (sensors.available())
  {
    float temperature = sensors.readTemperature(address);

    Serial.print(temperature);
    Serial.print(F(" 'C "));
    Serial.println(stateOn ? "  ON": " OFF");
    if( millis() - timestamp > 1000)
    {
      timestamp = millis();
      controller.updateTemp(temperature);
    }
    sensors.request(address);
  }
if(Serial.available() > 0)
{
  start = Serial.parseInt();
  histereza = Serial.parseInt();
}

}
