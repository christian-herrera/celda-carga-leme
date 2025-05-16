#include "HX711.h"
#include "Config.h"
#include "Commands.h"
#include "EEPROM.h"

HX711 loadcell;

char buffer[DIM_BUFFER];
unsigned long mark;
EstadoSistema maqEstados = IDLE;
MiDataVar data;


float getSinFromMillis(unsigned long t) {
  return 200.0 * sin(2 * PI * t / 10000.0) + 30.0 * sin(2 * PI * t / 1050.0) + 150;
}


/*
   SETUP
*/
void setup() {
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, 0);
  Serial.begin(SERIAL_BAUDRATE);

  loadcell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  EEPROM.get(EEPROM_DIR_PESO_CAL, data.peso_conocido);
  EEPROM.get(EEPROM_DIR_CTE_CAL, data.cte_calibracion);
}

/*
   LOOP
*/
void loop() {
  if (checkCommand()) {
    processCommand(maqEstados);
  }



  switch (maqEstados) {
    case IDLE:
      break;

    /*
     * En este estado, envia cada un cierto tiempo los valores que
     * devuelve la libreria de la celda de carga. Se puede colocar
     * con promedios o con una unica muestra.
     */
    case READING:
      // ─────────────────────────────────────── LECTURA EN KG DE LA CELDA ──
      //Serial.println("#kg$" + String(loadcell.get_units(1), 2) + "&\r");   // A Implementar
      Serial.print("#kg$" + String(getSinFromMillis(millis()), 2) + "&\r");  // ELIMINAR...
      // ────────────────────────────────────────────────────────────────────

#if MS_BETWEEN_READ != 0
      delay(MS_BETWEEN_READ);
#endif

      break;


    /*
     * En este estado, envía un único valor que será aquel que devuelve la
     * libreria de la celda. Se puede configurar en el archivo Config.h
     * si es una única lectura o un promedio de N lecturas.
     */
    case ONE_READ:
      // ─────────────────────────────────────── LECTURA EN KG DE LA CELDA ──
      //Serial.println("#kg$" + String(loadcell.get_units(NUM_READS_FOR_AVG), 2) + "&\r");   // A Implementar
      Serial.print("#one-read$" + String(getSinFromMillis(millis()), 2) + "&\r");  // ELIMINAR...
      // ────────────────────────────────────────────────────────────────────
      maqEstados = IDLE;
      break;





    /*
     * En este testado, realiza la funcion "Tara" para fijar el offset
     * de la medicion. La libreria implementa esto con un metodo.
     */
    case SET_TARE:
      // ────────────────────────────────────────────────── APLICO LA TARA ──
      // loadcell.set_scale(data.cte_calibracion);        // A implementar
      // loadcell.tare(5);                                // A implementar
      // ────────────────────────────────────────────────────────────────────
      Serial.println("#tare-ok&\r");
      maqEstados = IDLE;
      blinkLed(10, 100);
      break;


    /*
     * En este estado, se obtiene en base a un peso conocido, el valor
     * de la constante de calibración. Envia paquetes de estados a la app
     * para seguir el proceso con la aplicacion.
     */
    case DO_CALIBRATION:
      Serial.print("#cal-process&\r");
      // ───────────────────────────────────────────── EJECUCIONES PREVIAS ──
      // loadcell.set_scale();    // A implementar
      // loadcell.tare();         // A implementar
      // ────────────────────────────────────────────────────────────────────
      delay(1000);
      for (byte i = 0; i < 100; i += 5) {
        Serial.print("#cal-time$" + String(i) + "&\r");
        blinkLed(10, 100);
      }
      Serial.print("#cal-init&\r");
      digitalWrite(LED_GREEN, 1);
      // ──────────────────────────────────────────────── CALIBRACIÓN REAL ──
      // data.cte_calibracion = loadcell.get_units(20) / data.peso_conocido;  // A implementar
      // loadcell.set_scale(data.cte_calibracion);                            // A implementar
      data.cte_calibracion = millis() / 100.0;               // ELIMINAR...
      EEPROM.put(EEPROM_DIR_CTE_CAL, data.cte_calibracion);  // A implementar
      // ────────────────────────────────────────────────────────────────────
      delay(2000);
      Serial.print("#cal-end&\r");
      digitalWrite(LED_GREEN, 0);
      maqEstados = IDLE;
      break;
  }
}
