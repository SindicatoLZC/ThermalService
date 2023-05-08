#ifndef SLEEP_CLASS_H
#define SLEEP_CLASS_H
class Sleep {
public:
  Sleep() {
  }
  static void passTime(unsigned long milisegundos) {
    unsigned long tiempoActual = millis();
    unsigned long tiempoFinal = tiempoActual + milisegundos;

    while (millis() < tiempoFinal) {
      // No hacer nada, solo esperar
    }
  }
};
#endif