/*
        printLCD.cpp - Apresentação de dados frequentes
        Criada por José Neves e Miguel Fazenda, 7 de fevereiro de 2018
        BitChallenge 2018
*/

#include "printLCD.h"

extern LiquidCrystal_I2C lcd;

String multiplicaCaracter(uint8_t numVezes, char ch) {
  String res = "";

  for (uint8_t i = 0; i < numVezes; i++)
    res += ch;

  return res;
}
String Int_To_Str(uint16_t numero, uint8_t expectedNumDigits) {
  String numeroStr = String(numero);
  uint8_t numZeros = expectedNumDigits - numeroStr.length();

  return multiplicaCaracter(numZeros, '0') + numeroStr;
}
void print_Int(uint8_t numero, uint8_t expectedNumDigits) {
  lcd.print(Int_To_Str(numero, expectedNumDigits));
}
void print_Blinking_Str(String str, bool vazio) {
  // Se se pretender apagar
  if (vazio)
    lcd.print(multiplicaCaracter(str.length(), ' '));
  // Se se pretender escrever
  else
    lcd.print(str);
}

void printTemperature(uint8_t temp) {
  print_Int(temp, 2);
  lcd.print((char)223);
  lcd.print("C");
}
void printHumidity(uint8_t hum) {
  print_Int(hum, 2);
  lcd.print("%");
}

// Grandeza pode tomar os valores 0 ou 1, sendo que 0 corresponde a doW e 1 a
// mês
String getMes_ou_DoW(uint8_t valor, byte grandeza) {
  char str[4];

  EEPROM.get(valor * 3 + grandeza * 7 * 3, str);
  str[3] = '\0';

  return String(str);
}
void printDoW(uint8_t doW, int8_t modo, bool aceso) {
  print_Blinking_Str(getMes_ou_DoW(doW, DOW), modo == CAMPO_DOW && !aceso);
}

void printData(uint8_t ano, uint8_t mes, uint8_t dia, int8_t modo, bool aceso) {
  print_Blinking_Str(Int_To_Str(dia, 2), modo == CAMPO_DIA && !aceso);
  lcd.print(" ");
  print_Blinking_Str(getMes_ou_DoW(mes - 1, MES), modo == CAMPO_MES && !aceso);
  lcd.print(" ");
  print_Blinking_Str(Int_To_Str(ano + 2000, 4), modo == CAMPO_ANO && !aceso);
}
void printHora(uint8_t hora, uint8_t minuto, int8_t modo, bool aceso) {
  print_Blinking_Str(Int_To_Str(hora, 2), modo == CAMPO_HORA && !aceso);
  lcd.print(":");
  print_Blinking_Str(Int_To_Str(minuto, 2), modo == CAMPO_MINUTO && !aceso);
}

void printUserName(char strIns[]) {
  uint8_t posFinal = strlen(strIns);

  lcd.print(strIns);
  lcd.print(multiplicaCaracter(LCD_COMP - 2 - posFinal, ' '));
}