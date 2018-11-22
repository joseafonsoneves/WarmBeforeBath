/*
        menus.cpp - Comunicação utilizador-dispositivo
        Criada por José Neves e Miguel Fazenda, 5 de fevereiro de 2018
        BitChallenge 2018
*/

#include "menus.h"

extern LiquidCrystal_I2C lcd;
extern Menus menus;
extern DS3231 rtc;
extern Estado estado;
extern Input input;

// Gerais
void Menus::init() {
  uint8_t setaVerticalMap[] = SETA_VERTICAL_MAP;
  lcd.createChar(SETA_VERTICAL, setaVerticalMap);

  opcoes = Opcoes();
  lastMenuId = MENU_INICIAL;

  setMenu(MENU_INICIAL);
}
void Menus::loop(dht* DHT) {
  switch (menuId) {
    case MENU_INICIAL:
      menus.menuInicialLoop(DHT);
      break;
    case MENU_OPCOES:
      menus.menuOpcoesLoop();
      break;
    case MENU_HORA_DESEJADA:
      menus.menuHoraDesejadaLoop();
      break;
    case MENU_UTILIZADORES:
      menuUtilizadoresLoop();
      break;
    case MENU_UTILIZADOR:
      menuUtilizadorLoop();
      break;
    case MENU_TEMP_DESEJADA:
      menus.menuTempDesejadaLoop();
      break;
    case MENU_CRIAR_UTILIZADOR:
      menuCriarUtilizadorLoop();
      break;
    case MENU_CONFIG_DATA_HORA:
      menuConfigDataHoraLoop();
      break;
    case MENU_SET_USER_NUM_MIN:
      menuNumMinutosDesejadoLoop();
      break;
    case MENU_MODO_RAPIDO:
      menuNumMinutosDesejadoLoop();
      break;
    case MENU_RESET_CONFIRMAR:
      menuResetConfirmarLoop();
  }
}
void Menus::setMenu(int menu) {
  lcd.clear();

  lastMenuId = menuId;
  menuId = menu;

  switch (menu) {
    case MENU_INICIAL:
      menuInicialInit();
      break;
    case MENU_OPCOES:
      menuOpcoesInit();
      break;
    case MENU_HORA_DESEJADA:
      menuHoraDesejadaInit();
      break;
    case MENU_TEMP_DESEJADA:
      menuTempDesejadaInit();
      break;
    case MENU_UTILIZADORES:
      menuUtilizadoresInit();
      break;
    case MENU_UTILIZADOR:
      menuUtilizadorInit();
      break;
    case MENU_CRIAR_UTILIZADOR:
      menuCriarUtilizadorInit();
      break;
    case MENU_CONFIG_DATA_HORA:
      menuConfigDataHoraInit();
      break;
    case MENU_SET_USER_NUM_MIN:
      menuNumMinutosDesejadoInit();
      break;
    case MENU_MODO_RAPIDO:
      menuNumMinutosDesejadoInit();
      break;
    case MENU_RESET_CONFIRMAR:
      menuResetConfirmarInit();
  }
}
void Menus::setMenuBack() {
  setMenu(lastMenuId);
}
void Menus::printFuncionalidadeFutura() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print(F("Funcionalidade"));
  lcd.setCursor(0, 2);
  lcd.print(F("Futura"));
  delay(DELAY_TIME);
  setMenu(MENU_OPCOES);
}

// Menu Inicial
void menuInicialDrawEstado() {
  lcd.setCursor(0, 2);  // Coloca o cursor no início da 3a fila
  switch (estado.estadoCodigo) {
    case ESTADO_NADA:
      lcd.print(F("                   "));
      lcd.setCursor(9, 3);  // Coloca o cursor no lugar da segunda opção
      lcd.print(F("Modo Rapido"));
      break;
    case ESTADO_TEMPORIZADOR:
      lcd.print(F("Ate as "));
      printHora(estado.horaDeParagem.hour(), estado.horaDeParagem.minute(),
                NAO_PISCAR, false);
      lcd.print(F("      "));
      lcd.setCursor(9, 3);  // Coloca o cursor no lugar da segunda opção
      lcd.print(F("Desligar   "));
      break;
    case ESTADO_AQUECE_E_MANTEM:
      lcd.print(F("Ate as "));
      printHora(estado.horaDeParagem.hour(), estado.horaDeParagem.minute(),
                NAO_PISCAR, false);
      lcd.print(F(" a "));
      printTemperature(estado.tempAManter);
      lcd.setCursor(9, 3);  // Coloca o cursor no lugar da segunda opção
      lcd.print(F("Cancelar   "));
      break;
  }
}
void Menus::menuInicialInit() {
  campoAtual = OPCOES;

  menuInicialDrawEstado();
}
void Menus::menuInicialLoop(dht* DHT) {
  bool a;

  lcd.setCursor(2, 0);
  printDoW(rtc.getDoW(), NAO_PISCAR, false);
  lcd.print(F(" "));
  printData(rtc.getYear(), rtc.getMonth(a), rtc.getDate(), NAO_PISCAR, false);
  lcd.setCursor(0, 1);
  printTemperature(DHT->temperature);
  lcd.setCursor(7, 1);
  printHora(rtc.getHour(a, a), rtc.getMinute(), NAO_PISCAR, false);
  lcd.setCursor(17, 1);
  printHumidity(DHT->humidity);
  lcd.setCursor(campoAtual == OPCOES ? 0 : 8, 3);
  lcd.write(SETA_HORIZONTAL);
  lcd.setCursor(1, 3);
  lcd.print(F("Opcoes"));

  if (estado.estadoMudou) {
    menuInicialDrawEstado();
    estado.estadoMudou = false;
  }

  if (input.checkCliqueBotao(BOTAO_BAIXO) ||
      input.rotationEvent == ROTATION_RIGHT) {
    campoAtual = MODO_RAPIDO;
    lcd.setCursor(0, 3);
    lcd.print(F(" "));  // Apagar a seta
  }
  if (input.checkCliqueBotao(BOTAO_CIMA) ||
      input.rotationEvent == ROTATION_LEFT) {
    campoAtual = OPCOES;
    lcd.setCursor(8, 3);
    lcd.print(F(" "));  // Apagar a seta
  }
  if (input.checkCliqueBotao(BOTAO_OK) ||
      input.checkCliqueBotao(BOTAO_ROTARY)) {
    if (campoAtual == OPCOES)
      setMenu(MENU_OPCOES);
    else {
      if (estado.estadoCodigo == ESTADO_NADA)
        setMenu(MENU_MODO_RAPIDO);
      else {
        estado.estadoCodigo = ESTADO_NADA;
        estado.estadoMudou = true;
      }
    }
  }
}

// Opções principais
void Menus::menuOpcoesInit() {
  opcoes.setOpcoes(6);

  opcoes.setConstOption(0, "Utilizadores");
  opcoes.setConstOption(1, "Config Data/Hora");
  opcoes.setConstOption(2, "Configurar Wi-fi");
  opcoes.setConstOption(3, "Idioma");
  opcoes.setConstOption(4, "Modo de desligar");
  opcoes.setConstOption(5, "RESET");

  opcoes.printOpcoes();
}

void Menus::menuOpcoesLoop() {
  opcoes.slideThroughOpcoes();

  if (input.checkCliqueBotao(BOTAO_OK) || input.checkCliqueBotao(BOTAO_ROTARY))
    switch (opcoes.opcaoAtual) {
      case 0:
        setMenu(MENU_UTILIZADORES);
        break;
      case 1:
        setMenu(MENU_CONFIG_DATA_HORA);
        break;
      case 5:
        setMenu(MENU_RESET_CONFIRMAR);
        break;
      default:
        printFuncionalidadeFutura();
        break;
    }
  if (input.checkCliqueBotao(BOTAO_CANCELAR))
    setMenu(MENU_INICIAL);
}

// Seleção de Variáveis
void Menus::menuTempDesejadaInit() {
  // Reutilização de variáveis para poupança de memória
  campoAtual = getSelectedUser(utilizadorSelecionado).temperature;
}
void Menus::menuTempDesejadaLoop() {
  lcd.setCursor(7, 1);
  printTemperature(campoAtual);

  input.rotaryAlteraVar(&campoAtual, MAX_TEMP, MIN_TEMP);

  if (input.checkCliqueBotao(BOTAO_OK) ||
      input.checkCliqueBotao(BOTAO_ROTARY)) {
    EEPROM_USER_DATA user = getSelectedUser(utilizadorSelecionado);
    user.temperature = campoAtual;
    setUserData(user);
    setMenuBack();
  }

  if (input.checkCliqueBotao(BOTAO_CANCELAR))
    setMenuBack();
}
void Menus::menuNumMinutosDesejadoInit() {
  // Reutilização de variáveis para poupança de memória
  if (menuId == MENU_SET_USER_NUM_MIN)
    campoAtual = getSelectedUser(utilizadorSelecionado).numMin;
  else if (menuId == MENU_MODO_RAPIDO)
    campoAtual = 30;
}
void Menus::menuNumMinutosDesejadoLoop() {
  lcd.setCursor(7, 1);
  print_Int(campoAtual, 3);
  lcd.print(F("min"));

  input.rotaryAlteraVar(&campoAtual, 120, 5);

  if (input.checkCliqueBotao(BOTAO_OK) ||
      input.checkCliqueBotao(BOTAO_ROTARY)) {
    if (menuId == MENU_SET_USER_NUM_MIN) {
      EEPROM_USER_DATA user = getSelectedUser(utilizadorSelecionado);
      user.numMin = campoAtual;
      setUserData(user);
      setMenuBack();
    } else if (menuId == MENU_MODO_RAPIDO) {
      estado.setEstadoTemporizador(&rtc, campoAtual);
      setMenuBack();
    }
  }

  if (input.checkCliqueBotao(BOTAO_CANCELAR))
    setMenuBack();
}

void Menus::menuHoraDesejadaInit() {
  hora = getSelectedUser(utilizadorSelecionado).horaPorDiaSemana[doW];

  if (hora.hora == 25) {
    hora.hora = 0;
    hora.minuto = 0;
  }

  campoAtual = CAMPO_HORA;
}
void leaveMenuHoraIniciar(HORA hora) {
  EEPROM_USER_DATA user = getSelectedUser(menus.utilizadorSelecionado);
  user.horaPorDiaSemana[menus.doW] = hora;
  setUserData(user);
  menus.setMenu(MENU_UTILIZADOR);
}
void Menus::menuHoraDesejadaLoop() {
  bool aceso = PISCAR_CAMPO;

  lcd.setCursor(8, 0);
  printHora(hora.hora, hora.minuto, campoAtual, aceso);
  lcd.setCursor(0, 1);
  lcd.print(F("OK - Sai e Guarda"));
  lcd.setCursor(0, 2);
  lcd.print(F("C - Sai e elimina"));
  lcd.setCursor(4, 3);
  lcd.print(F("alarme"));

  if (input.checkCliqueBotao(BOTAO_BAIXO))
    campoAtual = CAMPO_MINUTO;
  if (input.checkCliqueBotao(BOTAO_CIMA))
    campoAtual = CAMPO_HORA;
  if (input.checkCliqueBotao(BOTAO_OK))
    leaveMenuHoraIniciar(hora);
  if (input.checkCliqueBotao(BOTAO_CANCELAR))
    leaveMenuHoraIniciar(HORA{25, 25});

  if (input.checkCliqueBotao(BOTAO_ROTARY)) {
    if (campoAtual == CAMPO_HORA)
      campoAtual = CAMPO_MINUTO;
    else if (campoAtual == CAMPO_MINUTO)
      leaveMenuHoraIniciar(hora);
  }
  if (campoAtual == CAMPO_HORA)
    input.rotaryAlteraVar(&hora.hora, 23, 0);
  else if (campoAtual == CAMPO_MINUTO)
    input.rotaryAlteraVar(&hora.minuto, 59, 0);
}

// UTILIZADORES
void Menus::menuUtilizadoresInit() {
  EEPROM_USER_DATA users[MAX_USERS];
  uint8_t usersCount = getAllUsersData(users);

  opcoes.setOpcoes(usersCount + 1);

  for (uint8_t i = 0; i < usersCount; i++)
    opcoes.setDynOptionCpy(i, users[i].userName, strlen(users[i].userName));

  opcoes.setConstOption(usersCount,
                        usersCount == MAX_USERS ? "CHEIO" : "NOVO UTILIZADOR");

  opcoes.printOpcoes();
}

void Menus::menuUtilizadoresLoop() {
  opcoes.slideThroughOpcoes();

  if (input.checkCliqueBotao(BOTAO_OK) ||
      input.checkCliqueBotao(BOTAO_ROTARY)) {
    // Caso seja a última opcao a selecionada (NOVO UTILIZADOR)
    if (opcoes.opcaoAtual == opcoes.numOpcoes - 1) {
      EEPROM_USER_DATA users[MAX_USERS];
      uint8_t usersCount = getAllUsersData(users);

      if (usersCount < MAX_USERS)
        setMenu(MENU_CRIAR_UTILIZADOR);
    } else {
      utilizadorSelecionado = opcoes.opcaoAtual;
      setMenu(MENU_UTILIZADOR);
    }
  }
  if (input.checkCliqueBotao(BOTAO_CANCELAR))
    setMenu(MENU_OPCOES);
}

void Menus::menuUtilizadorInit() {
  String buf;

  EEPROM_USER_DATA user = getSelectedUser(utilizadorSelecionado);

  opcoes.setOpcoes(11);

  // Cria as strings para a opcao da temperatura e dos minutos
  buf = String(user.temperature);
  buf.concat(char(223));
  buf.concat("C");
  opcoes.setDynOptionCpy(MUDAR_USER_TEMP, (char*)buf.c_str(), buf.length());

  buf = String(user.numMin);
  buf.concat("min");
  opcoes.setDynOptionCpy(MUDAR_USER_NUM_MIN, (char*)buf.c_str(), buf.length());

  opcoes.setDynOptionCpy(0, user.userName, strlen(user.userName));
  for (uint8_t d = 0; d < 7; d++) {
    int8_t hora = user.horaPorDiaSemana[d].hora;
    int8_t minuto = user.horaPorDiaSemana[d].minuto;

    buf = getMes_ou_DoW(d, DOW);
    if (hora == 25) {
      buf.concat(" Sem alarme");
    } else {
      buf.concat(" ");
      buf.concat(Int_To_Str(hora, 2));
      buf.concat(":");
      buf.concat(Int_To_Str(minuto, 2));
    }

    opcoes.setDynOptionCpy(d + 1, (char*)buf.c_str(), buf.length());
  }
  opcoes.setConstOption(ELIMINAR_USER, "ELIMINAR");

  opcoes.printOpcoes();
  doW = 0;
}
void Menus::menuUtilizadorLoop() {
  opcoes.slideThroughOpcoes();

  if (input.checkCliqueBotao(BOTAO_OK) ||
      input.checkCliqueBotao(BOTAO_ROTARY)) {
    switch (opcoes.opcaoAtual) {
      // As outras opcões que existem no menu são dias da semana
      case 0:
        break;
      default:
        doW = opcoes.opcaoAtual - 1;
        setMenu(MENU_HORA_DESEJADA);
        break;
      case MUDAR_USER_TEMP:
        setMenu(MENU_TEMP_DESEJADA);
        break;
      case MUDAR_USER_NUM_MIN:
        setMenu(MENU_SET_USER_NUM_MIN);
        break;
      case ELIMINAR_USER:
        removeUserData(opcoes.listaOpcoes[0]);
        setMenu(MENU_UTILIZADORES);
        break;
    }
  }
  if (input.checkCliqueBotao(BOTAO_CANCELAR))
    setMenu(MENU_UTILIZADORES);
}

void Menus::menuCriarUtilizadorInit() {
  if (strIns != NULL) {
    delete[] strIns;
    strIns = NULL;
  }
  if (strIns == NULL)
    strIns = new char[DIM_USERNAME + 1];

  for (uint8_t i = 0; i <= DIM_USERNAME + 1; i++)
    strIns[i] = '\0';

  strIns[0] = 'A';

  campoAtual = 0;

  lcd.setCursor(0, 3);
  lcd.print(F("Nao pode ter espacos"));
}
void leaveMenuCriarUtilizador(char* strIns[]) {
  delete[](*strIns);
  *strIns = NULL;
  menus.setMenu(MENU_UTILIZADORES);
}
// Guarda o novo utilizador. Caso já exista devolve false
bool saveNovoUtilizador(char strIns[]) {
  EEPROM_USER_DATA users[MAX_USERS];
  uint8_t usersCount = getAllUsersData(users);

  for (uint8_t i = 0; i < usersCount; i++) {
    if (strcmp(users[i].userName, strIns) == 0) {
      lcd.clear();
      lcd.home();
      lcd.print(F("Nome ja utilizado"));
      delay(DELAY_TIME);
      lcd.clear();
      return false;
    }
  }

  HORA a = HORA{25, 25};
  EEPROM_USER_DATA userData = {"", {a, a, a, a, a, a, a}, 20, 30};

  strcpy(userData.userName, strIns);
  setUserData(userData);
  return true;
}
void Menus::menuCriarUtilizadorLoop() {
  lcd.setCursor(2, 0);
  printUserName(strIns);
  lcd.setCursor(2 + campoAtual, 1);
  lcd.write(SETA_VERTICAL);

  // Se se estiver na primeira posição
  if (campoAtual == 0)
    // Altera a letra selecionada de A a Z
    input.rotaryAlteraVar((uint8_t*)(strIns + campoAtual), 'Z', 'A');
  else {
    // Altera a letra selecionada de A a Z e espaço branco
    input.rotaryAlteraVar((uint8_t*)(strIns + campoAtual), 'Z', 'A' - 1);
    // Se passou para cima do espaço vazio
    if (strIns[campoAtual] == '\0' + 1)
      strIns[campoAtual] = 'A';  // Coloca um A
    // Se chegou ao caracter depois do Z ou antes do A
    else if (strIns[campoAtual] < 'A')
      strIns[campoAtual] = '\0';  // Mete um espaço vazio
  }

  if (input.checkCliqueBotao(BOTAO_ROTARY)) {
    // Se estiver num espaço em branco ou na última posição
    if (strIns[campoAtual] == '\0' || campoAtual == DIM_USERNAME - 1) {
      // Se conseguir criar um novo utilizador
      if (saveNovoUtilizador(strIns))
        leaveMenuCriarUtilizador(&strIns);
    } else {
      campoAtual = strIns[campoAtual] == '\0' || campoAtual == DIM_USERNAME - 1
                       ? campoAtual
                       : campoAtual + 1;  // Avança a seta uma posição
      lcd.setCursor(2 + campoAtual - 1,
                    1);  // Coloca o cursor na posição anterior da seta
      lcd.print(" ");    // Apaga a seta
    }
  }

  if (input.checkCliqueBotao(BOTAO_CIMA)) {
    campoAtual = campoAtual == 0 ? 0 : campoAtual - 1;
    lcd.setCursor(2 + campoAtual + 1, 1);
    lcd.print(" ");
  }
  if (input.checkCliqueBotao(BOTAO_BAIXO)) {
    campoAtual = strIns[campoAtual] == '\0' || campoAtual == DIM_USERNAME - 1
                     ? campoAtual
                     : campoAtual + 1;
    lcd.setCursor(2 + campoAtual - 1, 1);
    lcd.print(" ");
  }

  if (input.checkCliqueBotao(BOTAO_CANCELAR))
    leaveMenuCriarUtilizador(&strIns);

  if (input.checkCliqueBotao(BOTAO_OK)) {
    if (saveNovoUtilizador(strIns)) {
      // Caso o utilizador tenha sido criado com sucesso sai do menu
      leaveMenuCriarUtilizador(&strIns);
    }
  }
}

// Acerto do Relógio
void Menus::menuConfigDataHoraInit() {
  bool a;

  data = {rtc.getYear(), rtc.getMonth(a), rtc.getDate()};
  hora = {rtc.getHour(a, a), rtc.getMinute()};
  doW = rtc.getDoW();

  campoAtual = CAMPO_DOW;
}
void leaveSaveConfigDataHora(DATE* data, HORA* hora, uint8_t doW) {
  if (verificaDataHora(data, hora)) {
    rtc.setYear(data->ano);
    rtc.setMonth(data->mes);
    rtc.setDate(data->dia);
    rtc.setHour(hora->hora);
    rtc.setMinute(hora->minuto);
    rtc.setDoW(doW);
    rtc.setSecond(0);

    menus.setMenu(MENU_OPCOES);
  } else {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(F("Data e Hora"));
    lcd.setCursor(0, 2);
    lcd.print(F("Impossiveis"));
    delay(DELAY_TIME);
    lcd.clear();
  }
}
void Menus::menuConfigDataHoraLoop() {
  bool aceso = PISCAR_CAMPO;

  lcd.setCursor(2, 1);
  printDoW(doW, campoAtual, aceso);
  lcd.print(F(" "));
  printData(data.ano, data.mes, data.dia, campoAtual, aceso);
  lcd.setCursor(6, 2);
  printHora(hora.hora, hora.minuto, campoAtual, aceso);

  switch (campoAtual) {
    case CAMPO_ANO:
      input.rotaryAlteraVar(&data.ano, 50, 18);
      break;
    case CAMPO_MES:
      input.rotaryAlteraVar(&data.mes, 12, 1);
      break;
    case CAMPO_DIA:
      input.rotaryAlteraVar(&data.dia, 31, 1);
      break;
    case CAMPO_HORA:
      input.rotaryAlteraVar(&hora.hora, 23, 0);
      break;
    case CAMPO_MINUTO:
      input.rotaryAlteraVar(&hora.minuto, 59, 0);
      break;
    case CAMPO_DOW:
      input.rotaryAlteraVar(&doW, 6, 0);
  }
  if (input.checkCliqueBotao(BOTAO_ROTARY)) {
    if (campoAtual != CAMPO_MINUTO)
      campoAtual++;
    else
      leaveSaveConfigDataHora(&data, &hora, doW);
  }

  if (input.checkCliqueBotao(BOTAO_BAIXO))
    campoAtual = campoAtual < CAMPO_MINUTO ? campoAtual + 1 : CAMPO_MINUTO;
  if (input.checkCliqueBotao(BOTAO_CIMA))
    campoAtual = campoAtual > 0 ? campoAtual - 1 : CAMPO_DOW;
  if (input.checkCliqueBotao(BOTAO_OK))
    leaveSaveConfigDataHora(&data, &hora, doW);
  if (input.checkCliqueBotao(BOTAO_CANCELAR))
    setMenu(MENU_OPCOES);
}

// Menu Reset
void Menus::menuResetConfirmarInit() {
  campoAtual = OPCOES;  // opcao "NAO" selecionada por predefinicao
  lcd.setCursor(0, 3);
  lcd.write(SETA_HORIZONTAL);
}

void Menus::menuResetConfirmarLoop() {
  lcd.setCursor(3, 0);
  lcd.print(F("Tem a certeza?"));
  lcd.setCursor(0, 1);
  lcd.print(F("(Apagar as config.)"));

  // Imprimir as opcoes
  lcd.setCursor(1, 3);
  lcd.print(F("Nao"));

  lcd.setCursor(9, 3);
  lcd.print(F("Sim"));

  if (input.checkCliqueBotao(BOTAO_CIMA) ||
      input.rotationEvent == ROTATION_LEFT) {
    campoAtual = MODO_RAPIDO;
    lcd.setCursor(0, 3);
    lcd.print(F(" "));  // Apagar a seta
    lcd.setCursor(8, 3);
    lcd.write(SETA_HORIZONTAL);
  }
  if (input.checkCliqueBotao(BOTAO_BAIXO) ||
      input.rotationEvent == ROTATION_RIGHT) {
    campoAtual = OPCOES;
    lcd.setCursor(8, 3);
    lcd.print(F(" "));  // Apagar a seta
    lcd.setCursor(0, 3);
    lcd.write(SETA_HORIZONTAL);
  }
  if (input.checkCliqueBotao(BOTAO_OK) ||
      input.checkCliqueBotao(BOTAO_ROTARY)) {
    if (campoAtual == MODO_RAPIDO) {
      // Caso seja selecionado a opcao SIM
      setMenu(MENU_INICIAL);
      resetEEPROM();
    } else {
      setMenu(MENU_OPCOES);
    }
  }
}