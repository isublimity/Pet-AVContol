#pragma once
#ifndef byte
#include <Arduino.h>
#endif

#include "../lib/digitalWriteFast.h"

#ifndef OnkyoNR509_H
#include "OnkyoNR509.cpp"
#endif
#ifndef OnkyoBUTTONS_H
#include "OnkyoNR509Buttons.h"
#endif

#ifndef OnkyoController_H
#define OnkyoController_H

class OnkyoController
{
    OnkyoNR509 *avr;

public:
    static OnkyoController *_instance;
    explicit OnkyoController(OnkyoNR509 *_avr)
    {
        avr = _avr;
    }

    bool isVolumeEncoderSupportInterrupt()
    {
        if (avr->getPins().VOLA == 2 || avr->getPins().VOLB == 2)
            return true;
        return false;
    }

    void setup()
    {
        // OnkyoController::_instance = this;
        avr->setup();
        avr->setVolume(-50);
        avr->LightThruLed(true);

        // Debug on start
        const char compile_date[] = __DATE__ " " __TIME__; // For dump
        avr->setDisplay("Privet V0.2", compile_date);
        // Force render
        render(true);
        setPinsExtender();
    };
    // -------------------- Interrupt\s -------------------------
    void encoderInterrupt()
    {
        avr->volumeEncoderTick();
        avr->setDisplay("...", ".-.");
    };

    void ledBlinks()
    {
        // led
    }
    void IRScan()
    {
        //
    }

    int bt1_oldKeyValue;
    long bt1_lastChange;
    int bt1_keyValue = 55; // Состояние покоя

    int _bt_values[4] = {55, 55, 55, 55};   // Переменная для хранения предыдущего значения состояния кнопок
    long _bt_lastChanges[4] = {0, 0, 0, 0}; // Переменная для хранения времени последнего изменения состояния
    int _bt_oldValues[4] = {0, 0, 0, 0};    // Состояние покоя

    int _applyButtonValue(int key, int actualKeyValue)
    {
        actualKeyValue = ((actualKeyValue - 128) >> 4) + 1;
        if ((actualKeyValue != _bt_oldValues[key]) && (millis() - _bt_lastChanges[key] > 100))
        {                                        // Пришло новое значение, и с последнего изменения прошло достаточно времени
            _bt_oldValues[key] = actualKeyValue; // Запоминаем новое значение
            _bt_lastChanges[key] = millis();     // Обнуляем таймер
        }
        // Отправляем старое, либо уже модифицированное новое значение
        if (_bt_values[key] != _bt_oldValues[key])
        {
            _bt_values[key] = _bt_oldValues[key];
            if (_bt_values[key] < 50)
            {
                return _bt_values[key];
            }
        }
        return 55;
    }

    // int _getButtonCode(int key,int value)
    // {
    //     if (value > 55) return 0;

    //     int minValue = (value > 2 ? value-2 : 0);
    //     int maxValue = value+2;

    // }

    // https://github.com/dxinteractive/AnalogMultiButton/blob/master/src/AnalogMultiButton.cpp
    volatile bool _flagSPRL_F = false;
    volatile bool _flagSEC_1H = false;
    volatile bool _flagSEC_1OFF = false;
    void setPinsExtender()
    {
        //  digitalWrite(16, LOW);
        //  pinMode(16, OUTPUT);
        //  digitalWrite(18, LOW);
        //  pinMode(18, OUTPUT);
        //  digitalWrite(20, LOW);
        //  pinMode(20, OUTPUT);
    }
    void applySectionContol()
    {
        // 30 PIN - SPRL_F
        // 28 PIN - SEC1_H
        // 26 PIN - SEC1_OFF
        digitalWrite(30, (_flagSPRL_F ? HIGH : LOW));
        digitalWrite(28, (_flagSEC_1H ? HIGH : LOW));
        digitalWrite(26, (_flagSEC_1OFF ? HIGH : LOW));
    }

    // функция для отправки байта [24 бита] в SPI - R2A
    void VolDataSendLong(long sentByte)
    {
        byte CLCK = 16;
        byte DATA = 18;
        for (byte i = 23; i != 255; i--) // (24 бита) количество бит данных для вывода
        {
            // 1UL = unsigned long
            digitalWriteFast(DATA, (sentByte & (1UL << i) ? HIGH : LOW));
            // Если бит (i) равен 1 то устанавливаем бит в регистре PORTB.0 (MOSI)
            // Иначе сбрасываем бит в регистре PORTB.0 (MOSI)
            delayMicroseconds(4); // Формируем задержку для CLK

            digitalWriteFast(CLCK, HIGH);
            delayMicroseconds(2);
            digitalWriteFast(DATA, LOW);
            if (i == 0)
            {
                //
                digitalWriteFast(DATA, LOW);
                delayMicroseconds(2);
            }
            else
            {
                delayMicroseconds(4);
            }
            digitalWriteFast(CLCK, LOW);
            delayMicroseconds(2);
            digitalWriteFast(DATA, LOW);
        }
        delayMicroseconds(30); // Apply
    }
    void VolDataSendLong2(long sentByte)
    {
        byte SCK_PIN_1 = 16;
        byte MOSI_PIN_1 = 18;
        // ------------------------------------------------------------------------------------
        
        for (byte i = 23; i != 255; i--) // (24 бита) количество бит данных для вывода
        {
            
            if (sentByte & (1UL << i))
                PORTB |= (1 << MOSI_PIN_1); 
            else
                PORTB &= ~(1 << MOSI_PIN_1); // Иначе сбрасываем бит в регистре PORTB.0 (MOSI)

            delayMicroseconds(2); // Формируем задержку для CLK

            PORTB |= (1 << SCK_PIN_1); // Устанавливаем бит на шине CLK (PORTB.1)
            delayMicroseconds(2);
            PORTB &= ~(1 << MOSI_PIN_1); // Сбрасываем бит в регистре PORTB.0 (MOSI)

            if (i == 0)                     // Проверяем не является ли текущий бит младшим
            {                               // Если бит младший:
                PORTB |= (1 << MOSI_PIN_1); // формируем защелку (устанавливаем бит в PORTB.0)
                delayMicroseconds(2);       // формируем задержку для защелки
            }
            else // Если бит (i) не является младшим
            {
                delayMicroseconds(2); // Формируем задержку для CLK
            }
            PORTB &= ~(1 << SCK_PIN_1); // Сбрасываем бит на шине CLK (PORTB.1)
        }

        delayMicroseconds(2);        // формируем задержку для сброса защелки
        PORTB &= ~(1 << MOSI_PIN_1); // Сбрасываем защелку PORTB.0 (MOSI)
    }

    void VolDataSend(byte type=0)
    {
        // ------------------------------
        // 18 -> 2 VOL_DATA
        // 16 -> 5 VOL_CLOCK
        // ------------------------------
         
         
         

         
        // CMD....
        if (type ==0 ) { // VCR
            
            VolDataSendLong(0xFF0103);
            VolDataSendLong(0xFF0022);
            VolDataSendLong(0xFFFFC5);
            VolDataSendLong(0xFFFFCC);
            VolDataSendLong(0xFFFFE6);
            VolDataSendLong(0xFFFFCC);
            VolDataSendLong(0xFFFFE6);
            VolDataSendLong(0x0F8131);
            VolDataSendLong(0xFF0022);

        }
        if (type ==1 ) { // TV
            VolDataSendLong(0xFF0003);
            VolDataSendLong(0xFF0022);
            VolDataSendLong(0xFFFFE5);
            VolDataSendLong(0xFFFFEC);
            VolDataSendLong(0xFFFFC6);
            VolDataSendLong(0xFFFFEC);
            VolDataSendLong(0x6F0131);
            VolDataSendLong(0xFF0022);
        }
        if (type ==2 ) { // CBL 
            VolDataSendLong(0xFF0003);
            VolDataSendLong(0xFF0022);
            VolDataSendLong(0xFFFFC5);
            VolDataSendLong(0xFFFFCC);
            VolDataSendLong(0xFFFFE6);
            VolDataSendLong(0xFFFFCC);
            VolDataSendLong(0x1F4131);
            VolDataSendLong(0xFF0022);
        }
        if (type ==4 ) { // GAME DIRECT 
                    
                VolDataSendLong(0xFF0103);
                VolDataSendLong(0xFF0022);
                VolDataSendLong(0xFFFFE5);
                VolDataSendLong(0xFFFFEC);
                VolDataSendLong(0xFFFFC6);
                VolDataSendLong(0xFFFFEC);
                VolDataSendLong(0x4900F1);
                VolDataSendLong(0xFF0022);

                delay(5);

                VolDataSendLong(0xFF0103);
                VolDataSendLong(0xFF0022);
                VolDataSendLong(0xFFFFE5);
                VolDataSendLong(0xFFFFEC);
                VolDataSendLong(0xFFFFC6);
                VolDataSendLong(0xFFFFEC);

                delay(36);

                VolDataSendLong(0x4900F1);
                VolDataSendLong(0xFF0022);
                VolDataSendLong(0xFF0103);
                VolDataSendLong(0xFFFFEC);
                VolDataSendLong(0xFFFFE5);
                VolDataSendLong(0xFFFFC6);

                delay(3);

                VolDataSendLong(0xFF0103);
                VolDataSendLong(0xFF0022);
                VolDataSendLong(0xFFFFE5);
                VolDataSendLong(0xFFFFEC);
                VolDataSendLong(0xFFFFC6);
                VolDataSendLong(0xFFFFEC);

                delay(163);

                VolDataSendLong(0x3C0103);
                VolDataSendLong(0x3C0022);
                VolDataSendLong(0xFFFFE5);
                VolDataSendLong(0xFFFFEC);
                VolDataSendLong(0xFFFFC6);
                VolDataSendLong(0xFFFFEC);

                delay(10);

        }
    }

    void buttonScan()
    {
        avr->ButtonsRead();

        int KEY0 = _applyButtonValue(0, avr->lastButtonValue0);
        int KEY1 = _applyButtonValue(1, avr->lastButtonValue1);
        int KEY2 = _applyButtonValue(2, avr->lastButtonValue2);
        int KEY3 = _applyButtonValue(3, avr->lastButtonValue3);

        if (KEY0 < 55 || KEY1 < 55 || KEY2 < 55 || KEY3 < 55)
        {
            String txt = "KEY";

            if (KEY1 >= 0 && KEY1 <= 4)
            {
                txt = "USB";
                _flagSEC_1OFF = !_flagSEC_1OFF;
                txt = "SEC 1OFF = " + String(_flagSEC_1OFF ? "ON" : "OFF");
                applySectionContol();
            }
                
            if (KEY1 >= 5 && KEY1 <= 8)
            {
                txt = "NET";
                _flagSEC_1H = !_flagSEC_1H;
                txt = "SEC 1H = " + String(_flagSEC_1H ? "ON" : "OFF");
                applySectionContol();
            }
                
            if (KEY1 >= 9 && KEY1 <= 12) 
            {
                txt = "PORT";
                _flagSPRL_F = !_flagSPRL_F;
                txt = "SPRL-F = " + String(_flagSPRL_F ? "ON" : "OFF");
                applySectionContol();
            }
                
            if (KEY1 >= 14 && KEY1 <= 16)
            {
                txt = "TVCD -T1";
                VolDataSend(1);
            }
                
            if (KEY1 >= 18 && KEY1 <= 22)
                txt = "TUNER";
            if (KEY1 >= 24 && KEY1 <= 28)
                txt = "AUX";
            if (KEY1 >= 30 && KEY1 <= 33)
            {
                txt = "GAME - T4"; 
                 VolDataSend(4);
            }
                
            if (KEY1 >= 35 && KEY1 <= 39)
            {
                txt = "CABLSAT - T2";
                VolDataSend(2);
                //
            }
            if (KEY1 >= 41 && KEY1 <= 45)
            {
                txt = "VCR - T0";
                VolDataSend(0);
                //
            }
            if (KEY0 >= 14 && KEY0 <= 18)
            {
                txt = "BDDVD";
            }
            // avr->setDisplay(txt, String(KEY0) + " " + String(KEY1) + " " + String(KEY2) + " " + String(KEY3));
            avr->setDisplay(txt, String(_flagSPRL_F ? "ON" : "OFF") + " " + String(_flagSEC_1H ? "ON" : "OFF") + " " + String(_flagSEC_1OFF ? "ON" : "OFF") + " " + String(KEY3));
        }
    }

    void render(bool force = false)
    {
        avr->renderVolume(force);
        avr->renderSymbols(force);
        avr->renderStrings(force);
    }

private:
    void _debug(String msg)
    {
        Serial.println(msg);
    }
};

#endif // OnkyoController_H