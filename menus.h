/*
        menus.h - Comunicação utilizador-dispositivo
        Criada por José Neves e Miguel Fazenda, 5 de fevereiro de 2018
        BitChallenge 2018
*/

#ifndef MENUS_H
#define MENUS_H

#include <dht.h>
#include "estado.h"
#include "memoria.h"
#include "opcoes.h"

#define MENU_INICIAL 0
#define MENU_OPCOES 1
#define MENU_TEMP_DESEJADA 2
#define MENU_SET_USER_NUM_MIN 3
#define MENU_HORA_DESEJADA 4
#define MENU_UTILIZADORES 5
#define MENU_UTILIZADOR 6
#define MENU_CONFIG_DATA_HORA 7
#define MENU_CRIAR_UTILIZADOR 8
#define MENU_MODO_RAPIDO 9
#define MENU_RESET_CONFIRMAR 10

#define OPCOES 0
#define MODO_RAPIDO 1
#define MUDAR_USER_TEMP 8
#define MUDAR_USER_NUM_MIN 9
#define ELIMINAR_USER 10

#define DELAY_TIME 1000
#define PISCAR_CAMPO millis() % 800 < 700
// O campo acenderá durante 700ms por cada 800ms

class Menus {
 public:
  uint8_t menuId;
  uint8_t lastMenuId;
  uint8_t campoAtual;

  DATE data;
  HORA hora;
  uint8_t doW;

  Opcoes opcoes;

  char* strIns = NULL;

  // Gerais
  void init();
  void loop(dht* DHT);
  void setMenu(int menu);
  void setMenuBack();
  void printFuncionalidadeFutura();

  // Home
  void menuInicialInit();
  void menuInicialLoop(dht* DHT);

  // Opções Principais
  void menuOpcoesInit();
  void menuOpcoesLoop();

  // Seleção de Variáveis
  void menuTempDesejadaInit();
  void menuTempDesejadaLoop();

  void menuNumMinutosDesejadoInit();
  void menuNumMinutosDesejadoLoop();

  void menuHoraDesejadaInit();
  void menuHoraDesejadaLoop();

  // Utilizadores
  void menuUtilizadoresInit();
  void menuUtilizadoresLoop();

  uint8_t utilizadorSelecionado;
  void menuUtilizadorInit();
  void menuUtilizadorLoop();

  void menuCriarUtilizadorInit();
  void menuCriarUtilizadorLoop();

  // Acerta Relógio
  void menuConfigDataHoraInit();
  void menuConfigDataHoraLoop();

  // Reset
  void menuResetConfirmarInit();
  void menuResetConfirmarLoop();
};

extern LiquidCrystal_I2C lcd;
extern Menus menus;
extern DS3231 rtc;
extern Estado estado;

#endif
