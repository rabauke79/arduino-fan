/*
   MOSFET: IRFZ44N

   Temperatursensor: DHT22 (Temperatur- und Luftfeuchtigkeitssensor)

   Lüfter:
   Artic F12
   Startimpuls PWM 40
   Minimum PWM 10
*/

//*********************************************************************************************************************

#include "DHT.h"            //DHT Bibliothek hinzufügen

#define DHTPIN 2            // Der Sensor hängt am Arduino am digital Pin2
#define DHTTYPE DHT22       // Es handelt sich um den DHT22 mit Temperatur- und Luftfeuchtigkeitsensor
DHT dht(DHTPIN, DHTTYPE);   //sprechen wir den Sensor doch einfach immer mit "dht" an...

const int fan01 = 10;       //Der Ventilator Nummer 1 ist am Arduino am digitalen Pin10 angeschlossen (via MOSFET)
float Temperatur;           // Globale Variale für die Temperatur
float Luftfeuchtigkeit;     // Globale Variable für die Luftfeuchtigkeit
bool fanStartImpulsOn = false; // Prüfvariable, ob ich dem Lüfter schon einen kleinen Schubs gegeben habe...

//*********************************************************************************************************************

void setup() {
  Serial.begin(9600);     //Serielle Verbindung starten (zur Ausgabe im seriellen Monitor)
  dht.begin();            //DHT Sensor starten

  TCCR1B = TCCR1B & 0b11111000 | 0x01; // Setzt Timer1 (Pin 9 und 10) auf 31300Hz (0x01 kein Pfeifen - 0x03 default)
  pinMode(fan01, OUTPUT); // set pin to output
}

//*********************************************************************************************************************

void loop() {

  temperaturMessung();      // Messen der Temperatur
  delay(2000);              // Intervall der Temperaturmessung


  // Steuerung der Lüftergeschwindigkeit basierend auf der Temperaturauswertung

  if (Temperatur >= 24) {
    fanSpeed(fan01, 80);
  }
  else if (Temperatur >= 23) {
    fanSpeed(fan01, 10);
  }
  else {
    fanSpeed(fan01,0);
    fanStartImpulsOn = false;
    }
}

void fanStartImpuls(byte fan) {   /* Kurzer Impuls, um dem Motor einen Startimpuls zu geben */
  analogWrite(fan, 40);
  delay(250);
  fanStartImpulsOn = true;
}

void fanSpeed(byte fan, byte setFanSpeed) { /* Übergabe von Werten, mit welcher Geschwindigkeit der Lüfter drehen soll*/
  if (fanStartImpulsOn == false && setFanSpeed > 0) {
    fanStartImpuls(fan01);
  }
  analogWrite(fan, setFanSpeed);
}

void temperaturMessung() {
  Luftfeuchtigkeit = dht.readHumidity(); //die Luftfeuchtigkeit auslesen und unter „Luftfeutchtigkeit“ speichern
  Temperatur = dht.readTemperature();//die Temperatur auslesen und unter „Temperatur“ speichern
  Serial.print("Luftfeuchtigkeit: "); //Im seriellen Monitor den Text und
  Serial.print(Luftfeuchtigkeit); //die Dazugehörigen Werte anzeigen
  Serial.println(" %");
  Serial.print("Temperatur: ");
  Serial.print(Temperatur);
  Serial.println(" Grad Celsius");
}
