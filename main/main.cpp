#include "HardwareSerial.h"
#include "driver/gpio.h"
#include "esp32-hal-gpio.h"
#include "esp_sleep.h"
#include <Arduino.h>
/*
Hardware Connections
======================
Push Button to GPIO 0 pulled down with a 10K Ohm
resistor

NOTE:
======
Bit mask of GPIO numbers which will cause wakeup. Only GPIOs
which have RTC functionality can be used in this bit map.
For different SoCs, the related GPIOs are:
- ESP32: 0, 2, 4, 12-15, 25-27, 32-39
- ESP32-S2: 0-21
- ESP32-S3: 0-21
- ESP32-C6: 0-7
- ESP32-H2: 7-14
*/

#define BUTTON_PIN_BITMASK 0x0002

#define GPIO_LEFT_BUTTON	13
#define GPIO_RIGHT_BUTTON	1
#define GPIO_PULLUP_ENA		2
#define GPIO_ORANGE_LED		15

RTC_DATA_ATTR int bootCount = 0;

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(GPIO_LEFT_BUTTON,INPUT);
  pinMode(GPIO_RIGHT_BUTTON,INPUT);
  pinMode(GPIO_ORANGE_LED,OUTPUT);

  //pinMode(GPIO_PULLUP_ENA,OUTPUT);
  //digitalWrite(GPIO_PULLUP_ENA,1);

  digitalWrite(GPIO_ORANGE_LED,0);
  delay(1000); //Take some time to open up the Serial Monitor
  digitalWrite(GPIO_ORANGE_LED,1);

  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  /*
  First we configure the wake up source
  We set our ESP32 to wake up for an external trigger.
  There are two types for ESP32, ext0 and ext1, ext0 
  don't support ESP32C6 so we use ext1.
  */

  //If you were to use ext1, you would use it like
  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK,ESP_EXT1_WAKEUP_ANY_LOW);

  delay(3000);

  for(int x=0;x<3;x++)
  {
	  digitalWrite(GPIO_ORANGE_LED,1);
	  delay(900);

	  digitalWrite(GPIO_ORANGE_LED,0);
	  delay(100);
  }

  //Go to sleep now
  Serial.println("Going to sleep now");
  esp_deep_sleep_start();

  Serial.println("This will never be printed");
}

void loop(){
  //This is not going to be called
}