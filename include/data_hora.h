//
// Created by João Matos on 14/12/2022.
//

#ifndef PROJETO1_DATA_HORA_H
#define PROJETO1_DATA_HORA_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "comum.h"

typedef struct {
    clock_t inicio;
    clock_t fim;
    clock_t tempo_decorrido;
} t_temporizador;

t_temporizador* criar_temporizador();
void iniciar_temporizador(t_temporizador* temporizador);
void parar_temporizador(t_temporizador* temporizador);
__inline__ void mostrar_tempo_decorrido(t_temporizador* temporizador, char* mensagem);

#endif //PROJETO1_DATA_HORA_H