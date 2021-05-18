// ------------------------------------------------ PINS    ---------------------------------------------------
// LED,in P701B use 6,8
#define PIN_LED_THRU 37     // LED Pin: HDMI THROUGH YELLOW SLI-343Y8C-3F D7152
#define PIN_LED_PURE 39     // LED Pin: PURE AUDIO BLUE SCB3318E-D2 THX D7154 
// ------------------------------------------------------------------------------------------------------------
// Volume encoder,in P701B use 12,14
#define PIN_ENCODER_VOL_A 2 // S7002 - EC12E2425WITH_WASHER
#define PIN_ENCODER_VOL_B 4
// ------------------------------------------------------------------------------------------------------------
// Keys pins 
#define PIN_PAN_KEY_0   55  // STBY,PURE AUDIO,Z2 OFF see R7101,R7105
#define PIN_PAN_KEY_1   54  // USB,PORT,TUNER,AUX,VCR/DVR R7106
#define PIN_PAN_KEY_2   56  // TUNING,DIMMER,MUSIC,TONE... see R7121,R7115,R7116...
#define PIN_PAN_KEY_3   57  // TUN UP,ENTER,RETURN... see R7123,R7124,R7125...
// Base key...
#define PIN_BASE_ANA    58 
// ------------------------------------------------------------------------------------------------------------
// VFD 16-BT-165GINK , Q7002 
#define PIN_VFD_RST     43   // FLDRST - RESET
#define PIN_VFD_CS      45    // FLDCS  - cabel select (enable_pin)
#define PIN_VFD_CLK     47   // FLDCLK - clock pin  CLOCK
#define PIN_VFD_SDO     41   // FLDSDO - data pin
// ------------------------------------------------------------------------------------------------------------
// Control pins
#define PIN_AV_POWERD   46   // POWERD,Pin 0 in JL901A
#define PIN_AV_VH_CON   44   // Power VFD + panel +32V DISPLAY SECTION PART9   P701B VH_CON1=4   MinePin=22
// IR Pin 
#define PIN_IR_RECV     3