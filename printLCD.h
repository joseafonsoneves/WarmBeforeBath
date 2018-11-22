/*
        printLCD.h - Apresentação de dados frequentes
        Criada por José Neves e Miguel Fazenda, 7 de fevereiro de 2018
        BitChallenge 2018
*/

#ifndef PRINT_H
#define PRINT_H

#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <stdint.h>
#include "relogio.h"

#define LCD_COMP 20
#define LCD_ALT 4

#define DOW 0
#define MES 1

#define CAMPO_DOW 0
#define CAMPO_DIA 1
#define CAMPO_MES 2
#define CAMPO_ANO 3
#define CAMPO_HORA 4
#define CAMPO_MINUTO 5
#define NAO_PISCAR -1

#define SETA_HORIZONTAL_MAP \
  { 0x00, 0x04, 0x02, 0x1F, 0x02, 0x04, 0x00, 0x00 }
#define SETA_VERTICAL_MAP \
  { 0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x00, 0x00 }

#define SETA_HORIZONTAL 1
#define SETA_VERTICAL 2

extern LiquidCrystal_I2C lcd;

String multiplicaCaracter(uint8_t numVezes, char ch);
String Int_To_Str(uint16_t numero, uint8_t expectedNumDigits);
void print_Int(uint8_t numero, uint8_t expectedNumDigits);
void print_Blinking_Str(String str, bool vazio);

void printTemperature(uint8_t temp);
void printHumidity(uint8_t hum);

String getMes_ou_DoW(uint8_t valor, byte grandeza);
void printData(uint8_t ano, uint8_t mes, uint8_t dia, int8_t modo, bool aceso);
void printHora(uint8_t hora, uint8_t minuto, int8_t modo, bool aceso);
void printDoW(uint8_t doW, int8_t modo, bool aceso);

void printUserName(char strIns[]);

#endif
