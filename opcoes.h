/*
        opcoes.h - Gestão da navegação num menu de várias opções
        Criada por José Neves e Miguel Fazenda, 5 de fevereiro de 2018
        BitChallenge 2018
*/

#ifndef OPCOES_H
#define OPCOES_H

#include "input.h"
#include "printLCD.h"

class Opcoes {
 public:
  Opcoes();

  uint8_t opcaoAtual = 0;
  uint8_t opcaoMaisAcima = 0;
  uint8_t numOpcoes = 0;

  uint16_t needsFreeBoolArray;
  char** listaOpcoes = NULL;

  void printOpcoes();
  void slideThroughOpcoes();
  void setOpcoes(int numeroOpcoes);

  void setConstOption(uint8_t num, const char* str);
  void setDynOptionCpy(uint8_t num, char* src, uint8_t len);
};

#endif