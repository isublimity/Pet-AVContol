// 16-BT-165GINK
// More info:
// [dengjiangcw63945] https://www.programmerall.com/article/701222027/
// LiquidCrystal      https://github.com/arduino-libraries/LiquidCrystal/blob/master/src/LiquidCrystal.cpp
// FutabaVFD162S      https://github.com/Andy4495/FutabaVFD162S/blob/main/src/FutabaVFD162S.cpp
// M162SD12AA         https://docs.rs-online.com/8fe1/0900766b80d12c94.pdf
#ifndef byte
#include <Arduino.h>
#endif
// ------------------------------------------------
#ifndef VFD16BT_H
#define VFD16BT_H

#define VOLCODE_ERROR(data) _CODEPACK(data, 0, 36, 0, 0, 8)
#define VOLCODE_MINUS(data) _CODEPACK(data, 0, 0, 0, 64, 0)
#define VOLCODE_PLUS(data) _CODEPACK(data, 0, 0, 64, 64, 0)
#define VOLCODE_1___(data) _CODEPACK(data, 0, 64, 0, 0, 0)
#define VOLCODE_DOT(data) _CODEPACK(data, 0, 8, 0, 0, 0)
#define VOLCODE_0__(data) _CODEPACK(data, 96, 0, 32, 32, 48)
#define VOLCODE_9__(data) _CODEPACK(data, 96, 32, 0, 32, 48)
#define VOLCODE_8__(data) _CODEPACK(data, 96, 32, 32, 32, 48)
#define VOLCODE_7__(data) _CODEPACK(data, 64, 0, 0, 32, 16)
#define VOLCODE_6__(data) _CODEPACK(data, 96, 32, 32, 0, 48)
#define VOLCODE_5__(data) _CODEPACK(data, 96, 32, 0, 0, 48)
#define VOLCODE_4__(data) _CODEPACK(data, 0, 32, 0, 32, 48)
#define VOLCODE_3__(data) _CODEPACK(data, 96, 32, 0, 32, 16)
#define VOLCODE_2__(data) _CODEPACK(data, 96, 32, 32, 32, 0)
#define VOLCODE_1__(data) _CODEPACK(data, 0, 0, 0, 32, 16)
#define VOLCODE__0_(data) _CODEPACK(data, 16, 16, 24, 24, 0)
#define VOLCODE__9_(data) _CODEPACK(data, 0, 16, 24, 24, 8)
#define VOLCODE__8_(data) _CODEPACK(data, 16, 16, 24, 24, 8)
#define VOLCODE__7_(data) _CODEPACK(data, 0, 16, 8, 16, 0)
#define VOLCODE__6_(data) _CODEPACK(data, 16, 0, 24, 24, 8)
#define VOLCODE__5_(data) _CODEPACK(data, 0, 0, 24, 24, 8)
#define VOLCODE__4_(data) _CODEPACK(data, 0, 16, 24, 0, 8)
#define VOLCODE__3_(data) _CODEPACK(data, 0, 16, 8, 24, 8)
#define VOLCODE__2_(data) _CODEPACK(data, 16, 16, 0, 24, 8)
#define VOLCODE__1_(data) _CODEPACK(data, 0, 16, 8, 0, 0)
#define VOLCODE___0(data) _CODEPACK(data, 12, 0, 4, 4, 6)
#define VOLCODE___9(data) _CODEPACK(data, 12, 4, 0, 4, 6)
#define VOLCODE___8(data) _CODEPACK(data, 12, 4, 4, 4, 6)
#define VOLCODE___7(data) _CODEPACK(data, 8, 0, 0, 4, 2)
#define VOLCODE___6(data) _CODEPACK(data, 12, 4, 4, 0, 6)
#define VOLCODE___5(data) _CODEPACK(data, 12, 4, 0, 0, 6)
#define VOLCODE___4(data) _CODEPACK(data, 0, 4, 0, 4, 6)
#define VOLCODE___3(data) _CODEPACK(data, 12, 4, 0, 4, 2)
#define VOLCODE___2(data) _CODEPACK(data, 12, 4, 4, 4, 0)
#define VOLCODE___1(data) _CODEPACK(data, 0, 0, 0, 4, 2)




#define ARRAY_SIZE(array) ((sizeof(array)) / (sizeof(array[0])))
#define sendBytesM(e) sendBytes(e, ARRAY_SIZE(e))
// 16-BT-165GINK
class VFD16BT {
public:
  VFD16BT(byte _reset, byte _cs, byte _clock, byte _data) {
    _pin_reset = _reset;
    _pin_cs = _cs;
    _pin_clock = _clock;
    _pin_data = _data;
    // Create
  }
  ~VFD16BT() {
    // Drop
  }


  //
  void setup() {
    // Pins
    setupPins();
    timingDelay(50);  //
    // Reset
    reset();
    // reset();
    // ----
    // Setup commands

    // 1.Set DISPLAY TIMING
    command(0xE0, 0x0F);  // 0F = 15 Grid

    // 2.Dim
    dimming(150);

    // 3.???????? WTF ???
    // 0xA0 => 160 -- 10100000 -- 00000101
    // 0xE0 => 224 -- 11100000 -- 00000111
    command(0xA0, 0xE0, 0xE0);

    // 4.Gray ???
    grayLevel();

    // Ram Sigment
    cleanAllSigment();

    print();


    // Apply
    apply();
  }

  // the_URAM ???? GrayLevel?
  void grayLevel(byte val = 0x03) {
    byte cmdGrayLevel[17];
    cmdGrayLevel[0] = 0xC0;                               // command =  192 -- 11000000 -- 00000011
    for (byte i = 0; i < 17; i++) cmdGrayLevel[i] = val;  // data = 3 -- 00000011 -- 11000000
    sendBytesM(cmdGrayLevel);
  }


  void printPrivet(byte i6 = 0, byte i5 = 0) {
    line2SetChar(14, 'P');
    line2SetChar(13, 'r');
    line2SetChar(12, 'i');
    line2SetChar(11, 'v');
    line2SetChar(10, 'e');
    line2SetChar(9, 't');
    line2SetChar(8, '!');
    line1SetChar(7, 0x00);
    line1SetChar(6, i6);
    line1SetChar(5, i5);
    line2SetChar(5, 0x04);
    line2SetChar(4, 0x00);
    line2SetChar(0, 0x04);
    line1SetChar(0, 0x04);
    line2SetChar(15, 0x04);
    line1SetChar(15, 0x04);
    print();
  }

  // --------
  void line1SetChar(byte adr, byte set) {
    if (adr>0 && adr <= ARRAY_SIZE(LINE1)) {
      LINE1[adr] = set;
    }
  }
  void line2SetChar(byte adr, byte set) {
    if (adr>0 && adr <= ARRAY_SIZE(LINE2)) {
      LINE2[adr] = set;
    }
  }

  void print() {
    sendBytes(LINE1, ARRAY_SIZE(LINE1), 0x20);
    sendBytes(LINE2, ARRAY_SIZE(LINE2), 0x00);
    apply();
  }
  //
  void apply() {
    command(0xE8);  // 232 -- 11101000 -- 00010111
  }

  // Set the dimming data
  void dimming(byte dim) {
    command(0xE4, dim);
  }
  
  void reset() {
    digitalWrite(_pin_clock, HIGH);
    timingDelay(RESET_HOLD_TIME);  // 10 MS
    digitalWrite(_pin_reset, LOW);
    timingDelay(RESET_HOLD_TIME);  // 10 MS
    digitalWrite(_pin_reset, HIGH);
    delay(RESET_HOLD_TIME);
    digitalWrite(_pin_reset, LOW);
    delay(RESET_HOLD_TIME);
    digitalWrite(_pin_reset, HIGH);
    delay(100);
    return;
  }


  void cleanVolumeSigment() {
    byte pack[5] = { 0, 0, 0, 0, 0 };
    sendBytes(pack, 5, 0x42);  // VOL
  }
  void setVolumeSigment(float val) {
    byte pack[5] = { 0, 0, 0, 0, 0 };
    _encodeVolumeSegment(pack, val, true);
    sendBytes(pack, 5, 0x42);  // VOL
  }

  void printStrings(String stringOne = "", String stringTwo = "") {
    _setStringToLine(0, stringOne);
    _setStringToLine(1, stringTwo);
    print();
  }


  //
  void command(byte cmd1) {
    byte c3[1] = { cmd1 };
    sendBytes(c3, 1);
  }

  //
  void command(byte cmd1, byte cmd2) {
    byte c2[2] = { cmd1, cmd2 };
    sendBytes(c2, 2);
  }

  //
  void command(byte cmd1, byte cmd2, byte cmd3) {
    byte c1[3] = { cmd1, cmd2, cmd3 };
    sendBytes(c1, 3);
  }
  void cleanAllSigment() {
    byte pack[5] = { 0, 0, 0, 0, 0 };
    sendBytes(pack, 5, 0x41);  //  ram_char 0x01
    sendBytes(pack, 5, 0x42);  //  ram_char 0x02
    sendBytes(pack, 5, 0x44);  //  ram_char 0x04
    sendBytes(pack, 5, 0x43);  //  ram_char 0x03
  }
  //
  void sendBytes(byte cmd[], byte sizeBytes, int appendCommand = -1) {

    cabelSelectStart();
    if (appendCommand >= 0) writeBits(appendCommand);
    for (byte i = 0; i < sizeBytes; i++) {
      writeBits(cmd[i]);
    }
    cabelSelectEnd();
  }
  // CS
  void cabelSelectStart() {
    timingDelay(DATA_SETUP_TIME);
    digitalWrite(_pin_cs, LOW);
  }

  // CS
  void cabelSelectEnd() {
    digitalWrite(_pin_cs, HIGH);
    timingDelay(DATA_SETUP_TIME);
  }
 void delay_us(uint16_t tic_us)
  {
	  tic_us *= 4; //1us = 4 öèêëà
	  __asm__ volatile 
		  (	
			"1: sbiw %0,1" "\n\t" //;
			"brne 1b"				
			: "=w" (tic_us)
			: "0" (tic_us)
		  );
  }
  // -----
  void writeBits(byte bits) {
    byte input = bits;
    delay_us(1);
    // @ToDo: Rewrite for fast? Real need? 
    // Use digitalPinToPort() , portInputRegister() и digitalPinToBitMask()
    // Then like 'D38_High PORTD |=B10000000', 'D38_Low PORTD &=B01111111' ... 
    for (byte i = 0; i < 8; i++) {
      digitalWrite(_pin_clock, LOW);  // CLOCK -> LOW
      digitalWrite(_pin_data, (bits & 0x01) ? HIGH : LOW);
      delay_us(5);
      digitalWrite(_pin_clock, HIGH);
      delay_us(5);
      bits >>= 1;
    }
    delay_us(DATA_SETUP_TIME);  // APPLY times
  }

  // Symbol lists
  typedef struct {  
      bool THX = false;
      bool RH = false;
      bool C = false;
      bool SW = false;
      bool SRB = false;
      bool DolbyChar = false;
      bool Box = false;
      bool LH = false;
      bool FL = false;
      bool SL = false;
      bool SB = false;
      bool DIRECT = false;
      bool A = false;
      bool LW = false;
      bool Audyssey = false;
      bool DolbyChar2 = false;
      bool SBL = false;
      bool BTL = false;
      bool B = false;
      bool Z2 = false;
      bool Z3 = false;
      bool Dynamic = false;
      bool ThueHD = false;
      bool BIAMP = false;
      bool RW = false;
      bool FR = false;
      bool SR = false;
      bool Z4 = false;
      bool DolbyRed = false;

      // S2

      bool MOpt = false;
      bool DTS = false;   
      bool XX = false;     // X Char for neo 
      bool Plus = false;  // Red plus char
      bool II = false;    // Dolby 2 
      bool UP = false;    // Ar.^
      bool AAC = false;
      bool DSD = false;
      bool N6 = false;    // Neo 6
      bool D = false;     // Red 'D' Char
      bool PL = false; 
      bool RIGHT = false; // > 
      bool PCM = false;
      bool EXP = false;  // Red 
      bool NEO = false;  // Neo:
      bool Headp = false;
      bool EX = false;
      bool LEFT = false;
      bool b9624 = false; // RED 96/24
      bool ES = false; // RED 
      bool Vol = false;
      bool Z = false;
      bool PLAY = false;  
      bool DOWN = false;  
      bool HD = false;   // '-HD'
      bool MSTR2 = false;
      bool EQ = false;
      bool X = false;
      bool PAUSE = false;

      // S3
      bool AUTO = false;
      bool SEARCH = false;
      bool NET = false;
      bool db = false;
      bool RDS = false;
      bool TUNED = false;
      bool ANALOG = false;
      bool SPS = false;
      bool DIGITAL = false;
      bool APD = false;
      bool HD2 = false;
      bool HDMI = false;
      bool MUTTING = false;
      bool FMSTEREO = false;
      bool USB = false;
      bool SLEEP = false;
        
  } SymbolsList;


  //
  SymbolsList Symbols;

  long c1Symbols = 0;
  long c2Symbols = 0;
  long c3Symbols = 0;

  long _getCacheKeySymbols(byte b[5]) {
      // hash = (hash + (324723947 + a[i])) ^93485734985;
      return b[0] * 12 + b[1] * 24 + b[2] * 34 + b[4] * 44;
  }

  void applySymbols() {
    byte VSTAB[5];

    long cacheKeySymbols = 0;
    // ------------------------------------------------------------ S1 ------------------------------------------------------------
    // Cacl
    VSTAB[0] = (0x01 * Symbols.THX) + (0x02 * Symbols.RH)   + (0X04 * Symbols.C)   + (0X08 * Symbols.SW)   +   (0x10*Symbols.SRB) +   (0x20*Symbols.DolbyChar );
    VSTAB[1] = (0x01 * Symbols.Box) + (0x02 * Symbols.LH)   + (0X04 * Symbols.FL ) + (0X08 * Symbols.SL)   +   (0x10*Symbols.SB)  +   (0x20*Symbols.DIRECT );
    VSTAB[2] = (0x01 * Symbols.A)   + (0x02 * Symbols.LW)+(0X04 * Symbols.Audyssey )+(0X08 * Symbols.DolbyChar2)+   (0x10*Symbols.SBL)  +   (0x20*Symbols.BTL );
    VSTAB[3] = (0x01 * Symbols.B) + (0x02 * Symbols.Z2) + (0X04 * Symbols.Z3 )+ (0X08 * Symbols.Dynamic)    +   (0x10*Symbols.ThueHD)+   (0x20*Symbols.BIAMP );
    VSTAB[4] = (0x01 * Symbols.RW) + (0x02 * Symbols.FR) + (0X04 * Symbols.SR  )+ (0X08 * Symbols.Z4)    +   (0x10*Symbols.DolbyRed) ;
    // Cache key calc
    cacheKeySymbols = _getCacheKeySymbols(VSTAB);
    if (cacheKeySymbols != c1Symbols) {
      c1Symbols = cacheKeySymbols;
      // Write
      sendBytes(VSTAB, 5, 0x43); 
    }  
    // ------------------------------------------------------------ S2 ------------------------------------------------------------
    // Cacl 
    VSTAB[0] = (0x01 * Symbols.MOpt) + (0x02 * Symbols.DTS) + (0X04 * Symbols.XX)  + (0X08 * Symbols.Plus) +   (0x10*Symbols.II)  +   (0x20*Symbols.UP );
    VSTAB[1] = (0x01 * Symbols.AAC) + (0x02 * Symbols.DSD)  + (0X04 * Symbols.N6 ) + (0X08 * Symbols.D)    +   (0x10*Symbols.PL)  +   (0x20*Symbols.RIGHT );
    VSTAB[2] = (0x01 * Symbols.PCM) + (0x02 * Symbols.EXP)  + (0X04 * Symbols.NEO )+ (0X08 * Symbols.Headp)+   (0x10*Symbols.EX)  +   (0x20*Symbols.LEFT );
    VSTAB[3] = (0x01 * Symbols.b9624) + (0x02 * Symbols.ES) + (0X04 * Symbols.Vol )+ (0X08 * Symbols.Z)    +   (0x10*Symbols.PLAY)+   (0x20*Symbols.DOWN );
    VSTAB[4] = (0x01 * Symbols.HD) + (0x02 * Symbols.MSTR2) + (0X04 * Symbols.EQ  )+ (0X08 * Symbols.X)    +   (0x10*Symbols.PAUSE) +   (0x20*1 );
    // Write
    cacheKeySymbols = _getCacheKeySymbols(VSTAB);
    if (cacheKeySymbols != c2Symbols) {
      c2Symbols = cacheKeySymbols;
      // Write
      sendBytes(VSTAB, 5, 0x44); 
    } 
    // ------------------------------------------------------------ S3 ------------------------------------------------------------
    VSTAB[0] = (0x01 * Symbols.AUTO) + (0x02 * Symbols.SEARCH) + (0X04 * Symbols.NET) + (0X08 * Symbols.db); 
    VSTAB[1] = (0x01 * Symbols.RDS)  + (0x02 * Symbols.TUNED)  + (0X04 * Symbols.ANALOG);                      
    VSTAB[2] = (0x01 * Symbols.SPS)  + (0x02 * Symbols.DIGITAL)+ (0X04 * Symbols.APD);                       
    VSTAB[3] = (0x01 * Symbols.HD2)  + (0x02 * Symbols.HDMI)   + (0X04 * Symbols.MUTTING);                     
    VSTAB[4] = (0x01 * Symbols.FMSTEREO) + (0x02 * Symbols.USB)+ (0X04 * Symbols.SLEEP);                   
    cacheKeySymbols = _getCacheKeySymbols(VSTAB);
    if (cacheKeySymbols != c2Symbols) {
      c2Symbols = cacheKeySymbols;
      // Write
      sendBytes(VSTAB, 5, 0x41); 
    } 
  }

private:
  byte MAX_CHAR = 14;
  int DATA_SETUP_TIME = 5;   // Datasheet specifies 3 us (tsu)
  int RESET_HOLD_TIME = 30;  // Datasheet specifies 30 us (twRST). This parameter seemed to be most sensitive to specfic minimal timing.

  byte LINE1[16] = { 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03 };
  byte LINE2[16] = { 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04 };
  //              _S1_ _01_ _02_ _03_ _04_ _05_ _06_ _07_ _08_ _09_ _10_ _11_ _12_ _13_ _14_ _S2_
  // S1 & S2 special ram char
  // Addr 0x41 for ram_char 0x01 :=> HD,SPS,RDS NET,USB... and db
  // Addr 0x42 for ram_char 0x02 :=> Encode Volume +- dot 188.8
  // Addr 0x43 for ram_char 0x03 :=> PreAmp,TrueHD,Dolby,Dynamic,THX,FL,C,FR...,Z2,Z3,Z4
  // Addr 0x44 for ram_char 0x04 :=> Dtx,dsd,play,pause,Up,Left,Down...

  byte _pin_reset;  // FLDRST - RESET  p19 in P701B
  byte _pin_cs;     // FLDCS  - cabel select (enable_pin) p21 in P701B
  byte _pin_clock;  // FLDCLK - clock pin; CLOCK p23 in P701B
  byte _pin_data;   // FLDSDO - data pin  p22


  void timingDelay(uint16_t delayInMicroseconds) {
    delayMicroseconds(delayInMicroseconds);
  }

  // ,
  void _encodeVolumeSegment(byte *data, float value, bool showPlusMinus) {
    // Value = Max +199.9, Min -199.9 | or -1999 & +1999
    // ---------------------------------
    if (showPlusMinus && value < 0) {  // Apply VOLCODE_MINUS
      VOLCODE_MINUS(data);
    }
    if (showPlusMinus && value > 0) {  // Apply VOLCODE_PLUS
      VOLCODE_PLUS(data);
    }
    if (value > 199.9 || value < -199.9) {  // Apply VOLCODE_ERROR
      VOLCODE_ERROR(data);
      return;
    }
    if (value >= 100 || value <= -100) {  // Apply VOLCODE_1___
      VOLCODE_1___(data);
    }
    VOLCODE_DOT(data);

    int vv = (value * 10.0) * (value < 0 ? -1 : 1);
    int c0 = ((vv / 1000) % 100);  // 147.98 -> 1
    int c1 = ((vv / 100) % 10);    // 147.98 -> 4
    int c2 = ((vv / 10) % 10);     // 147.98 -> 7
    int c3 = (vv % 10);            // 147.98 -> 9

    if (c0 > 1) {  // Apply VOLCODE_1x
      VOLCODE_ERROR(data);
      return;
    }

    if (vv >= 100) {  // Apply VOLCODE_1x
      switch (c1) {
        case 0: VOLCODE_0__(data); break;
        case 1: VOLCODE_1__(data); break;
        case 2: VOLCODE_2__(data); break;
        case 3: VOLCODE_3__(data); break;
        case 4: VOLCODE_4__(data); break;
        case 5: VOLCODE_5__(data); break;
        case 6: VOLCODE_6__(data); break;
        case 7: VOLCODE_7__(data); break;
        case 8: VOLCODE_8__(data); break;
        case 9: VOLCODE_9__(data); break;
      }
    }
    if (vv >= 10) {  // Apply VOLCODE_1x
      switch (c2) {
        case 0: VOLCODE__0_(data); break;
        case 1: VOLCODE__1_(data); break;
        case 2: VOLCODE__2_(data); break;
        case 3: VOLCODE__3_(data); break;
        case 4: VOLCODE__4_(data); break;
        case 5: VOLCODE__5_(data); break;
        case 6: VOLCODE__6_(data); break;
        case 7: VOLCODE__7_(data); break;
        case 8: VOLCODE__8_(data); break;
        case 9: VOLCODE__9_(data); break;
      }
    }
    if (vv >= 1) {  // Apply VOLCODE_1x
      switch (c3) {
        case 0: VOLCODE___0(data); break;
        case 1: VOLCODE___1(data); break;
        case 2: VOLCODE___2(data); break;
        case 3: VOLCODE___3(data); break;
        case 4: VOLCODE___4(data); break;
        case 5: VOLCODE___5(data); break;
        case 6: VOLCODE___6(data); break;
        case 7: VOLCODE___7(data); break;
        case 8: VOLCODE___8(data); break;
        case 9: VOLCODE___9(data); break;
      }
    }
    // printHexBin(data,5);
  }
  // ------------------------------------------------------------------------------------------------------

  void _CODEPACK(byte *data, byte a0, byte a1, byte a2, byte a3, byte a4) {
    // apply bit mask
    data[0] |= a0;
    data[1] |= a1;
    data[2] |= a2;
    data[3] |= a3;
    data[4] |= a4;
  }

  void _setStringToLine(byte line, String str) {
    for (byte position = 0; position < 14; position++) {
      byte chr = str.length() >= position ? str.charAt(position) : 0x00;
      if (line == 0) {
        line1SetChar(MAX_CHAR - position, chr);
      } else {
        line2SetChar(MAX_CHAR - position, chr);
      }
    }
  }
  void setPinsDefault() {
    digitalWrite(_pin_data, LOW);
    digitalWrite(_pin_reset, HIGH);
    digitalWrite(_pin_clock, LOW);
    digitalWrite(_pin_cs, LOW);
  }


  void setupPins() {
    setPinsDefault();
    pinMode(_pin_data, OUTPUT);
    pinMode(_pin_reset, OUTPUT);
    pinMode(_pin_clock, OUTPUT);
    pinMode(_pin_cs, OUTPUT);
    setPinsDefault();
  }
};
// 8 - UP Arroy
// 9 Down
// 10 Left Ar
// 11 Rign Ar
// 13 OK
// 14 Up Ar B
// 15 Left
// 16 || - pause
// 18 19 Dobly
// 20 - II
// 21 - Om
// 22 - <

// 23 UpDown
// 24 - DownBackArroy
// 25 BackUp
// 26 > Play
// 27    Double
// 28 >> Double
// 29 << Double
// 31 UpDown?

// 127 -BACLK
// 128 -
// 129 Disk
// 131 - Done
// 133 Folder
// 135 - Play
// 137 - ON
// 139 Warn
// 142 IPOD
// 146 Warn
// 149 Question
// 150 Lock
// 153 - 154 Heard
// 157 - Tr
// 159 Clock
//

#endif  // VFD16BT