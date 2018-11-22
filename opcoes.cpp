/*
        opcoes.cpp - Gestão da navegação num menu de várias opções
        Criada por José Neves e Miguel Fazenda, 5 de fevereiro de 2018
        BitChallenge 2018
*/

#include "opcoes.h"

extern Input input;

Opcoes::Opcoes() {
  uint8_t setaHorizontalMap[] = SETA_HORIZONTAL_MAP;
  lcd.createChar(SETA_HORIZONTAL, setaHorizontalMap);
  // Criam uma seta horizontal

  listaOpcoes = NULL;
}
void Opcoes::printOpcoes() {
  // Percorre os índices das opções desde aquela que está mais acima até à que
  // fica mais abaixo ou à última
  for (uint8_t i = opcaoMaisAcima; i < opcaoMaisAcima + 4 && i < numOpcoes;
       i++) {
    lcd.setCursor(1, i - opcaoMaisAcima);  // Vai descendo o cursor pelo ecrã
                                           // sempre na coluna 1
    lcd.print(listaOpcoes[i]);             // Imprime a opção
  }

  lcd.setCursor(0, opcaoAtual - opcaoMaisAcima);
  lcd.write(SETA_HORIZONTAL);
}
void Opcoes::slideThroughOpcoes() {
  if (input.checkCliqueBotao(BOTAO_CIMA) ||
      input.rotationEvent == ROTATION_LEFT) {
    if (opcaoAtual > 0) {
      lcd.setCursor(0, opcaoAtual - opcaoMaisAcima);
      lcd.print(" ");
      opcaoAtual--;

      /*Se a opcao selecionada for a primeira que se vê no lcd, diminuir o
      opcaoview para passar a ser a segunda, caso não seja a primeira de todas
      as opcoes*/
      if (opcaoAtual == opcaoMaisAcima && opcaoAtual > 0) {
        opcaoMaisAcima--;
        lcd.clear();
        printOpcoes();
      } else {
        lcd.setCursor(0, opcaoAtual - opcaoMaisAcima);
        lcd.write(SETA_HORIZONTAL);
      }
    }
  } else if (input.checkCliqueBotao(BOTAO_BAIXO) ||
             input.rotationEvent == ROTATION_RIGHT) {
    if (opcaoAtual < numOpcoes - 1) {
      lcd.setCursor(0, opcaoAtual - opcaoMaisAcima);
      lcd.print(" ");
      opcaoAtual++;

      /*Se a opcao selecionada for a ultima que se vê no lcd, aumentar o
      opcaoview para passar a ser a terceira, caso não seja a ultima de todas as
      opcoes*/
      if (opcaoAtual - opcaoMaisAcima == 3 && opcaoAtual < numOpcoes - 1) {
        opcaoMaisAcima++;
        lcd.clear();
        printOpcoes();
      } else {
        lcd.setCursor(0, opcaoAtual - opcaoMaisAcima);
        lcd.write(SETA_HORIZONTAL);
      }
    }
  }
}
void Opcoes::setOpcoes(int numeroOpcoes) {
  // Se listaOpcoes guardar algo
  if (listaOpcoes != NULL) {
    // Cria uma máscara de bits com o bit mais à direita a 1, que serve para
    // selecionar o bit
    //   correspondente à opção i do needsFreeBoolArray
    uint16_t mask = 1;

    // Para cada opção na lista
    for (int i = 0; i < numOpcoes; i++) {
      // Se ela precisar de ser libertada
      if (needsFreeBoolArray & mask) {
        delete[] listaOpcoes[i];  // Liberta-a
        listaOpcoes[i] = NULL;    // e assegura-se que fica a NULL
      }

      // Desloca a máscara para a esquerda
      mask = mask << 1;
    }

    // Liberta o vetor
    delete[] listaOpcoes;
  }

  numOpcoes = numeroOpcoes;
  opcaoAtual = 0;
  opcaoMaisAcima = 0;

  listaOpcoes = new char*[numeroOpcoes];
}

void Opcoes::setConstOption(uint8_t num, const char* str) {
  // Coloca o bit correspondente a esta opcao a 0 (não necessita free, porque
  // não foi alocada memoria)
  needsFreeBoolArray &= ~(1 << num);

  listaOpcoes[num] = (char*)str;
}

// Cria uma opcao com string alocada dinamicamente
// Aloca memoria com len+1 bytes para a string e copia a string src
void Opcoes::setDynOptionCpy(uint8_t num, char* src, uint8_t len) {
  // Coloca o bit correspondente a esta opcao a 1 (necessita free, porque foi
  // alocada memoria)
  needsFreeBoolArray |= (1 << num);

  // Aloca memoria e copia a string
  char* ptr = new char[(len + 1) * sizeof(char)];
  strcpy(ptr, src);
  listaOpcoes[num] = ptr;
}