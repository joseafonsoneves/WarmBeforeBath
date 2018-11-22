/*
        estado.h - Gestão do funcionamento do aquecedor
        Criada por José Neves e Miguel Fazenda, 18 de março de 2018
        BitChallenge 2018
*/

#ifndef ESTADO_H
#define ESTADO_H

#include <DS3231.h>
#include "memoria.h"
#include "relogio.h"

#define ESTADO_NADA 1
#define ESTADO_TEMPORIZADOR 2
#define ESTADO_AQUECE_E_MANTEM 3

// Metade da dimensão do intervalo de temperaturas centrado na tempAManter e no
// qual o aquecedor se encontra desligado
#define PRECISAO_TEMP 2

class Estado {
 public:
  bool isAquecedorLigado = false;
  bool estadoMudou = false;
  uint8_t estadoCodigo = ESTADO_NADA;
  uint8_t tempAManter;
  uint8_t minutoAnterior;
  DateTime horaDeParagem;

  void setEstadoTemporizador(DS3231* rtc, uint8_t minutos);
  void comparaHoraELigaAquecedor(DS3231* rtc,
                                 HORA* rtcHora,
                                 HORA* hora,
                                 uint8_t minutos,
                                 uint8_t temperatura);
  void checkHoraDeAquecer(DS3231* rtc);

  void estadoNada(DS3231* rtc);
  void estadoTemporizador(DS3231* rtc);
  void estadoAqueceMantem(float temp, DS3231* rtc);
  void loop(float temp, DS3231* rtc);

 private:
  void mantemTemp(float temp);
};

#endif