/*
        memoria.cpp - Gestão da informação armazenada na EEPROM externa
        Criada por José Neves e Miguel Fazenda, 26 de março de 2018
        BitChallenge 2018
*/

#include "memoria.h"

#define PAGE_NETWORK_CONNECTION_SETTINGS 128
#define PAGE_USER_FIRST_CONNECTION_SETTINGS 1024

extern extEEPROM memEEPROM;

// Retorna os dados de todos os utilizadores na EEPROM
int8_t getAllUsersData(EEPROM_USER_DATA* userData) {
  int8_t counter = 0;
  EEPROM_USER_DATA data;

  // Percorre todos os espaços da EEPROM que possam corresponder a utilizadores
  for (uint8_t userNum = 0; userNum < MAX_USERS; userNum++) {
    // Guarda em data o que lá está
    memEEPROM.read(PAGE_USER_FIRST_CONNECTION_SETTINGS + userNum * PAGE_SIZE,
                   (byte*)&data, sizeof(EEPROM_USER_DATA));

    // Se estiver lá um utilizador
    if (data.userName[0] != 0) {
      // e o apontador dado apontar para algum endereço
      if (userData != NULL)
        userData[counter] = data;  // Guarda-o no próximo lugar do vetor

      // Atualiza o número de utilizadores encontrados
      counter++;
    }
  }

  return counter;
}
// Retorna os dados de um utilizador a partir do seu número
EEPROM_USER_DATA getSelectedUser(uint8_t userSelecionado) {
  EEPROM_USER_DATA users[MAX_USERS];

  getAllUsersData(users);

  return users[userSelecionado];
}
// Retorna os dados de um utilizador a partir do seu nome ou um utilizador vazio
EEPROM_USER_DATA_OPTIONAL getUserData(String user) {
  EEPROM_USER_DATA data;

  // Percorre todos os espaços da EEPROM que possam corresponder a utilizadores
  for (uint8_t userPageNum = 0; userPageNum < MAX_USERS; userPageNum++) {
    // Guarda em data o que lá está
    memEEPROM.read(
        PAGE_USER_FIRST_CONNECTION_SETTINGS + userPageNum * PAGE_SIZE,
        (byte*)&data, sizeof(EEPROM_USER_DATA));

    // Se os nomes corresponderem
    if (user.equalsIgnoreCase(data.userName))
      // Retorna os seus dados
      return EEPROM_USER_DATA_OPTIONAL{true, userPageNum, data};
  }

  // Se não tiver encontrado um utilizador com esse nome, retorna um vazio
  return EEPROM_USER_DATA_OPTIONAL{
      false, -1,
      EEPROM_USER_DATA{
          "", {HORA(), HORA(), HORA(), HORA(), HORA(), HORA(), HORA()}, 0, 0}};
}

//  Devolve um sitio na EEPROM para escrever um utilizador, o sítio onde ele
//  está se já estiver na EEPROM ou -1 se não existir espaço
short findPageForUser(String user) {
  EEPROM_USER_DATA_OPTIONAL foundUser = getUserData(user);

  // Se o utilizador existir
  if (foundUser.userExists)
    // Retorna a sua página
    return foundUser.userPageNum;
  else {
    EEPROM_USER_DATA data;

    // Percorre todos os espaços da EEPROM que possam corresponder a
    // utilizadores
    for (uint8_t userPageNum = 0; userPageNum < MAX_USERS; userPageNum++) {
      // Guarda em data o que lá está
      memEEPROM.read(
          PAGE_USER_FIRST_CONNECTION_SETTINGS + userPageNum * PAGE_SIZE,
          (byte*)&data, sizeof(EEPROM_USER_DATA));

      // Se não estiver lá um utilizador
      if (strlen(data.userName) == 0)
        // Retorna a página
        return userPageNum;
    }
  }

  // Se estiver cheio, retorna -1
  return -1;
}

// Escreve os dados de um utilizador já existente ou de um novo
void setUserData(EEPROM_USER_DATA data) {
  uint8_t pageForUser = findPageForUser(data.userName);

  // Se se tiver encontrado uma página
  if (pageForUser != -1)
    // Escreve lá os dados do utilizador
    memEEPROM.write(
        PAGE_USER_FIRST_CONNECTION_SETTINGS + pageForUser * PAGE_SIZE,
        (byte*)&data, sizeof(EEPROM_USER_DATA));
}

void removeUserData(String user) {
  EEPROM_USER_DATA_OPTIONAL foundUser = getUserData(user);

  // Se se tiver encontrado o utilizador pretendido
  if (foundUser.userPageNum != -1) {
    // Declara dados de utilizador vazios
    EEPROM_USER_DATA userEmptyData = EEPROM_USER_DATA{
        "", {HORA(), HORA(), HORA(), HORA(), HORA(), HORA(), HORA()}, 0, 0};
    // Escreve-os no espaço do utilizador
    memEEPROM.write(
        PAGE_USER_FIRST_CONNECTION_SETTINGS + foundUser.userPageNum * PAGE_SIZE,
        (byte*)&userEmptyData, sizeof(EEPROM_USER_DATA));
  }
}

// Remove todos os utilizadores
void resetEEPROM() {
  // Declara dados de utilizador vazios
  EEPROM_USER_DATA userEmptyData = EEPROM_USER_DATA{
      "", {HORA(), HORA(), HORA(), HORA(), HORA(), HORA(), HORA()}, 0, 0};

  // Percorre todos os espaços da EEPROM que possam corresponder a
  // utilizadores
  for (uint8_t userPageNum = 0; userPageNum < MAX_USERS; userPageNum++) {
    // Escreve os dados vazios no espaço em que está
    memEEPROM.write(
        PAGE_USER_FIRST_CONNECTION_SETTINGS + userPageNum * PAGE_SIZE,
        (byte*)&userEmptyData, sizeof(EEPROM_USER_DATA));
  }
}