#include "VFD16BT.cpp"
#include "OnkyoNR509.cpp"
#include <ArduinoJson.h>
// ArduinoThread
// ------------------------------------------------------------------------------------------------------------------------------------------------
// Led
byte PIN_LED_THRU = 39;
byte PIN_LED_PURE = 37;

// Volume encoder
byte PIN_ENC_VOLA = 2;
byte PIN_ENC_VOLB = 4;

// Keys
byte PIN_PAN_KEY_0 = 55;
byte PIN_PAN_KEY_1 = 54;
byte PIN_PAN_KEY_2 = 56;
byte PIN_PAN_KEY_3 = 57;
byte PIN_BASE_ANA  = 58;
// ---------- VFD 16-BT-165GINK
byte PIN_VFD_RST = 43;  // FLDRST - RESET
byte PIN_VFD_CS = 45;   // FLDCS  - cabel select (enable_pin)
byte PIN_VFD_CLK = 47;  // FLDCLK - clock pin; CLOCK
byte PIN_VFD_SDO = 41;  // FLDSDO - data pin

// Control pins
byte PIN_AV_POWERD = 46;  // POWERD,Pin 0 in JL901A
byte PIN_AV_VH_CON = 44;  // Power VFD + panel +32V DISPLAY SECTION PART9;  P701B VH_CON1=4 ; MinePin=22
// ------------------------------------------------------------------------------------------------------------------------------------------------
struct PIN_SETS pinList;
// pinList.POWERD = (byte) 123;
// pinList.VH_CON = 12;
const char compile_date[] = __DATE__ " " __TIME__;

// ------------------------------------------------------------------------------------------------------------------------------------------------
// Create Onkyo TX-NR509
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
          PIN_ENC_VOLA,
          PIN_ENC_VOLB
);

// ------------------------------------------------------------------------------------------------------------------------------------------------

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);  // Sleeper for run
  Serial.begin(9600);
  Serial.setTimeout(2000);
  delay(1000);  // Sleeper for run
  Serial.println("Run setup.k..\n-------------------------------");
  // OnkyoNR509 Setup
  avr.setup();
  avr.setVolume(-50);
  // avr.LightThruLed(true);
  // avr.vfd.printStrings("Privet V0.1", compile_date);
  delay(100);  // Need sleep


  
  pinMode(PIN_BASE_ANA, INPUT);
  digitalWrite(PIN_BASE_ANA,LOW);

  if (PIN_ENC_VOLA == 2 || PIN_ENC_VOLB == 2) {
          attachInterrupt(0, encoderTick, CHANGE);
  }
        

  return;
}
void encoderTick() {
  avr.volumeEncoderTick();
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
  int start = millis() ;
  int count = 1;
  int sensorValue0,sensorValue1,sensorValue2,sensorValue3;
  for (int i = 0 ; i < count ; i++) {
      
      // 
      avr.ButtonsRead();
  }
  // avr.vfd.printStrings("DONE","OK!");

  int msec= millis() - start;
// Serial.print() ;
// Serial.println(" msec (30000 calls)") ;

  
  // float voltage = sensorValue * (5.0 / 1023.0);
  
  //delay(5250);                       // wait for a second
  int V10=avr._lastButtonValue0;
  int V20=analogRead(PIN_PAN_KEY_0);
  int V11=avr._lastButtonValue1;
  int V21=analogRead(PIN_PAN_KEY_1);
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
  avr.vfd.printStrings(String(avr.getVolume())+" <- VOL ","");
  avr.renderVolume();
  delay(50);  

  avr.vfd.Symbols.DOWN  = true;
  // avr.vfd.Symbols.PLAY  = false;
  avr.vfd.Symbols.SLEEP = false;
  // avr.vfd.applySymbols();             
  // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(5);       
 
  
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
