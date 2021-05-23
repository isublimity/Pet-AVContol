// ------------------------------------------------------------------------------------------------------------------------------------------------
#include "Arduino.h"
// ------------------------------------------------------------------------------------------------------------------------------------------------
#include "Settings.h"                              // Settings pin
#include "VFD16BT.cpp"                             // VDF Class
#include "OnkyoNR509.cpp"                          // AV Class
#include "OnkyoController.cpp"                     // Onkyo Controller
#include "../lib/ArduinoThread/Thread.h"           // Thread-s
#include "../lib/ArduinoThread/ThreadController.h" // Thread-s
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
    PIN_IR_RECV);
OnkyoController control(&avr);
// ------------------------------------------ Interrupts ------------------------------------------------------------------------------------------------------
void __InterruptEncoderTick()
{
  control.encoderInterrupt();
}

// ------------------------------------------ Threads ---------------------------------------------------------------------------------------------------------
Thread *t_buttonsScan;
Thread *t_vfdRender;
Thread *t_panicScan;
Thread *t_IRScan;
Thread *t_messageBus;
Thread *t_ledBlinks;
ThreadController groupOfThreads;

void _threadLedBlinksCallback()
{
  static bool ledBUILTINStatus = false;
  ledBUILTINStatus = !ledBUILTINStatus;
  digitalWrite(LED_BUILTIN, ledBUILTINStatus);
  control.ledBlinks();
}
void _threadButtonScan()
{
  control.buttonScan();
}
void _threadVfdRender()
{
  control.render();
}
void _threadIRScan()
{
  control.IRScan();
}
void setupThreads()
{

  t_vfdRender = new Thread();
  t_vfdRender->onRun(_threadVfdRender);
  t_vfdRender->setInterval(50);

  t_IRScan = new Thread();
  t_IRScan->onRun(_threadIRScan);
  t_IRScan->setInterval(10);

  t_buttonsScan = new Thread();
  t_buttonsScan->onRun(_threadButtonScan);
  t_buttonsScan->setInterval(50);

  t_ledBlinks = new Thread();
  t_ledBlinks->onRun(_threadLedBlinksCallback);
  t_ledBlinks->setInterval(1500);

  groupOfThreads = ThreadController();
  groupOfThreads.add(t_ledBlinks);
  groupOfThreads.add(t_vfdRender);
  groupOfThreads.add(t_IRScan);
  groupOfThreads.add(t_buttonsScan);
  //
}
// ------------------------------------------ Setup  ----------------------------------------------------------------------------------------------------------
void setup()
{
  // 30 PIN - SPRL_F
  // 28 PIN - SEC1_H
  // 26 PIN - SEC1_OFF
  digitalWrite(30, LOW);
  pinMode(30, OUTPUT);
  digitalWrite(28, LOW);
  pinMode(28, OUTPUT);
  digitalWrite(26, LOW);
  pinMode(26, OUTPUT);

  // 18 -> 2 VOL_DATA
  // 16 -> 5 VOL_CLOCK
  digitalWrite(18, LOW);
  pinMode(18, OUTPUT);
  digitalWrite(16, LOW);
  pinMode(16, OUTPUT);


  
  Serial.begin(9600);
  delay(1000); // Sleeper for setup serial
  Serial.println("Run x setup.k..\n-------------------------------");
  pinMode(LED_BUILTIN, OUTPUT);

  //  Onkyo NR509 Setup -------------------------
  control.setup();
  // attach Interrupt
  if (control.isVolumeEncoderSupportInterrupt())
  {
    attachInterrupt(0, __InterruptEncoderTick, CHANGE);
  }

  delay(100); // Need sleep for see ...
  // Last setup attach threads & run
  setupThreads();
}

void loop()
{
  // run ThreadController
	// this will check every thread inside ThreadController,
  groupOfThreads.run();
}