/*
        input.cpp - Gestão dos inputs dados pelo utilizador
        Criada por José Neves e Miguel Fazenda, 26 de março de 2018
        BitChallenge 2018
*/

#include "input.h"
#include <Arduino.h>

void Input::init() {
  // Inicialização dos botões
  pinMode(BOTAO0_PIN, INPUT_PULLUP);
  pinMode(BOTAO1_PIN, INPUT_PULLUP);
  pinMode(BOTAO2_PIN, INPUT_PULLUP);
  pinMode(BOTAO3_PIN, INPUT_PULLUP);
  pinMode(BOTAOROTARY_PIN, INPUT);
  // Inicialização do rotary encoder
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  for (unsigned int i = 0; i < NUM_BOTOES; i++) {
    botoes[i].estadoBotaoAnt = 0;
    botoes[i].estadoBotao = 0;
  }

  lastInputChangeMillis = millis();
}

// Atualiza o estado dos botões
void Input::update(LiquidCrystal_I2C* lcd) {
  // Caso se tenha rodado o rotary encoder
  if (rotationEvent != ROTATION_NONE)
    updateBacklighTimeout(lcd);

  // Guarda o estado anterior dos botões
  this->refresh();

  // Lê o estado de cada botão
  botoes[0].estadoBotao = !digitalRead(BOTAO0_PIN);
  botoes[1].estadoBotao = !digitalRead(BOTAO1_PIN);
  botoes[2].estadoBotao = !digitalRead(BOTAO2_PIN);
  botoes[3].estadoBotao = !digitalRead(BOTAO3_PIN);
  botoes[4].estadoBotao = !digitalRead(BOTAOROTARY_PIN);

  // Para cada botão
  for (unsigned int i = 0; i < NUM_BOTOES; i++)
    // Se tiver sido premido ou largado
    if (botoes[i].estadoBotaoAnt != botoes[i].estadoBotao)
      updateBacklighTimeout(lcd);
}

void Input::updateBacklighTimeout(LiquidCrystal_I2C* lcd) {
  // Guarda o tempo em milissegundos no momento
  lastInputChangeMillis = millis();
}

bool Input::checkCliqueBotao(uint8_t botao) {
  // Se o botão tiver sido clicado e no momento já se encontrar largado
  if (botoes[botao - 1].estadoBotao == true &&
      botoes[botao - 1].estadoBotaoAnt == false) {
    botoes[botao - 1].estadoBotaoAnt = 1;
    // Atualiza o estado anterior do botão
    return true;
  }

  return false;
}

// Guarda os estados dos botoes no momento anterior, para depois se poder
// compararar com o
//  estado atual
void Input::refresh() {
  for (unsigned int i = 0; i < NUM_BOTOES; i++) {
    botoes[i].estadoBotaoAnt = botoes[i].estadoBotao;
  }

  rotationEvent = ROTATION_NONE;
}

void Input::rotaryEncoderInterrupt() {
  bool p2 = digitalRead(2);

  // Conta o numero de rotações. Serve para diminuir a sensibilidade do rotary
  // encoder
  rotationCounter += (digitalRead(3) != p2) ? 1 : -1;

  if (rotationCounter >= 4) {
    rotationEvent = ROTATION_RIGHT;
    rotationCounter = 0;
  } else if (rotationCounter <= -4) {
    rotationEvent = ROTATION_LEFT;
    rotationCounter = 0;
  }
}

bool Input::rotaryAlteraVar(uint8_t* var, uint8_t varMax, uint8_t varMin) {
  if (rotationEvent == ROTATION_RIGHT) {
    // Aumenta-se a variável em 1, a não ser que ela seja igual ao máximo
    // (nesse caso, passa a ser o mínimo)
    *var = (*var < varMax) ? *var + 1 : varMin;
    return true;
  } else if (rotationEvent == ROTATION_LEFT) {
    // Diminui-se a hora em 1, a não ser que ela seja igual ao mínimo
    // (nesse caso, passa a ser o máximo)
    *var = (*var > varMin) ? *var - 1 : varMax;
    return true;
  }

  return false;
}