#include <Arduino.h>

const int kButtonPin = 2;
const int kLedPin    = 3;

enum Mode {
    STEADY = 0,
    SLOW_BLINK,
    MEDIUM_BLINK,
    FAST_BLINK
};

Mode currentMode = STEADY;
const uint8_t kTotalModes = 4;

int lastReading = HIGH;
int stableState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

unsigned long prevMillis = 0;
unsigned long blinkInterval = 1000;
bool blinkLedState = false;

void setIntervalForMode(Mode m) {
    switch (m) {
        case STEADY:      blinkInterval = 0;    break;
        case SLOW_BLINK:  blinkInterval = 1000; break;
        case MEDIUM_BLINK:blinkInterval = 200;  break;
        case FAST_BLINK:  blinkInterval = 50;  break;
    }
}

void applyModeImmediate(Mode m) {
    blinkLedState = false;
    prevMillis = millis();
    if (m == STEADY) {
        digitalWrite(kLedPin, LOW);
    } else {
        blinkLedState = true;
        digitalWrite(kLedPin, LOW);
    }
}

void setup() {
    pinMode(kButtonPin, INPUT_PULLUP);
    pinMode(kLedPin, OUTPUT);
    setIntervalForMode(currentMode);
    applyModeImmediate(currentMode);
}

void loop() {
    int reading = digitalRead(kButtonPin);

    if (reading != lastReading) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != stableState) {
            stableState = reading;
            if (stableState == LOW) {
                currentMode = (Mode)((currentMode + 1) % kTotalModes);
                setIntervalForMode(currentMode);
                applyModeImmediate(currentMode);
            }
        }
    }

    lastReading = reading;

    if (currentMode == STEADY) {
    } else {
        unsigned long now = millis();
        if (now - prevMillis >= blinkInterval) {
            prevMillis = now;
            blinkLedState = !blinkLedState;
            digitalWrite(kLedPin, blinkLedState ? HIGH : LOW);
        }
    }

    delay(5);
}
