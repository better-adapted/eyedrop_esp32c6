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

#define uS_TO_S_FACTOR 1000000ULL /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30          /* Time ESP32 will go to sleep (in seconds) */

#define GPIO_LEFT_BUTTON	2
#define GPIO_RIGHT_BUTTON	3
#define GPIO_RED_LED		22
#define GPIO_GREEN_LED		23

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
  //Go to sleep now
  //Serial.println("Going to sleep now");
  //esp_deep_sleep_start();

  Serial.begin(115200);

  pinMode(GPIO_LEFT_BUTTON,INPUT);
  pinMode(GPIO_RIGHT_BUTTON,INPUT);
  pinMode(GPIO_RED_LED,OUTPUT);
  pinMode(GPIO_GREEN_LED,OUTPUT);

  //pinMode(GPIO_PULLUP_ENA,OUTPUT);
  //digitalWrite(GPIO_PULLUP_ENA,1);

  digitalWrite(GPIO_GREEN_LED,1);
  digitalWrite(GPIO_GREEN_LED,1);
  delay(500); //Take some time to open up the Serial Monitor
  digitalWrite(GPIO_RED_LED,0);
  digitalWrite(GPIO_RED_LED,0);
  delay(500); //Take some time to open up the Serial Monitor

#ifdef LED_FLASH
  //Increment boot number and print it every reboot
  ++bootCount;
  Serial.println();
  Serial.println("Boot number: " + String(bootCount));
  Serial.println();

  //Print the wakeup reason for ESP32
  print_wakeup_reason();

  /*
  First we configure the wake up source
  We set our ESP32 to wake up for an external trigger.
  There are two types for ESP32, ext0 and ext1, ext0 
  don't support ESP32C6 so we use ext1.
  */

  //If you were to use ext1, you would use it like


  delay(1000);

  for(int x=0;x<5;x++)
  {
	  digitalWrite(GPIO_GREEN_LED,1);
	  delay(100);

	  digitalWrite(GPIO_GREEN_LED,0);
	  delay(900);
  }

  digitalWrite(GPIO_RED_LED,1);

  delay(1000); //Take some time to open up the Serial Monitor
  digitalWrite(GPIO_RED_LED,0);

  //Go to sleep now
#endif

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  int res = esp_sleep_enable_ext1_wakeup(0b00001100,ESP_EXT1_WAKEUP_ANY_LOW);
  Serial.println("\r\nesp_sleep_enable_ext1_wakeup() :" +  String(res));

  Serial.println("Going to sleep now");
  delay(100);
  esp_deep_sleep_start();
}

void loop(){
  //This is not going to be called
}