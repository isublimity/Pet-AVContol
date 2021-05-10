// ------------------------------------------------------------------------------------------------------------------------------------------------
#include "Settings.h"       // Settings pin
#include "VFD16BT.cpp"      // VDF Class 
#include "OnkyoNR509.cpp"   // AV Class 
#include <ArduinoJson.h>    // Json parse 
#include "IRremote.h"       // IR 
// ------------------------------------------------------------------------------------------------------------------------------------------------
// TODO:
// - ArduinoThread
// - TaskManagerIO scheduling and event based library for Arudino and mbed
// ------------------------------------------------------------------------------------------------------------------------------------------------
// Other settings: 
#define DECODE_RC6 // For IRremote
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
          PIN_ENCODER_VOL_B
);
// IR from IRremote
IRrecv IRReciver(PIN_IR_RECV);
// -------------------------------------------------------- SETUP ---------------------------------------------------------------------------------------
void setup() {
  // LED internal
  pinMode(LED_BUILTIN, OUTPUT);

  delay(1000);  // Sleeper for setup serial
  Serial.begin(9600);
  Serial.setTimeout(2000);
  delay(1000);  // Sleeper for setup serial
  // Done 
  Serial.println("Run setup.k..\n-------------------------------");
  // ------- Main setup
 
  // avr 
  setupAV();
  delay(100);  // Need sleep for see ...
  // 
  if (PIN_ENCODER_VOL_A == 2 || PIN_ENCODER_VOL_B == 2) {
        attachInterrupt(0, encoderTick, CHANGE);
  }
  // IR...
  setupExternal();
  // Tests...
  setupTestCode();
  return;
}
// -------------------- Interrupt\s -------------------------
void encoderTick() {
  avr.volumeEncoderTick();
}
// --------------------  OnkyoNR509 Setup -------------------------
void setupAV() {
  avr.setup();
  avr.setVolume(-50);
  avr.LightThruLed(true);
  // Debug on start 
  const char compile_date[] = __DATE__ " " __TIME__; // For dump 
  avr.vfd.printStrings("Privet V0.1", compile_date);
}

// ---------------------------------------------
void setupExternal() {

  IRReciver.enableIRIn();
}
// ---------------------------------------------
void setupTestCode() {

  
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
void vfdSetChar(StaticJsonDocument<500> doc) {

  const byte addr = doc["addr"].as<byte>();

  if (!addr) {
    Serial.println("net set addr");
    return;
  }
  Serial.println("vfdSetChar > ");
  byte pack[5];
  pack[0] = doc["b0"].as<byte>();
  pack[1] = doc["b1"].as<byte>();
  pack[2] = doc["b2"].as<byte>();
  pack[3] = doc["b3"].as<byte>();
  pack[4] = doc["b4"].as<byte>();


  avr.vfd.line2SetChar(3, 1);
  avr.vfd.line2SetChar(4, 2);
  avr.vfd.line2SetChar(4, 3);  // Char=4 IN ADDR = 0x43 // Encode Volume
  avr.vfd.line2SetChar(5, 4);  // Char=4 IN ADDR = 0x43+1
  avr.vfd.line2SetChar(6, 5);  // Char=5 IN ADDR = 0x43+2


  // debug
  printHexBin(pack, 5);

  avr.vfd.sendBytes(pack, 5, addr);
  avr.vfd.apply();
  delay(20);
  // Serial.println(addr);
  // Serial.print("b0 = ");
  // Serial.println(doc["b0"].as<byte>());
  // Serial.print("b1 = ");
  // Serial.println(doc["b1"].as<byte>());
}


// ------------------------------------------------------------------------------------------------------


void loop() {

  if (Serial.available()) {
    avr.LightThruLed(true);
    avr.LightPureLed(true);

    StaticJsonDocument<500> doc;

    // Read the JSON document from the "link" serial port
    DeserializationError err = deserializeJson(doc, Serial);

    if (err == DeserializationError::Ok) {
      Serial.println("JSON OK");
      // {"cmd":"vfdSet","addr":41,"b0":12,"b1":15,"b2":23,"b3":0,"b4":3}
      // {"cmd":"vfdSet","addr":41,"b0":12,"b1":15,"b2":23,"b3":00}
      // Print the values
      // (we must use as<T>() to resolve the ambiguity)

      String command = doc["cmd"].as<String>();
      Serial.println(command);
      if (command.equals("vfdSet")) {

        vfdSetChar(doc);
      }

    } else {
      // Print error to the "debug" serial port
      Serial.print("deserializeJson() returned ");
      Serial.println(err.c_str());

      // Flush all bytes in the "link" serial port buffer
      // while (Serial.available() > 0)
      //  Serial.read();
    }
    avr.LightThruLed(false);
    avr.LightPureLed(false);
  }
  // ToDo: Rewrite to fast read button.s
  // avr.vfd.printStrings("GO...","...");
  // int start = millis() ;
  //  int msec= millis() - start;

  //delay(5250);                       // wait for a second
  //  int V10=avr._lastButtonValue0;
  //  int V20=analogRead(PIN_PAN_KEY_0);
  //  int V11=avr._lastButtonValue1;
  //  int V21=analogRead(PIN_PAN_KEY_1);
  // -----
 
  avr.vfd.Symbols.DOWN = false;
  // avr.vfd.Symbols.PLAY  = true;
  avr.vfd.Symbols.SLEEP = true;
  // avr.vfd.applySymbols();

  //int zxx = 1 + random(0, 1999);
  // int min = random(0, 100);
  // float randNumber = zxx / 10.0;
  // if (min > 50) randNumber = randNumber * -1;
  // avr.vfd.setVolumeSigment(randNumber);
  // avr.vfd.printStrings(String(sensorValue1)+":"+String(sensorValue0),String(sensorValue2)+","+String(sensorValue3));
  // double perOne =  ceil((msec * count) - 0.5) / count; // ( msec / count );
  // avr.readEncoder();
  // int ss=digitalRead(PIN_ENC_VOLA);
  // int ssB=digitalRead(PIN_ENC_VOLB);
  // int statusSensor = digitalRead (3);
  if ( IRReciver.decode(  )) { // если данные пришли

    
    IRReciver.printIRResultShort(&Serial);
  
    Serial.println( IRReciver.decodedIRData.address, HEX ); // печатаем данные
    Serial.println( IRReciver.decodedIRData.command, HEX ); // печатаем данные
    avr.vfd.printStrings(String(avr.getVolume())+" <- VOL ",String(IRReciver.decodedIRData.command));
    avr.renderVolume();
    IRReciver.resume(); // принимаем следующую команду
    
  }
  
  
  // delay(50);  

  // avr.vfd.Symbols.DOWN  = true;
  // avr.vfd.Symbols.PLAY  = false;
  //avr.vfd.Symbols.SLEEP = false;
  // avr.vfd.applySymbols();             
  // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  //delay(5);       
 
  
  //randsD();
  
  return;
}
/**
 * TMARK    Triangle Mark, That only applies to line 2
 *   |      □ □ □ □ □ □ . . . □ □ □ □ □ □
 *   |      □ □ □ □ □ □ . . . □ □ □ □ □ □
 *   +--->  ▼ ▼ ▼ ▼ ▼ ▼ . . . ▼ ▼ ▼ ▼ ▼ ▼
 * 
 * interrupts: 
 *  - https://alexgyver.ru/lessons/interrupts/
 *  - https://github.com/NicoHood/PinChangeInterrupt
 *  - https://tsibrov.blogspot.com/2019/06/arduino-interrupts-part2.html
*/
