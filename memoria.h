/*
        memoria.h - Gestão da informação armazenada na EEPROM externa
        Criada por José Neves e Miguel Fazenda, 26 de março de 2018
        BitChallenge
*/

#ifndef MEMORIA_H
#define MEMORIA_H

#include <extEEPROM.h>
#include "structs.h"

#define MAX_USERS 8
#define PAGE_SIZE 64
#define MAX_TEMP 40
#define MIN_TEMP 15

extern extEEPROM memEEPROM;

int8_t getAllUsersData(EEPROM_USER_DATA* userData);
EEPROM_USER_DATA getSelectedUser(uint8_t userSelecionado);
EEPROM_USER_DATA_OPTIONAL getUserData(String user);

short findPageForUser(String name);
void setUserData(EEPROM_USER_DATA data);
void removeUserData(String user);
void resetEEPROM();

#endif