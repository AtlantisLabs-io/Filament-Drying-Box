/*
Configuration.cpp - Library for saving, loading,
and deleting the various configurations both
for the active profile and the physical setup
of the extruder.

Created by Matthew P. Rogge, Februrary 12, 2014.
Released into the public domain.

*/

#include "Arduino.h"
#include "EEPROMAnything.h"
#include "Configuration.h"
#include "Thermistor.h"

void Configuration::loadDefaultProfile() {

  //General
  profile.profileNumber = -1;
  strcpy(profile.name, "PET 1.85mm");

  //heat Zones
  profile.zone1InitialSetTemp = 0.0;
  
  profile.zone1SetTemp = 100.0;

  //Safety parameters
  profile.maxTemp = 285; //The max temp allowed for any heating zone
}


void Configuration::loadDefaultConfig()
{

  //load the physical with the default parameters.
  //EEPROM storage
  physical.loadAutomatically = false;


  //Zone 1 heater
  physical.zone1.heaterPin = 12;
  physical.zone1.Kd = 0.0;
  physical.zone1.Ki = 0.15;
  physical.zone1.Kp = 3.4;
  physical.zone1.maxDutyCycle = 100;
  physical.zone1.minDutyCycle = 0;
  physical.zone1.thermistorBCoefficient = 3950;
  physical.zone1.thermistorNumSamples = 20;
  physical.zone1.thermistorPin = A6;
  physical.zone1.thermistorRNom = 100000;
  physical.zone1.thermistorSeriesResistor = 1000;
  physical.zone1.timeBase = 2000;
  physical.zone1.setTemp = 0.0;
  physical.zone1.PWM = true;
  physical.zone1.activeCooling = false;

  //Safety Parameters for the different states
  physical.maxTemp = 280;//the max temp any heater is allowed to achieve
}

Configuration::Configuration()
{
  loadDefaultProfile();
  loadDefaultConfig();
  //loadConfig();
}


void Configuration::saveConfig()
{
  Serial.println(F("Saving Config."));
  physical.configStored = true;//Make sure that the config is marked as stored
  EEPROM_writeAnything(0, physical); //Write to EEPROM
}


void Configuration::deleteConfig()
{
  physical.configStored = false;//Mark config as not stored
  EEPROM_writeAnything(0, physical.configStored); //Just update the configStored variable the rest is irrelevant
}


void Configuration::loadConfig()
{
  EEPROM_readAnything(0, physical);
}


void Configuration::saveProfile()
{
  EEPROM_writeAnything(calculateOffset(profile.profileNumber), profile);
}

void Configuration::deleteProfile(int profileNum)
{
}

boolean Configuration::loadProfile(int profileNum)
{
  EEPROM_readAnything(calculateOffset(profileNum), profile);
}

int Configuration::calculateOffset(int profileNum)
{
  return sizeof(PhysicalConfig) + profileNum * sizeof(Profile); //Will padding in the struct cause problems with this?
}

void Configuration::loadProfileName(char* namePtr, int profile) {
  EEPROM_readAnything(calculateOffset(profile), tempProfile);
  strcpy(namePtr, tempProfile.name);
}

void Configuration::loadProfileNames() {
  for (int i = 0; i < 10; i++) {
    loadProfileName(profileNames[i], i);
  }
}


