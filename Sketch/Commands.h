#ifndef COMMANDS_H
#define COMMANDS_H

#include "Config.h"

bool checkCommand();
void processCommand(EstadoSistema &maqEstados);


void blinkLed();


float descomponerValor(char * str);



#endif
