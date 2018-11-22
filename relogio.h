/*
        relogio.h - Gestão da informação relacionada com o RTC
        Criada por José Neves e Miguel Fazenda, 18 de março de 2018
        BitChallenge 2018
*/

#ifndef RELOGIO_H
#define RELOGIO_H

#include <stdint.h>

#include <DS3231.h>
#include "structs.h"

bool verificaDataHora(DATE* data, HORA* hora);

bool DateTime_antes_DS3231(DS3231* rtc, DateTime* dataHora);

DateTime converte_DS3231(DS3231* rtc);

#endif