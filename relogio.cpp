/*
        relogio.cpp - Gestão da informação relacionada com o RTC
        Criada por José Neves e Miguel Fazenda, 18 de março de 2018
        BitChallenge 2018
*/

#include "relogio.h"

bool verificaDataHora(DATE* data, HORA* hora) {
  bool bissexto = data->ano % 4 == 0;

  if (data->mes == 2) {
    if (data->dia > 28 && !bissexto)
      return false;
    if (data->dia > 29 && bissexto)
      return false;
  }

  if (data->mes == 4 || data->mes == 6 || data->mes == 9 || data->mes == 11)
    if (data->dia > 30)
      return false;

  return true;
}

bool DateTime_antes_DS3231(DS3231* rtc, DateTime* dataHora) {
  bool a = false;

  if (dataHora->year() - 2000 < rtc->getYear())
    return true;
  else if (dataHora->year() - 2000 == rtc->getYear()) {
    if (dataHora->month() < rtc->getMonth(a))
      return true;
    else if (dataHora->month() == rtc->getMonth(a)) {
      if (dataHora->day() < rtc->getDate())
        return true;
      else if (dataHora->day() == rtc->getDate()) {
        if (dataHora->hour() < rtc->getHour(a, a))
          return true;
        else if (dataHora->hour() == rtc->getHour(a, a)) {
          if (dataHora->minute() < rtc->getMinute())
            return true;
          else if (dataHora->minute() == rtc->getMinute()) {
            if (dataHora->second() < rtc->getSecond())
              return true;
          }
        }
      }
    }
  }

  return false;
}

DateTime converte_DS3231(DS3231* rtc) {
  bool a;

  return DateTime(rtc->getYear() + 2000, rtc->getMonth(a), rtc->getDate(),
                  rtc->getHour(a, a), rtc->getMinute(), rtc->getSecond());
}
