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
#define LED_GREEN 5



/*
 * Comunicación serial
 */
#define SERIAL_BAUDRATE 57600
#define DIM_BUFFER 20



/*
 * Maquina de Estados
 */
typedef enum {
  IDLE,
  PRE_READING,
  READING,
  SET_TARE,
  DO_CALIBRATION
} EstadoSistema;


#endif
