/*
        input.h - Gestão dos inputs dados pelo utilizador
        Criada por José Neves e Miguel Fazenda, 26 de março de 2018
        BitChallenge 2018
*/

#ifndef INPUT_H
#define INPUT_H

#include <LiquidCrystal_I2C.h>
#include <stdint.h>

#define NUM_BOTOES 5

#define BOTAO_CIMA 1
#define BOTAO_BAIXO 2
#define BOTAO_OK 3
#define BOTAO_CANCELAR 4
#define BOTAO_ROTARY 5

#define BOTAO0_PIN 12
#define BOTAO1_PIN 11
#define BOTAO2_PIN 10
#define BOTAO3_PIN 9
#define BOTAOROTARY_PIN 4

#define ROTATION_NONE 0
#define ROTATION_RIGHT 1
#define ROTATION_LEFT -1

#define RE_A_PIN 2
#define RE_B_PIN 3
#define PRECISAO 4

typedef struct botao_stru {
  uint8_t estadoBotaoAnt, estadoBotao;
} Botao;

class Input {
 public:
  int8_t rotationCounter = 0;
  int8_t rotationEvent = ROTATION_NONE;
  Botao botoes[NUM_BOTOES];

  // Guarda o tempo em que um botão foi premido (ou evento de rotacao) pela
  // ultima vez
  // para depois desligar a backlight passado X tempo
  unsigned long lastInputChangeMillis;

  void init();
  void update(LiquidCrystal_I2C* lcd);
  void refresh();

  void updateBacklighTimeout(LiquidCrystal_I2C* lcd);

  bool checkCliqueBotao(uint8_t botao);

  void rotaryEncoderInterrupt();
  bool rotaryAlteraVar(uint8_t* var, uint8_t varMax, uint8_t varMin);
};

#endif