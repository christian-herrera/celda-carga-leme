#ifndef CONFIG_H
#define CONFIG_H


/*
 * Pines para el HX711
 */
#define LOADCELL_DOUT_PIN 2
#define LOADCELL_SCK_PIN 3


/*
 * Pines para los leds
 */
#define LED_GREEN 13



/*
 * Comunicación serial
 */
#define SERIAL_BAUDRATE 57600
#define DIM_BUFFER 20


/*
 * Memoria EEPROM
 */
#define EEPROM_DIR_PESO_CAL 0
#define EEPROM_DIR_CTE_CAL  EEPROM_DIR_PESO_CAL + 4



/*
 * Maquina de Estados
 */
typedef enum {
  IDLE,
  READING,
  SET_TARE,
  DO_CALIBRATION
} EstadoSistema;


typedef struct {
  float rawKG;
  float peso_conocido;
  float cte_calibracion;
} MiDataVar;



#endif
