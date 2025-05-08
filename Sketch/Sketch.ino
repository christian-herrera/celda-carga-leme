/*
  // Nombre: dina_00.ino
  // Versión: 0.1
  // Autor: Matías Herlein
  // Descripción: Dinamometro con HX711
  Se trata de un programa que permite la comunicación del módulo que permite levantar la
  tensión de desbalance de la celda de carga, y utiliza Labview para graficar y registrar y guardar los datos registrados.
  Parte del código está tomado de los ejemplos de bodge : https://github.com/bogde/HX711/tree/master
*/

//#include "Adafruit_HX711.h"
#include "HX711.h"
//#include "SDHT.h"

#define DHTTYPE    DHT11


// HX711 circuit wiring, LED y DHT
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
const int DHT_DOUT = 4;
const int LED_PIN = 5;
int i = 0;
char option;
float cal_value = -4687.11f; // para peso conocido 47.650;
float peso_conocido;

// denominada scale
HX711 scale;


//Declarar ubicacion del dht--- Cuando funcione la lib
//SDHT dht(DHT_DOUT,DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);

  //Señalización de inicio
  signal(5);

  Serial.begin(57600);
  Serial.println("Iniciando dinamómetro digital");
  delay(100);

  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);


  scale.set_scale(cal_value);     // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  //----- Fin config Scale ------//

  //Para habilitar cuando funcione DHT
  //dht.begin();
}

void loop() {

  //--- Buffer check --
  if (Serial.available() != 0) {
    option = Serial.read();
    switch (option) {
      case 'f':
        Serial.flush();
        //Señalización de comienzo
        signal(5);
        if (scale.is_ready()) {
          long F;
          long reading = scale.read();
          Serial.println("Fuerza");
          Serial.print("one reading:\t");
          Serial.print(scale.get_units(), 3); Serial.println(" kg");
          Serial.print("| Promedio N=20:\t");//10HZ, es decir 1c/ 0,1 s = 100 ms demora 2s en hacer el promedio
          F = scale.get_units(20);
          Serial.print(F, 3); Serial.print(" kg\t"); Serial.print(F * 9.8, 3); Serial.println(" N");
          option = ' ';
        }
        else {
          Serial.println("HX711 not found.");
        }
        break;
      case 't':
        Serial.flush();
        digitalWrite(LED_PIN, LOW);
        Serial.println("Tara. No aplicar fuerza en la celda.");
        delay(200);
        tara();
        option = ' ';
        break;
      case'c':
        Serial.flush();
        calibrate_celda();
        option = ' ';
        break;
      case 'g':
        if (scale.is_ready()) {
          while (Serial.read() != 's') {
            // en kg y en N,  g=9.797m/s^2  gravimetría nacional,según IGN
            float enkg=scale.get_units(1);
            String Salida=String(enkg, 4);
            //Salida.replace(".", ",");
            Serial.println(Salida);
          }
        }
        break;
      case 'm':
        ajuste_manual();
        break;
      case 'p':
        float myFloat = 3.141595;
        while (Serial.read() != 's')
        {
          Serial.print("El valor de pi es: ");
          Serial.println(myFloat, 4); // Muestra el valor con 4 decimales
          delay(100); // Espera 2 segundos
        }
        break;
    }
  }
}


void tara() {
  scale.set_scale(cal_value);// this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare(5);  // reset the scale to
}

float calibrate_celda() {
  /*How to calibrate your load cell
    1 - Call set_scale() with no parameter.
    2 - Call tare() with no parameter.
    3 - Place a known weight on the scale and call get_units(10). Pedir
    4 - Divide the result in step 3 to your known weight. You should get about the parameter you need to pass to set_scale().
    5 - Adjust the parameter in step 4 until you get an accurate reading.*/
  scale.set_scale();
  scale.tare();
  Serial.println("Ubicar peso conocido");
  delay(200);
  //Serial.println("Ubicar peso conocido, antes que se apague el LED amarillo,y espere a que vuelva a encender /n Sino debe repetir la calibración");
  digitalWrite(LED_PIN, HIGH);
  delay(20000);
  digitalWrite(LED_PIN, LOW);
  //Serial.println("Comienza Calibracion");
  //
  //Serial.println("Ingrese peso conocido");
  //  if (Serial.available() > 0) { // Verifica si hay datos disponibles
  //   cal_value= Serial.parseFloat();
  // }//
  peso_conocido = 47.650f;
  cal_value = scale.get_units(20) / peso_conocido;
  scale.set_scale(cal_value);
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Calibracion finalizada [" + String(cal_value,2) + "]");
  //Serial.print("El valor de calib es:"); Serial.println(cal_value);
  signal(2);
  return cal_value;
}

void enviar() {
}

int signal(byte N) {
  //Señalización de inicio

  for (int i = 0; i < N; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(150);
    return i = 0;
  }
}
float ajuste_manual() {
  //Serial.println("Ajuste manual de valor de calib.");
  cal_value = -4687.11f; // para peso conocido 47.650
  scale.set_scale(cal_value);
  digitalWrite(LED_PIN, HIGH);
  Serial.println("Calibracion finalizada [" + String(cal_value, 2) + "]");
  return cal_value;
}





/* Hasta que pueda acomodar el DHT
  leerDHT(char opt){
    float var=0;
     switch (opt) {
      case 't':
        digitalWrite(LED, LOW);
        var=dht.temperature();
        digitalWrite(LED, HIGH);
        break;
      case 'h':
        // statements
        break;
      default:
        serial.println("Hola soy dht")
    }
    return var;
  }*/
