// ------------------------------------------------------------------------------------------------------------------------------------------------
#include "Arduino.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------
#include "Settings.h"                     // Settings pin
#include "VFD16BT.cpp"                    // VDF Class 
#include "OnkyoNR509.cpp"                 // AV Class 
#include "../lib/ArduinoThread/Thread.h"  // Thread-s
#include "OnkyoController.cpp"            // Onkyo Controller
// ------------------------------------------------------------------------------------------------------------------------------------------------
// Create base class: Onkyo TX-NR509
OnkyoNR509 avr(
          PIN_AV_POWERD,
          PIN_AV_VH_CON,
          PIN_VFD_RST,
          PIN_VFD_CS,
          PIN_VFD_CLK,
          PIN_VFD_SDO,
          PIN_LED_PURE,
          PIN_LED_THRU,
          PIN_PAN_KEY_0,
          PIN_PAN_KEY_1,
          PIN_PAN_KEY_2,
          PIN_PAN_KEY_3,
          PIN_ENCODER_VOL_A,
          PIN_ENCODER_VOL_B,
          PIN_IR_RECV
);
OnkyoController control(&avr);
// ------------------------------------------------------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(9600);
  delay(1000);  // Sleeper for setup serial
  Serial.println("Run setup.k..\n-------------------------------");
  pinMode(LED_BUILTIN, OUTPUT);

  //  OnkyoNR509 Setup -------------------------
  control.setup();
  delay(100);  // Need sleep for see ...

}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
  Serial.println("Run se");
}