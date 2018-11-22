/*
        estado.cpp - Gestão do funcionamento do aquecedor
        Criada por José Neves e Miguel Fazenda, 5 de fevereiro de 2018
        BitChallenge 2018
*/

#include "estado.h"

#define MINUTOS_ANTES_LIGAR_AQUECEDOR 5

DateTime calculaHoraDeParagem(DS3231* rtc, uint8_t minutos) {
  // 1 - Converte a hora e data da classe DS3231 na classe DateTime
  // 2 - Retira o respetivo tempo UNIX
  // 3 - Soma-lhe o número de segundos correspondente aos minutos
  // 4 - Constrói uma nova classe DateTime a partir do novo tempo UNIX
  return DateTime(converte_DS3231(rtc).unixtime() +
                  (minutos + MINUTOS_ANTES_LIGAR_AQUECEDOR) * 60);
}

void Estado::setEstadoTemporizador(DS3231* rtc, uint8_t minutos) {
  estadoCodigo = ESTADO_TEMPORIZADOR;
  estadoMudou = true;
  horaDeParagem = calculaHoraDeParagem(rtc, minutos);
}

void Estado::comparaHoraELigaAquecedor(DS3231* rtc,
                                       HORA* rtcHora,
                                       HORA* hora,
                                       uint8_t numMinutos,
                                       uint8_t temperatura) {
  if (rtcHora->hora == hora->hora && rtcHora->minuto == hora->minuto) {
    DateTime novaHoraDeParagem = calculaHoraDeParagem(rtc, numMinutos);

    // Se se está no estado NADA (o aquecedor não está ligado) ou
    // se o aquecedor está ligado, chegou à hora de início do utilizador i e a
    // hora de paragem deste é posterior à que estava agendada, define a nova
    // hora de paragem do aquecedor
    if (estadoCodigo == ESTADO_NADA ||
        (novaHoraDeParagem.unixtime() > horaDeParagem.unixtime())) {
      estadoCodigo = ESTADO_AQUECE_E_MANTEM;
      estadoMudou = true;
      tempAManter = temperatura;
      horaDeParagem = novaHoraDeParagem;
    }
  }
}

// Verifica se chegou a alguma hora agendada para ligar o aquecedor
void Estado::checkHoraDeAquecer(DS3231* rtc) {
  // Se o minutoAnterior for diferente do atual, mudou de minuto, assim o corpo
  // do if só corre uma vez em cada minuto

  if (minutoAnterior != rtc->getSecond()) {
    bool a;
    HORA rtcHora(rtc->getHour(a, a), rtc->getMinute());
    EEPROM_USER_DATA users[MAX_USERS];
    uint8_t usersCount = getAllUsersData(users);

    // Percorre todos os utilizadores
    for (uint8_t i = 0; i < usersCount; i++) {
      // Obtem cada um a hora de aquecer naquele dia
      // (se a hora for 25 significa que não tem alarme agendado)
      HORA hora = users[i].horaPorDiaSemana[rtc->getDoW()];
      bool mudouDeDia;
      if (hora.hora < 25) {
        HORA horaAIniciar =
            hora.subtraiMinutos(MINUTOS_ANTES_LIGAR_AQUECEDOR, &mudouDeDia);

        if (!mudouDeDia) {
          // Se a hora coincide com a atual
          comparaHoraELigaAquecedor(rtc, &rtcHora, &horaAIniciar,
                                    users[i].numMin, users[i].temperature);
        }
      }

      if (rtcHora.hora == 23 &&
          rtcHora.minuto >= 60 - MINUTOS_ANTES_LIGAR_AQUECEDOR) {
        // Verifica se no dia atual é suposto ligar o aquecedor para estar
        // quente no dia seguinte
        // (ex: se estiver programado para estar quente à 00:03 do dia seguinte
        // pode ter de ligar às 23:58 do dia atual) Apenas é relevante a partir
        // das 23 e 55 (com MINUTOS_ANTES_LIGAR_AQUECEDOR a 5 minutos)
        hora = users[i].horaPorDiaSemana[(rtc->getDoW() + 1) % 7];
        if (hora.hora < 25) {
          HORA horaAIniciar =
              hora.subtraiMinutos(MINUTOS_ANTES_LIGAR_AQUECEDOR, &mudouDeDia);

          if (mudouDeDia) {
            // Se a hora coincide com a atual
            comparaHoraELigaAquecedor(rtc, &rtcHora, &horaAIniciar,
                                      users[i].numMin, users[i].temperature);
          }
        }
      }
    }
  }
  minutoAnterior = rtc->getSecond();
}

void Estado::estadoNada(DS3231* rtc) {
  isAquecedorLigado = false;
}
void Estado::estadoTemporizador(DS3231* rtc) {
  // Se ainda não se tiver atingido a hora de paragem
  if (!DateTime_antes_DS3231(rtc, &horaDeParagem))
    isAquecedorLigado = true;
  else {
    estadoCodigo = ESTADO_NADA;
    estadoMudou = true;
  }
}
void Estado::estadoAqueceMantem(float temp, DS3231* rtc) {
  // Se ainda não se tiver atingido a hora de paragem
  if (!DateTime_antes_DS3231(rtc, &horaDeParagem))
    mantemTemp(temp);
  else {
    estadoCodigo = ESTADO_NADA;
    estadoMudou = true;
  }
}
void Estado::loop(float temp, DS3231* rtc) {
  checkHoraDeAquecer(rtc);

  switch (estadoCodigo) {
    case ESTADO_NADA:
      estadoNada(rtc);
      break;
    case ESTADO_TEMPORIZADOR:
      estadoTemporizador(rtc);
      break;
    case ESTADO_AQUECE_E_MANTEM:
      estadoAqueceMantem(temp, rtc);
      break;
  }
}

void Estado::mantemTemp(float temp) {
  // Se o aquecedor está desligado
  if (!isAquecedorLigado) {
    // e a temperatura atual estiver abaixo da mínima
    if (temp <= tempAManter - PRECISAO_TEMP)
      // Liga o aquecedor
      isAquecedorLigado = true;
  }
  // Se o aquecedor está ligado e a temperatura acima da máxima
  else if (temp > tempAManter + PRECISAO_TEMP)
    // Desliga o aquecedor
    isAquecedorLigado = false;
}