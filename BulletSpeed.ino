#include "Arduino.h"
#include "GyverTM1637.h"

#define PIN_SPEED_START 2
#define PIN_SPEED_END 3
#define TM1637_CLK 4
#define TM1637_DIO 5

#define DISTANCE_M 0.1

#define DEBUG 1

GyverTM1637 disp(TM1637_CLK, TM1637_DIO);

volatile unsigned long interruptedStart = 0;
volatile unsigned long interruptedEnd = 0;

void setup()
{
    #ifdef DEBUG
    Serial.begin(9600);
    #endif

    disp.clear();
    disp.brightness(5);
    disp.displayInt(0);

    pinMode(PIN_SPEED_START, INPUT);
    pinMode(PIN_SPEED_END, INPUT);

    attachInterrupt(digitalPinToInterrupt(PIN_SPEED_START), triggerInterruptStart, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_SPEED_END), triggerInterruptEnd, RISING);
}

void loop()
{
    if(interruptedStart > 0 && interruptedEnd > 0){
        unsigned long diff = calculateTimeDifference();
        float speedMPerS = DISTANCE_M / (diff / 1000000.0f);
        printSpeed(speedMPerS);

        #ifdef DEBUG
        Serial.println();
        Serial.println("Start " + String(interruptedStart));
        Serial.println("End " + String(interruptedEnd));
        Serial.println("Diff " + String(calculateTimeDifference()));
        Serial.println("Speed " + String(speedMPerS) + " m/s");
        Serial.println();
        #endif
        interruptedStart = 0;
        interruptedEnd = 0;
    }
}

void triggerInterruptStart()
{
    interruptedStart = micros();
}

void triggerInterruptEnd()
{
    interruptedEnd = micros();
}

unsigned long calculateTimeDifference()
{
    if (interruptedEnd > interruptedStart)
    {
        return interruptedEnd - interruptedStart;
    }
    return interruptedStart - interruptedEnd;
}

void printSpeed(float speedMPerS)
{
    disp.clear();
    
    if(speedMPerS > 1){
        disp.displayInt((int) speedMPerS);
    } else {
        int percent = floor(speedMPerS * 100);
        disp.displayInt(percent);
        disp.display(1, 0);
        if(percent < 10){
            disp.display(2, 0);
        }
    }
}