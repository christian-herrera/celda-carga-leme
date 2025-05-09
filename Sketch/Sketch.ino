#include "HX711.h"
#include "Config.h"
#include "Commands.h"

HX711 loadcell;
char buffer[DIM_BUFFER];
EstadoSistema maqEstados = IDLE;
float rawKG;
float peso_conocido;


void setup() {
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_GREEN, 0);

  Serial.begin(SERIAL_BAUDRATE);

  loadcell.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("[DEBUG] Sistema inicializado!");

}

void loop() {
  if (checkCommand()) {
    processCommand(maqEstados);
  }



  switch (maqEstados) {
    case IDLE:
      break;

    case PRE_READING:
      Serial.println("[DEBUG] Tomando muestras...");
      maqEstados = READING;
      break;

    case READING:
      //Serial.println(String(loadcell.get_units(1), 4) + " kg");
      Serial.println("#" + String(loadcell.get_units(1), 4) + "&");
      break;

    case SET_TARE:
      loadcell.set_scale(2280.0f);
      loadcell.tare(5);
      maqEstados = IDLE;
      Serial.println("[DEBUG] Offset ajustado!");
      rawKG = loadcell.get_units(1);
      Serial.println("[DEBUG] Valor actual: [" + String(rawKG, 4) + "]");
      break;


    case DO_CALIBRATION:
      loadcell.set_scale();
      loadcell.tare();
      Serial.println("[DEBUG] Colocar peso conocido!");
      delay(20000);
      Serial.println("[DEBUG] Iniciando calibración...");
      float cal_value = loadcell.get_units(20) / peso_conocido;
      loadcell.set_scale(cal_value);
      Serial.println("[DEBUG] Calibración finalizada...");
      break;
  }




}
