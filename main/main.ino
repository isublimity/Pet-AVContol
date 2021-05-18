// ------------------------------------------------------------------------------------------------------------------------------------------------


#include "Settings.h"                     // Settings pin
#include "VFD16BT.cpp"                    // VDF Class 
#include "OnkyoNR509.cpp"                 // AV Class 
#include "libs/ArduinoThread/Thread.h"  // Thread-s
#include "OnkyoController.cpp"            // Onkyo Controller
// # i nclude <ArduinoJson.h>                  // Json parse




// ------------------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------------------

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
// OnkyoController control(&avr);

// Thread* buttonsThr   = new Thread();
// Thread* vfdUpdateThr = new Thread();
// Thread* panicReadThr = new Thread();
// Thread* irReadThr    = new Thread();
// Thread* ledBlinksThr = new Thread();
// Thread* serialThr    = new Thread();
// ThreadController groupOfThreads = ThreadController();
// -------------------------------------------------------- SETUP ---------------------------------------------------------------------------------------
void setup() {
  // LED internal
  pinMode(LED_BUILTIN, OUTPUT);

  delay(1000);  // Sleeper for setup serial
  Serial.begin(19200);
  Serial2.begin(19200);
  // Serial.setTimeout(2000);
  // Serial2.setTimeout(200);
  delay(1000);  // Sleeper for setup serial
  // Done 
  Serial.println("Run setup.k..\n-------------------------------");
  // ------- Main setup

  //  OnkyoNR509 Setup -------------------------
  // control.setup();
  delay(100);  // Need sleep for see ...
  // 
//  if (PIN_ENCODER_VOL_A == 2 || PIN_ENCODER_VOL_B == 2) {
//        attachInterrupt(0, encoderTick, CHANGE);
//  }
  // IR...
  setupExternal();
  // Tests...
  setupTestCode();
  // Threads
  setupThreads();

  
  digitalWrite(LED_BUILTIN , HIGH);
  delay(1000);
  Serial.println("Setup done");
  return;
}

// -------------------- Threads ------------------------------
void setupThreads() {
  // Configure myThread
	// buttonsThr.onRun(niceCallback);
	// buttonsThr.setInterval(500);
}


// ---------------------------------------------
void setupExternal() {

  

}
// ---------------------------------------------
void setupTestCode() {

  pinMode(10, OUTPUT);
  digitalWrite(10 , LOW);  
  // Test code 
  // pinMode(PIN_BASE_ANA, INPUT);
  // digitalWrite(PIN_BASE_ANA,LOW);
}


void printHexBin(byte *data, byte length) {

  char tmp[16];
  for (int i = 0; i < length; i++) {

    sprintf(tmp, "0x%.2X", data[i]);

    Serial.print(tmp);
    Serial.print("\t[");
    Serial.print(data[i]);
    Serial.print("]\t");
    //for(int v = 7; v>=0;v--) {
    //    Serial.print((char)('0' + ((c>>v)&1)));
    //}
  }
  Serial.println();
}

void readRemouteCommand() {

  if (Serial.available()) {
    avr.LightThruLed(true);
    avr.LightPureLed(true);

    // StaticJsonDocument<500> doc;
    // DeserializationError err = deserializeJson(doc, Serial);
//    if (err == DeserializationError::Ok) {
//      Serial.println("JSON OK");
//      // {"cmd":"vfdSet","addr":41,"b0":12,"b1":15,"b2":23,"b3":0,"b4":3}
//      // {"cmd":"vfdSet","addr":41,"b0":12,"b1":15,"b2":23,"b3":00}
//      String command = doc["cmd"].as<String>();
//      Serial.println(command);
//      if (command.equals("vfdSet")) {
//        // vfdSetChar(doc);
//      }
//    } else {
//      Serial.print("deserializeJson() returned ");
//      Serial.println(err.c_str());
//      // Flush all bytes in the "link" serial port buffer
//      // while (Serial.available() > 0)
//      //  Serial.read();
//    }
    avr.LightThruLed(false);
    avr.LightPureLed(false);
  }
}

// ----------------------------------- LOOP -------------------------------------------------------------------
String nom = "Arduino";
String msg;
void readSerialPort() {
  msg = "";
  if (Serial2.available()) {
    delay(10);
    while (Serial2.available() > 0) {
      digitalWrite(LED_BUILTIN , LOW);
      msg += (char)Serial.read();
      Serial.print(msg);
      digitalWrite(LED_BUILTIN , HIGH);
    }
    Serial2.flush();
  }
}

void sendData() {
  //write data
  Serial2.print(nom);
  Serial2.print(" received : ");
  Serial2.print(msg);
}

void loop() {

  // digitalWrite(LED_BUILTIN , HIGH);
  // digitalWrite(10 , HIGH);
  // Serial.print("r>");
  // readSerialPort();
  // Serial.print("+");
  // if (msg != "") {
    // sendData();
    // Serial.println(msg);
    // digitalWrite(LED_BUILTIN , LOW);
  // }



  // delay(2500);
  // digitalWrite(LED_BUILTIN , LOW);
  // digitalWrite(10 , LOW);
  // delay(2500);
  // avr.vfd.Symbols.DOWN = false;
  // avr.vfd.Symbols.PLAY  = true;
  // avr.vfd.Symbols.SLEEP = true;
  // avr.vfd.applySymbols();
  // avr.readEncoder();


/*
  // IR .... 
  if ( IRReciver.decode(  )) { // если данные пришли
    IRReciver.printIRResultShort(&Serial);
    Serial.println( IRReciver.decodedIRData.address, HEX ); // печатаем данные
    Serial.println( IRReciver.decodedIRData.command, HEX ); // печатаем данные
    avr.vfd.printStrings(String(avr.getVolume())+" <- VOL ",String(IRReciver.decodedIRData.command));
    avr.renderVolume();
    IRReciver.resume(); // принимаем следующую команду
  }
  // delay(52);
  // 
*/
  //delay(52);
  return;
}

/**
 * 
 * ToDo read: 
 * - ArduinoThread
 * - TaskManagerIO scheduling and event based library for Arudino and mbed https://github.com/davetcc/TaskManagerIO
 *  
 * 
 * 
 * 
 * interrupts: 
 *  - https://alexgyver.ru/lessons/interrupts/
 *  - https://github.com/NicoHood/PinChangeInterrupt
 *  - https://tsibrov.blogspot.com/2019/06/arduino-interrupts-part2.html
*/
