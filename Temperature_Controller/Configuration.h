/*
  Configuration.h - Library for saving, loading,
 and deleting the various configurations both
 for the active profile and the physical setup
 of the extruder.

 Created by Matthew P. Rogge, Februrary 12, 2014.
 Released into the public domain.
 */


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
     *the physical setup of the Heater box.                    */

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


