class OnkyoController {
    static OnkyoNR509 *avr; 
    static OnkyoController *instance;
    OnkyoController(OnkyoNR509 *_avr) {
        avr = _avr;
    }
    static void _in_encoderTick();
    static void setup() 
}