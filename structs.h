/*
        strcucts.h - Registo do conjunto de estruturas
        Criada por José Neves e Miguel Fazenda, 18 de março de 2018
        BitChallenge 2018
*/

#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>

#define DIM_USERNAME 15

typedef struct {
  uint8_t ano, mes, dia;  // ano = anoReal - 2000 para ser de 8 bits
} DATE;

class HORA {
 public:
  HORA(void);
  HORA(uint8_t hora, uint8_t minuto);
  uint8_t hora, minuto;
  HORA subtraiMinutos(uint8_t minutes, bool* mudouDeDia);
};

typedef struct {
  char userName[DIM_USERNAME + 1];
  HORA horaPorDiaSemana[7];  // 25 para não aquecer nesse dia da semana
  uint8_t temperature;
  uint8_t numMin;
} EEPROM_USER_DATA;

typedef struct horario_stru_optional {
  bool userExists;
  int userPageNum;
  EEPROM_USER_DATA data;
} EEPROM_USER_DATA_OPTIONAL;

#endif
