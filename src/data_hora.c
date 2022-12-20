//
// Created by João Matos on 14/12/2022.
//

#include "../include/data_hora.h"


int obter_minuto_atual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_min;
}

int obter_hora_atual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_hour;
}

int obter_dia_atual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mday;
}

int obter_mes_atual() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    return tm.tm_mon + 1;
}

int obter_ano_atual() {
    time_t tempo = time(NULL);
    struct tm* data = localtime(&tempo);
    return data->tm_year + 1900;
}

char* obter_data_atual() {
    time_t tempo = time(NULL);
    struct tm* data = localtime(&tempo);
    char* data_atual = (char*) malloc(sizeof(char) * 11);
    sprintf(data_atual, "%d-%d-%d", data->tm_mday, data->tm_mon + 1, data->tm_year + 1900);
    return data_atual;
}

char* obter_hora_atual_completa() {
    time_t tempo = time(NULL);
    struct tm* data = localtime(&tempo);
    char* hora_atual = (char*) malloc(sizeof(char) * 9);
    sprintf(hora_atual, "%d:%d:%d", data->tm_hour, data->tm_min, data->tm_sec);
    return hora_atual;
}