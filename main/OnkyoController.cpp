#pragma once
#include <OnkyoController.h>
#ifndef byte
#include <Arduino.h>
#endif

#ifndef OnkyoNR509_H
#include "OnkyoNR509.cpp"
#endif

#ifndef OnkyoController_H
#define OnkyoController_H

void OnkyoController::setup() {
            instance = this;
            avr->setup();
            avr->setVolume(-50);
            avr->LightThruLed(true);

            if (avr->getPins().VOLA == 2 || avr->getPins().VOLB == 2) {
                Serial.println('attachInterrupt');
                attachInterrupt(0, _in_encoderTick, CHANGE);
            }


            // Debug on start 
            const char compile_date[] = __DATE__ " " __TIME__; // For dump 
            avr->vfd.printStrings("Privet V0.1", compile_date);
            avr->vfd.apply();
        }
    // -------------------- Interrupt\s -------------------------
    static void _in_encoderTick() {
        instance->avr->volumeEncoderTick();
        Serial.println('df.achInterrupt');
        
    }
};


#endif // OnkyoController_H