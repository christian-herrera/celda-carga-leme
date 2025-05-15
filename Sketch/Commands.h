#ifndef COMMANDS_H
#define COMMANDS_H

#include "Config.h"

bool checkCommand();
void processCommand(EstadoSistema &maqEstados);

void blinkLed(byte veces, int ms);

float descomponerValor(char * str);



#endif
