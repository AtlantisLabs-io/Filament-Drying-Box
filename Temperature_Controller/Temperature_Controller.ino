//include libraries
#include "ClickEncoder.h"
#include "TimerOne.h"
#include "PID_v1.h"
#include "Thermistor.h"
#include "Configuration.h"
#include "Heater.h"
#include <DHT.h>;
#include <LiquidCrystal.h>

//Create the configuration
//Configuration configuration;


ClickEncoder *encoder;

//process variables
int setTemp;
int actTemp;
int heaterDS;
int fanDS;
int humidity;
int tempDHT;

//encoder variables
int16_t last, value;
ClickEncoder::Button bState;

//LCD and update timing
//LCD
const int rs = 4, rw = 5, en = 6, d4 = 7, d5 = 8, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, rw, en, d4, d5, d6, d7);
unsigned long now;
unsigned long updateTime;
int updateInterval = 1000;

//create the heater
Heater heater(setTemp);

//fan
int fanState = LOW;             // ledState used to set the LED
unsigned long previousMillis = 0;        // will store last time LED was updated
long interval = 1000;

//DHT Sensor
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Setup State Machine

//States enum
enum States {
  DISPLAY_MENU,
  CHANGE_SET_TEMP,
  CHANGE_FAN_DS,
  SAVE_SETTINGS,
  SAFETY_SHUTDOWN
} ;

States currentState;
boolean stateChanged = false;

//Declare state functions
void displayMenu();
void changeSetTemp();
void changeFanDS();
void saveSettings();
void safetyShutdown();

//Pointers to State functions
void (*state_table[])() = {
  displayMenu,
  changeSetTemp,
  changeFanDS,
  saveSettings,
  safetyShutdown
};



//Menu Setup
struct MenuItem {
  String text;
  int* value;
  States state;
};

void changeSetTemp();
void changeFanDS();
void saveSettings();
int selectedItem = 0;
int topItem = 0;
const int numMenuItems = 6;
struct MenuItem menu[numMenuItems] = {
  {"Set Temp", &setTemp, CHANGE_SET_TEMP},
  {"Act Temp", &actTemp, DISPLAY_MENU},
  {"Heater DS", &heaterDS, DISPLAY_MENU},
  {"Fan DS", &fanDS, CHANGE_FAN_DS},
  {"Humidity", &humidity, DISPLAY_MENU},
  {"Save Settings", NULL, SAVE_SETTINGS}
};

void timerIsr() {
  encoder->service();
}

void setup() {
  Serial.begin(250000);
  //Set initial state
  currentState = DISPLAY_MENU;

  //Start humidity sensor
  dht.begin();

  //Start LCD
  lcd.begin(16, 2);
  lcd.clear();

  //Encoder
  encoder = new ClickEncoder(A1, A2, A0);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);

  //fan
  pinMode(FAN_PIN, OUTPUT);

  heater.setMode(MANUAL);
  heater.setDutyCycle(9);

  //Print initial values to LCD
  drawMenu();


}

void loop() {
  //Do tasks that must be done on each loop
  //Safety check
  //run fan

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (fanState == LOW) {
      interval = 1000;
      fanState = HIGH;
    } else {
      interval = 100;
      fanState = LOW;
    }

    // set the LED with the fanState of the variable:
    digitalWrite(FAN_PIN, fanState);
  }
  //activate heaters
  heater.activate();
  actTemp = heater.getTemp();
  heaterDS = heater.getDutyCycle();

  //check humidity
  humidity = round(dht.readHumidity());
  tempDHT = round(dht.readTemperature());


  //update the menu if the updateTime has been reached.
  //This keeps the displayed temperatures and duty cycles up to date
    now = millis();
    if (now >= updateTime) {
      updateTime = now + updateInterval;
      drawMenu(); // may need to switch to updateMenu() to reduce lcd flicker.
    }

  //Call function of the current state
  state_table[currentState]();
}

void displayMenu() {
  value = value + encoder->getValue();
  bState = encoder->getButton();

  //If the button has been clicked, switch to selected item's state
  if (bState == ClickEncoder::Clicked) {
    currentState = menu[selectedItem].state;
    stateChanged = true;
    return;
  }

  //If the encoder has been turned, advance the menu
  int encoderThreshold = 3;

  if (value > -encoderThreshold && value < encoderThreshold) {
    return;
  } else if (value > 0) {
    menuDown();
    value = 0;
  } else {
    menuUp();
    value = 0;
  }

  drawMenu();
}

void drawMenu() {

  //redraw the menu
  lcd.clear();
  if (topItem == selectedItem) {
    Serial.print(">");
    lcd.setCursor(0, 0);
    lcd.print (">");

  } else {
    Serial.print(" ");
    lcd.setCursor(1, 0);

  }
  lcd.print(menu[topItem].text);
  if (menu[topItem].value != NULL) {
    lcd.setCursor(16 - getDigits(*menu[topItem].value), 0);
    lcd.print(*menu[topItem].value);
  }

  Serial.print(menu[topItem].text);
  Serial.print("\t");
  Serial.println(getDigits(*menu[topItem].value));

  if (topItem + 1 == selectedItem) {
    Serial.print(">");
    lcd.setCursor(0, 1);
    lcd.print (">");
  } else {
    Serial.print(" ");
    lcd.setCursor(1, 1);
  }
  Serial.print(menu[topItem + 1].text);
  Serial.print("\t");
  Serial.println(getDigits(*menu[topItem + 1].value));
  Serial.println();
  lcd.print(menu[topItem + 1].text);
  if (menu[topItem + 1].value != NULL) {
    lcd.setCursor(16 - getDigits(*menu[topItem + 1].value), 1);
    lcd.print(*menu[topItem + 1].value);
  }
}

void updateMenu() {
  //Update the values being displayed without rewriting the entire lcd screen.
}

void menuUp() {
  if (selectedItem > 0) {
    if (topItem == selectedItem) {
      topItem--;
    }
    selectedItem--;
  }
}

void menuDown() {
  if (selectedItem < numMenuItems - 1) {
    if (topItem != selectedItem) {
      topItem++;
    }
    selectedItem++;
  }
}

//getDigits figures out how many digits are in a positive integer
int getDigits(int val) {
  if (val == 0) return 1;
  int count = 0;
  while (val != 0)
  {
    // n = n/10
    val /= 10;
    ++count;
  }
  return count;
}


void changeSetTemp() {
  static int prevTemp;

  //If the state has just been changed to changeSetTemp, set up the screen
  if (stateChanged) {
    stateChanged = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(menu[selectedItem].text);
    lcd.setCursor(8 - getDigits(*menu[selectedItem].value), 1);
    lcd.print(*menu[selectedItem].value);
  }

  bState = encoder->getButton();

  //If the button has been clicked, switch to selected item's state
  if (bState == ClickEncoder::Clicked) {
    currentState = DISPLAY_MENU;
    drawMenu();
    return;
  }

  setTemp += encoder->getValue();
  if (setTemp < MIN_SET_TEMP) {
    setTemp = MIN_SET_TEMP;
  }

  if (setTemp > MAX_TEMP) {
    setTemp = MAX_TEMP;
  }
  if (setTemp != prevTemp) {
    lcd.setCursor(7, 1);
    lcd.print("   ");
    lcd.setCursor(8 - getDigits(*menu[selectedItem].value), 1);
    lcd.print(*menu[selectedItem].value);
    Serial.print("New Temp: ");
    Serial.println(setTemp);
    prevTemp = setTemp;
  }

}


void changeFanDS() {
  Serial.println("Changing Fan DS!");
  Serial.println();
  currentState = DISPLAY_MENU;
  drawMenu();
}


void saveSettings() {
  Serial.println("Saving Settings!");
  Serial.println();
  currentState = DISPLAY_MENU;
  drawMenu();
}

void safetyShutdown() {

}


