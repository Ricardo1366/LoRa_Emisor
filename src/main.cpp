#define DEBUG

#include <Arduino.h>
#include <pines.h>
#include <LoRa.h>
#include <RocketScream_LowPowerAVRZero.h>

#define FRECUENCIA 868E6

void Despierta();

int temperatura = 0;

void setup()
{
  delay(10000);
  // Configuración inicial.
#if defined(DEBUG)
  Serial2.begin(115200);
#endif
  // Termómetro.
  pinMode(TERMOMETRO_DATOS, INPUT);
  pinMode(TERMOMETRO_POWER, OUTPUT);
  digitalWrite(TERMOMETRO_POWER, 0);

  pinMode(TEMP_DONEPIN, OUTPUT);
  digitalWrite(TEMP_DONEPIN, LOW);

  // LoRa.setPins(RFM95_SS, RFM95_RST, RFM95_DIO0);
  LoRa.setPins(LORA_SSPIN, LORA_RESETPIN, LORA_DIO0PIN);

#if defined(DEBUG)
  Serial2.println(F("Configuramos modo sueño."));
#endif

  // Configuramos el modo "sueño"
  // set_sleep_mode(SLEEP_MODE_STANDBY);
  // sleep_enable();

  // Módulo lora
  if (!LoRa.begin(FRECUENCIA))
  {
#if defined(DEBUG)
    Serial2.println(F("Fallo al inicalizar el módulo de rádio RFM_95."));
#endif
  }
  else
  {
    LoRa.beginPacket();
    LoRa.print(F("Modulos LoRa inicializado."));
    LoRa.endPacket();
  }

  // Activamos la interrupción que tiene que despertarmos.
  attachInterrupt(digitalPinToInterrupt(TPL5010_WAKE), Despierta, FALLING);
  // attachInterrupt(TEMP_WAKEPIN, Despierta, RISING);
}

void loop()
{
  // Tenemos que informar al temporizador que estamos despiertos o nos reseteará.
  digitalWrite(TPL5010_DONE, HIGH);
#if defined(DEBUG)
  Serial2.println(F("Estamos depiertos."));
  LoRa.beginPacket();
  LoRa.print(F("Estamos depiertos."));
  LoRa.endPacket();
#endif
  digitalWrite(TPL5010_DONE, LOW);

  // Emcendemos el termómetro
  digitalWrite(TERMOMETRO_POWER, HIGH);
  LoRa.begin(FRECUENCIA);
  // Encendemos el módulo de radio.
  if (!LoRa.begin(FRECUENCIA))
  {
    Serial2.print(F("¡ FALLO ! ---> "));
  }
#if defined(DEBUG)
  Serial2.println(F("LoRa activado."));
  LoRa.beginPacket();
  LoRa.print(F("LoRa activado."));
  LoRa.endPacket();
#endif

  // Leemos la temperatura.
  temperatura = analogRead(TERMOMETRO_DATOS);

  // Apagamos el termómetro
  digitalWrite(TERMOMETRO_POWER, LOW);

  // Enviamos el dato.
  LoRa.beginPacket();
  LoRa.print(temperatura);
  LoRa.endPacket();
#if defined(DEBUG)
  Serial2.println(F("Información enviada. ¡ A dormir !"));
  LoRa.beginPacket();
  LoRa.print(F("Información enviada. ¡ A dormir !"));
  LoRa.endPacket();
#endif
  delay(5);
  // y ponemos a dormir el módulo de rádio.
  LoRa.sleep();

  // Ponemos a dormir el micro. Al despertar continuaremos en la línea siguiente.
  LowPower.powerDown();
#if defined(DEBUG)
  Serial2.println(F("Despiertos. ¡ Al curro de nuevo !"));
#endif
}

// Función ejecutada al recibir la interrupción.
void Despierta()
{
}