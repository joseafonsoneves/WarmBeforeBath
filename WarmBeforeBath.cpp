/*
                                WarmBeforeBath
    José Neves      89683
    Miguel Fazenda  90146
    BitChallenge 2018
*/

#include <dht.h>
#include <extEEPROM.h>
#include "estado.h"
#include "input.h"
#include "menus.h"

#define DHT_PIN 8
#define RELAY_PIN 7

#define BACKLIGHT_TIMEOUT_MILLIS 30000
#define MENU_TIMEOUT_MILLIS 120000

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
DS3231 rtc;
extEEPROM memEEPROM(kbits_256, 2, 64);
dht DHT;

Menus menus;
Estado estado;
Input input;

int temp;
unsigned long lastUpdateTempMillis;
bool lcdState;

void rotaryEncoderInterrupt() {
  input.rotaryEncoderInterrupt();
}

void setup() {
  lcd.begin(LCD_COMP, LCD_ALT);

  // Inicialização dos botões e rotary encoder
  input.init();
  attachInterrupt(digitalPinToInterrupt(2), rotaryEncoderInterrupt, CHANGE);

  // Inicialização do relay
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);

  menus.init();

  lastUpdateTempMillis = 0;
  lcdState = true;  // A backlight deve estar ligada ao inicio
}

void loop() {
  unsigned long millisTime = millis();
  bool backlightShouldBeOn;

  input.update(&lcd);

  // De dois em dois segundos
  if (millisTime - lastUpdateTempMillis > 2000) {
    // Lê a temperatura e a humidade no sensor
    DHT.read22(DHT_PIN);

    // Atualiza o estado do aquecedor
    estado.loop(float(DHT.temperature), &rtc);

    // Liga ou desliga o relé
    digitalWrite(RELAY_PIN, !estado.isAquecedorLigado);

    // Atualiza lastUpdateMillis
    lastUpdateTempMillis = millisTime;
  }

  // Atualizar os menus
  menus.loop(&DHT);

  // Verifica se passou o tempo necessário para desligar a backlight desde a
  // ultima interação do utilizador
  backlightShouldBeOn =
      millisTime < BACKLIGHT_TIMEOUT_MILLIS + input.lastInputChangeMillis;

  // Liga ou desliga a backlight se for necessário
  if (lcdState != backlightShouldBeOn) {
    lcdState = backlightShouldBeOn;
    lcd.setBacklight(lcdState);
  }

  // Passado algum tempo sem interação
  if (menus.menuId != MENU_INICIAL &&
      (millisTime > (input.lastInputChangeMillis + MENU_TIMEOUT_MILLIS)))
    menus.setMenu(MENU_INICIAL);  // Volta ao menu inicial
}