/*
        strcucts.cpp - Registo do conjunto de estruturas
        Criada por José Neves e Miguel Fazenda, 6 de setembro de 2018
        BitChallenge 2018
*/

#include "structs.h"
#include <Arduino.h>

HORA::HORA(void) {
  this->hora = 0;
  this->minuto = 0;
}

HORA::HORA(uint8_t hora, uint8_t minuto) {
  this->hora = hora;
  this->minuto = minuto;
}

// Apenas funciona para subtrair entre 0 e 59 minutos
HORA HORA::subtraiMinutos(uint8_t minutos, bool* mudouDeDia) {
  int8_t hora = this->hora - minutos / 60;
  int8_t minuto = this->minuto - minutos % 60;

  if (minuto < 0) {
    hora--;
    minuto = 60 + minuto;
  }

  if (mudouDeDia != NULL) {
    // Se der uma hora negativa significa que passou para o dia anterior
    *mudouDeDia = hora < 0;
  }

  if (hora < 0) {
    hora = 24 + hora;
  }
  return HORA(hora, minuto);
}