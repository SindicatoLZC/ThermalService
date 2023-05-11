#ifndef SLEEP_CLASS_H
#define SLEEP_CLASS_H
class Sleep {
public:
  Sleep() {
  }
  static void passTime(unsigned long interval) {
    unsigned long currentMillis = millis();
    while (millis() - currentMillis < interval) {
      // No hacer nada, simplemente esperar
    }
  }
};
#endif