#ifndef byte
#include <Arduino.h>
#endif

#include "VFD16BT.cpp"

#define VOLUME_ENCODER_MAX 10
#define VOLUME_ENCODER_MIN -70
#define VOLUME_ENCODER_STEP 0.5

#ifndef GyverEncoder_h
#include "GyverLibs/GyverEncoder/GyverEncoder.h"
#endif

#ifndef OnkyoNR509_H
#define OnkyoNR509_H
// Onkyo TX-NR509
// namespace Onkyo;
struct PIN_SETS
{
  byte POWERD = 0; // POWERD,Pin 0 in JL901A
  byte VH_CON = 0; // Trigger for KRC107S IN AV [VH_CON1/LEDSTBY] 0 - Off,1 - Power 40V
  byte LED_PURE = 0;
  byte LED_THRU = 0;

  byte KEY0 = 0;
  byte KEY1 = 0;
  byte KEY2 = 0;
  byte KEY3 = 0;

  byte VOLA = 0;
  byte VOLB = 0;
  // 16-BT-165GINK
  byte VFD_RST = 0;
  byte VFD_CS = 0;
  byte VFD_CLK = 0;
  byte VFD_SDO = 0;
};

class OnkyoNR509
{
public:
  // Create VFD class 16-BT-165GINK
  VFD16BT vfd;
  // ---- CREATE
  OnkyoNR509(byte &_POWERD,
             byte &_VH_CON,
             byte &_VFD_RST,
             byte &_VFD_CS,
             byte &_VFD_CLK,
             byte &_VFD_SDO,
             byte &_LED_PURE,
             byte &_LED_THRU,
             byte &_KEY_0,
             byte &_KEY_1,
             byte &_KEY_2,
             byte &_KEY_3,
             byte &_VOL_A,
             byte &_VOL_B) : vfd(_VFD_RST, _VFD_CS, _VFD_CLK, _VFD_SDO)
  {
    // Control
    pins.POWERD = _POWERD;
    pins.VH_CON = _VH_CON;
    // Display
    pins.VFD_RST = _VFD_RST;
    pins.VFD_CS = _VFD_CS;
    pins.VFD_CLK = _VFD_CLK;
    pins.VFD_SDO = _VFD_SDO;
    // Leds
    pins.LED_PURE = _LED_PURE;
    pins.LED_THRU = _LED_THRU;
    // Buttons
    pins.KEY0 = _KEY_0;
    pins.KEY1 = _KEY_1;
    pins.KEY2 = _KEY_2;
    pins.KEY3 = _KEY_3;
    // Vol
    pins.VOLA = _VOL_A;
    pins.VOLB = _VOL_B;

    //
  }
  volatile float VolumeValue = 0 ;
  volatile float _lastRenderVolume = 0 ;
  volatile int encCounter;
  volatile boolean encoderState0, encoderlastState;

  float getVolume() {
    return VolumeValue;
  }
  void setVolume(float value) {
      if (value > VOLUME_ENCODER_MAX ) value = VOLUME_ENCODER_MAX;
      if (value < VOLUME_ENCODER_MIN ) value = VOLUME_ENCODER_MIN;
      VolumeValue = value;
  }
  void volumeEncoderTick()
  {
    // @ToDo - rewrite digitalReadFast(p);

    encoderState0 = digitalRead(pins.VOLA);
    if (encoderState0 != encoderlastState)
    {
      // encCounter += 
      float val = (digitalRead(pins.VOLB) != encoderlastState) ? (-1 * VOLUME_ENCODER_STEP) : VOLUME_ENCODER_STEP;
      setVolume(VolumeValue+val);

      encoderlastState = encoderState0;
    }
  }

  void renderVolume(bool force=false ) {
      if (_lastRenderVolume != VolumeValue || force) {
        // Value change 
        vfd.setVolumeSigment(VolumeValue);
        _lastRenderVolume = VolumeValue;
      }
  }


  volatile int buttonState = 0;

  int inline analogReadFast(byte ADCpin)
  {

    byte ADCSRAoriginal = ADCSRA;
    ADCSRA = (ADCSRA & B11111000) | 4;
    int adc = analogRead(ADCpin);
    ADCSRA = ADCSRAoriginal;
    return adc;
  }

  int _lastButtonValue0, _lastButtonValue1, _lastButtonValue2, _lastButtonValue3;

  void ButtonsRead()
  {
    //
    byte ADCSRAoriginal = ADCSRA;
    ADCSRA = (ADCSRA & B11111000) | 4;
    _lastButtonValue0 = analogRead(pins.KEY0);
    _lastButtonValue1 = analogRead(pins.KEY1);
    _lastButtonValue2 = analogRead(pins.KEY2);
    _lastButtonValue3 = analogRead(pins.KEY3);
    ADCSRA = ADCSRAoriginal;
    // if > 1000 => no press
  }

  void PowerOn()
  {
    digitalWrite(pins.POWERD, HIGH);
  }

  void PowerOff()
  {
    digitalWrite(pins.POWERD, LOW);
  }

  void VhPanelOn()
  {
    digitalWrite(pins.VH_CON, HIGH);
  }
  void VhPanelOff()
  {
    digitalWrite(pins.VH_CON, LOW);
  }

  void setup()
  {
    // Pins
    setupPins();
    delay(5);
    // PowerON
    PowerOn();
    delay(150);
    // Enable panel
    VhPanelOn();
    delay(150);
    vfd.setup();
    vfd.apply();
  }
  // PureLED
  void LightPureLed(bool flag)
  {
    if (pins.LED_PURE > 0)
    {
      ledPinOnOff(pins.LED_PURE, flag);
    }
  }
  // ThureLED
  void LightThruLed(bool flag)
  {
    if (pins.LED_THRU > 0)
    {
      ledPinOnOff(pins.LED_THRU, flag);
    }
  }
  // Keys 0,1,2
  void SetKeyPins(byte key0, byte key1, byte key2)
  {
    if (key0 > 0)
      pins.KEY0 = key0;
    if (key1 > 0)
      pins.KEY1 = key1;
    if (key2 > 0)
      pins.KEY2 = key2;
  }

private:
  struct PIN_SETS pins;

  void ledPinOnOff(byte pin, bool onoff)
  {
    digitalWrite(pin, onoff ? HIGH : LOW);
  }
  void initLowPin(byte pin)
  {
    if (pin > 0)
    {
      digitalWrite(pin, LOW);
      pinMode(pin, OUTPUT);
      digitalWrite(pin, LOW);
    }
  }
  void setupPins()
  {
    initLowPin(pins.VH_CON);
    initLowPin(pins.POWERD);
    initLowPin(pins.LED_PURE);
    initLowPin(pins.LED_THRU);

    if (pins.KEY0 > 0)
    {
      pinMode(pins.KEY0, INPUT);
      digitalWrite(pins.KEY0, HIGH);
    }

    if (pins.KEY1 > 0)
    {
      pinMode(pins.KEY1, INPUT);
      digitalWrite(pins.KEY1, HIGH);
    }

    if (pins.KEY2 > 0)
    {
      pinMode(pins.KEY2, INPUT);
      digitalWrite(pins.KEY2, HIGH);
    }
    if (pins.KEY3 > 0)
    {
      pinMode(pins.KEY3, INPUT);
      digitalWrite(pins.KEY3, HIGH);
    }

    digitalWrite(pins.VOLA, LOW);
    pinMode(pins.VOLA, INPUT);
    digitalWrite(pins.VOLB, LOW);
    pinMode(pins.VOLB, INPUT);
  }
};
#endif // OnkyoNR509_H