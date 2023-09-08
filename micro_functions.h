//
// Created by emanu on 27/05/2023.
//

#ifndef SIMULATORE_PER_DEBUG_MICRO_FUNCTIONS_H
#define SIMULATORE_PER_DEBUG_MICRO_FUNCTIONS_H

#include "main.h"

//funzioni controllo motore
void motor_control_micro_part(direction *d, speed *s);
void stop_motor_micro_part(void);

//funzione inizializzazione
void initialization_sequence(void);
void initializeTOF(void);

//funzione lettura tof
int read_tof_micro_part(void);

//funzione seriale
void sendMessage(char *outputMessage);
#endif
