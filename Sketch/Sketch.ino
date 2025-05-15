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
  return 50.0 * sin(2 * PI * t / 5000.0) + 150;
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
  Serial.println("[DEBUG] Sistema inicializado!");
  Serial.println("[DEBUG] cte_calibracion: [" + String(data.cte_calibracion) + "]");
  Serial.println("[DEBUG] peso_conocido: [" + String(data.peso_conocido) + "]");
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

    case READING:
      //Serial.println(String(loadcell.get_units(1), 4) + " kg");
      //Serial.println("#" + String(loadcell.get_units(1), 4) + "&");
      Serial.print("#kg$" + String(getSinFromMillis(millis()), 2) + "&\r");
      delay(100);
      break;

    case SET_TARE:
      // loadcell.set_scale(data.cte_calibracion);
      // loadcell.tare(5);
      maqEstados = IDLE;
      // data.rawKG = loadcell.get_units(1);
      Serial.println("#tare-ok&\r");
      blinkLed();
      break;


    case DO_CALIBRATION:
      Serial.print("#cal-process&\r");
      // loadcell.set_scale();
      // loadcell.tare();
      delay(1000);
      for (byte i = 0; i < 100; i+=5) {
        Serial.print("#cal-time$" + String(i) + "&\r");
        delay(1000);
      }
      Serial.print("#cal-init&\r");
      // data.cte_calibracion = loadcell.get_units(20) / data.peso_conocido;
      // loadcell.set_scale(data.cte_calibracion);
      delay(2000);
      Serial.print("#cal-end&\r");
      maqEstados = IDLE;
      break;
  }
}
