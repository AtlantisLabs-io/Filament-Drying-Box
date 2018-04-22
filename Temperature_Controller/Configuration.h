/*
  Configuration.h

  Contains the settings for vairous elements of the heater box.

 Created by Christine Matthew P. Rogge
 Februrary 12, 2014.
 Released into the public domain.
 */
 
 #define HEATER_PIN  11
 #define KI 0.15
 #define KP 3.4
 #define KD 0.0
 #define MAX_DUTY_CYCLE  90
 #define MIN_DUTY_CYCLE 0
 #define B_COEFFICIENT 4096
 #define NUM_THERMISTOR_SAMPLES 20
 #define THERMISTOR_PIN A6
 #define R_NOMINAL 100000
 #define T_NOMINAL 25
 #define SERIES_RESISTOR 10000
 #define TIME_BASE 2000
 #define PWM true
 #define MAX_TEMP 100 //the max temp any heater is allowed to achieve
 #define MIN_SET_TEMP 0 //The minimum set temperature

 //DHT sensor
 #define DHTPIN 2    
#define DHTTYPE DHT22   // DHT 22  (AM2302)


/*
#ifndef Configuration_h
#define Configuration_h

#include "Arduino.h"

class Configuration
{
  public:
    struct Profile {
      //General
      int profileNumber;
      char name[20];
 
      //heating Zones
      double zone1InitialSetTemp;

      double zone1SetTemp;
 
      //Safety parameters
      double minExtrudeTemp;
      double maxTemp; //The maximum temp allowed for any heating zone
      unsigned long maxPreheatTime;
    } profile, tempProfile;

    //Heating zone
    struct HeatingZone {
      int timeBase;//Time base in milliseconds if zero standard PWM will be used
      double maxDutyCycle;//The max dutyCycle for the barrel
      double minDutyCycle;//The min dutyCycle for the barrel
      double Kp, Ki, Kd;
      long thermistorRNom;
      int thermistorTNom;
      int thermistorNumSamples;
      int thermistorBCoefficient;
      int thermistorSeriesResistor;
      int thermistorPin;
      int heaterPin;
      int coolerPin;
      double setTemp;
      boolean PWM;
      boolean activeCooling;
    };
	
	
    /*The physicalConfig struct holds information describing *
     *the physical setup of the Heater box.                    

    struct PhysicalConfig {
      //variables that help with storage
      boolean configStored;
      boolean loadAutomatically;

      //Heating Zones
      HeatingZone zone1;


      //Parameters for the different states
      double maxTemp;
    } physical;

    char profileNames[10][20];
    char name0[20];
    void loadProfileName(char* namePtr, int profile);//Used for menus.
    void loadProfileNames();
    Configuration();//constructor
    void saveConfig();
    void deleteConfig();
    void loadConfig();//returns true if successfull
    void loadDefaultConfig();
    void saveProfile();//Saves profile in designated position.
    boolean loadProfile(int profileNum);
    void loadDefaultProfile();
    void deleteProfile(int profileNum);

    //returns a pointer to the name of the indicated profile
    char *getName(int profileNum);


  private:
    int calculateOffset(int profileNum);//determines the address of a profile
    int numProfiles;//The number of allowed profiles
};

#endif
*/

