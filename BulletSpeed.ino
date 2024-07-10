#include "Arduino.h"

#define PIN_SPEED_START 2
#define PIN_SPEED_END 3


volatile unsigned long interruptedStart = 0;
volatile unsigned long interruptedEnd = 0;

void setup()
{
    Serial.begin(9600);
    Serial.print("Start,end");

    pinMode(PIN_SPEED_START, INPUT);
    pinMode(PIN_SPEED_END, INPUT);

    attachInterrupt(digitalPinToInterrupt(PIN_SPEED_START), triggerInterruptStart, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_SPEED_END), triggerInterruptEnd, RISING);
}

void loop()
{
    if(interruptedStart > 0 && interruptedEnd > 0){
        Serial.println();
        Serial.println("Start " + String(interruptedStart / 1000));
        Serial.println("End " + String(interruptedEnd / 1000));
        Serial.println("Diff " + String(calculateTimeDifference() / 1000));
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