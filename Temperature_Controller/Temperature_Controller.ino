#include "PID_v1.h"
#include "Thermistor.h"
#include "Configuration.h"
#include "Heater.h"
//Globals
unsigned long now;
unsigned long refreshTime;
int refreshInterval = 1000;


//Create the configuration
Configuration configuration;

//create the heater
Heater heater1(&configuration.physical.zone1);

void setup() {
  // put your setup code here, to run once:
//  heater1._thermistor._tPin = A6;
  Serial.begin(250000);
  while (!Serial); // while the serial stream is not open, do nothing:
  //set heater temp
  configuration.physical.zone1.setTemp = configuration.profile.zone1SetTemp;
  Serial.print("SetTemp: ");
  Serial.println(configuration.physical.zone1.setTemp);
  heater1.setMode(MANUAL);
  heater1.setDutyCycle(50);
  heater1.setMode(AUTOMATIC);

}

void loop() {
  now = millis();
  
  heater1.activate();
  if (now > refreshTime){
    ;
    Serial.print("Temp: ");
    Serial.print(heater1.getTemp());
    Serial.print("\tDuty Cycle: ");
    Serial.println(heater1.getDutyCycle());
    refreshTime = now + refreshInterval;
  }
}
