#pragma once

#include <Arduino.h>
// Definición de los pines de la placa.

// Módulo temporizador
#define TPL5010_DONE PIN_PC0	// Temporizador: Despierta
#define TPL5010_WAKE PIN_PC2	// Temporizador: Hecho

// Módulo radio.
#define RFM95_DIO0 PIN_PC1
#define RFM95_DIO1 PIN_PC3
#define RFM95_RST PIN_PD6
#define RFM95_SS PIN_PA7
#define RFM95_SCK PIN_PA6
#define RFM95_MISO PIN_PA5
#define RFM95_MOSI PIN_PA4

#define LED_BUILTIN DD0

// Sensor temperatura TMP36GZ
#define TERMOMETRO_POWER PIN_A2
#define TERMOMETRO_DATOS PIN_A1

