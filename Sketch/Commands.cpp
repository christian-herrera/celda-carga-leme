#include "Arduino.h"
#include "Commands.h"
#include "string.h"

extern float peso_conocido;
extern char buffer[DIM_BUFFER];


bool checkCommand() {
  int i = 0;
  if (Serial.available()) {
    while (Serial.available()) {
      delay(5);
      buffer[i++] = (char)Serial.read();

      //Exit en el limite del buffer
      if (i == DIM_BUFFER - 1)
        break;
    }
    buffer[i] = '\0';
  }

  return i > 0;
}


void processCommand(EstadoSistema &maqEstados) {
  //Serial.println("[DEBUG] Recibido: [" + String(buffer) + "]");
  
  // #start&
  if (strcmp(buffer, "#start&") == 0) {
    digitalWrite(LED_GREEN, 1);
    maqEstados = READING;
    return;
  }

  // #stop&
  if (strcmp(buffer, "#stop&") == 0) {
    digitalWrite(LED_GREEN, 0);
    maqEstados = IDLE;
    return;
  }

  // #set-tare&
  if (strcmp(buffer, "#set-tare&") == 0) {
    blinkLed();
    maqEstados = SET_TARE;
    return;
  }


  // #set-weight$xxxx.xx&
  if (strncmp(buffer, "#set-weight$", 12) == 0) {
    char * initStr = buffer + 12;
    char * endStr = strchr(initStr, '&');
    
    if(endStr != NULL){
      *end = '\0';
      peso_conocido = atof(start);
      maqEstados = IDLE;
      Serial.println("[DEBUG] Peso conocido guardado: [" + String(peso_conocido, 4) + "]");
    }
        
    return;
  }

  // DO-CALIBRATION
  if (strcmp(buffer, "do-calibration") == 0) {
    maqEstados = DO_CALIBRATION;
    return;
  }
}




void blinkLed() {
  for (int j = 0; j < 15; j++) {
    digitalWrite(LED_GREEN, 1);
    delay(100);
    digitalWrite(LED_GREEN, 0);
    delay(100);
  }
}
