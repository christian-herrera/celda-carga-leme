#include "Arduino.h"
#include "Commands.h"
#include "string.h"
#include "EEPROM.h"

extern MiDataVar data;
extern char buffer[DIM_BUFFER];


// ─────────────────────────────────────────────
//                          Función: checkCommand()
// ─────────────────────────────────────────────
bool checkCommand() {
  int i = 0;
  char c;
  while (Serial.available()) {
    delay(5);
    c = Serial.read();

    //Exit al llegar el Retorno de carro
    if (c == '\r') {
      break;
    }

    buffer[i++] = c;  //Agrego la buffer y luego incremento i

    //Exit en el limite del buffer
    if (i == DIM_BUFFER - 1)
      break;
  }
  buffer[i] = '\0';
  return i > 0;
}

// ─────────────────────────────────────────────
//           Función: processCommand(EstadoSistema &maqEstados)
// ─────────────────────────────────────────────
void processCommand(EstadoSistema &maqEstados) {
  //Serial.println("[DEBUG] Recibido: [" + String(buffer) + "]");

  // #check&  -> #ok&
  if (strcmp(buffer, "#check&") == 0) {
    Serial.print("#ok&\r");
    return;
  }


  // #get-cal-val& -> #cte-calib$xxxx.xx&
  if (strcmp(buffer, "#get-cte-calib&") == 0) {
    Serial.print("#cte-calib$" + String(data.cte_calibracion, 2) + "&\r");
  }

  // #get-weight&  ->  #weight$xxxx.xx&
  if (strcmp(buffer, "#get-weight&") == 0) {
    Serial.print("#weight$" + String(data.peso_conocido, 2) + "&\r");
  }

  // #start&  -> #starting&
  if (strcmp(buffer, "#start&") == 0) {
    digitalWrite(LED_GREEN, 1);
    Serial.print("#starting&\r");
    maqEstados = READING;
    delay(2000);
    return;
  }

  // #stop&  -> #stopping&
  if (strcmp(buffer, "#stop&") == 0) {
    digitalWrite(LED_GREEN, 0);
    Serial.print("#stopping&\r");
    maqEstados = IDLE;
    return;
  }

  // #set-tare&  -> #tare-ok&
  if (strcmp(buffer, "#set-tare&") == 0) {
    maqEstados = SET_TARE;
    return;
  }


  // #set-weight$xxxxxx&  -> #saved-weight&
  if (strncmp(buffer, "#set-weight$", 12) == 0 && strchr(buffer, '&') != NULL) {
    data.peso_conocido = descomponerValor(buffer);
    EEPROM.put(EEPROM_DIR_PESO_CAL, data.peso_conocido);
    Serial.println("#saved-weight&\r");
    maqEstados = IDLE;
    return;
  }


  // #set-cal$xxxxxx&  ->  #saved-cte-calib&
  if (strncmp(buffer, "#set-cal$", 9) == 0 && strchr(buffer, '&') != NULL) {
    data.cte_calibracion = descomponerValor(buffer);
    EEPROM.put(EEPROM_DIR_CTE_CAL, data.cte_calibracion);
    Serial.println("#saved-cte-calib&\r");
    maqEstados = IDLE;
    return;
  }



  // DO-CALIBRATION
  if (strcmp(buffer, "#do-auto-cal&") == 0) {
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



float descomponerValor(char *str) {
  // Si llega el str, debe verificarse antes que exista el '$' y el '&'.
  char *initStr = strchr(str, '$') + 1;  // Toma la posicion del '$'
  char *endStr = strchr(str, '&');       // Toma la posicion del '&'
  *endStr = '\0';                        // En el lugar del '&' coloco el '\0'

  return (float)atof(initStr);
}
