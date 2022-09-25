#define DEBUG_OFF

#include <Arduino.h>
#include <pines.h>
#include <LoRa.h>
#include <RocketScream_LowPowerAVRZero.h>

#define FRECUENCIA 868E6

void Despierta();

int temperatura = 0;

void setup()
{
  // Configuración inicial.
#if defined(DEBUG)
  Serial2.begin(115200);
#endif
  // Termómetro.
  pinMode(TERMOMETRO_DATOS, INPUT);
  pinMode(TERMOMETRO_POWER, OUTPUT);
  digitalWrite(TERMOMETRO_POWER, 0);

  pinMode(TPL5010_DONE, OUTPUT);

  LoRa.setPins(RFM95_SS, RFM95_RST, RFM95_DIO0);

  // Módulo lora
  if (!LoRa.begin(FRECUENCIA))
  {
#if defined(DEBUG)
    Serial2.println(F("Fallo al inicalizar el módulo de rádio RFM_95."));
#endif
  }

  // Activamos la interrupción que tiene que despertarmos.
  attachInterrupt(digitalPinToInterrupt(TPL5010_WAKE), Despierta, FALLING);
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

  // Emcendemos el termómetro
  digitalWrite(TERMOMETRO_POWER, HIGH);

  // Finalizamos el aviso al temporizador.
  digitalWrite(TPL5010_DONE, LOW);

  // Encendemos el módulo de radio.
  if (!LoRa.begin(FRECUENCIA))
  {
#if defined(DEBUG)
    Serial2.print(F("¡ FALLO ! ---> "));
#endif
  }
  else
  {
#if defined(DEBUG)
    Serial2.println(F("LoRa activado."));
    LoRa.beginPacket();
    LoRa.print(F("LoRa activado."));
    LoRa.endPacket();
#endif
  }
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
  // Ponemos un retraso de 5 milisegundos para que de tiempo de escribir en el monitor serial.
  delay(5);
#endif
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