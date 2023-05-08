#ifndef LED_CONTROL_H
#define LED_CONTROL_H
#include "Sleep.h"
#include <stdint.h>

class LedControl {
public:
  static const uint8_t PIN_LED_RED = 32;
  static const uint8_t PIN_LED_BLUE = 25;
  static const uint8_t PIN_LED_GREEN = 33;
  LedControl() {
  }

  static void blink(uint8_t led) {
    digitalWrite(led, HIGH);
    Sleep::passTime(500);
    digitalWrite(led, LOW);
  }

  static void turnOn() {
    digitalWrite(PIN_LED_BLUE, HIGH);
  }

  static void turnOff() {
    digitalWrite(PIN_LED_BLUE, LOW);
  }

  static void parpadearLed(int pinLed, unsigned long duracionEncendido) {
    unsigned long tiempoActual = millis();
    static unsigned long tiempoPrevio = tiempoActual;

    if (tiempoActual - tiempoPrevio >= duracionEncendido) {
      tiempoPrevio = tiempoActual;
      digitalWrite(pinLed, !digitalRead(pinLed));
    }
  }

  static void blinkError() {
    for (uint8_t i = 0; i < 6; i++) {
      digitalWrite(PIN_LED_RED, HIGH);
      Sleep::passTime(20);
      digitalWrite(PIN_LED_RED, LOW);
    }
  }
};
#endif