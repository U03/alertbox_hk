
//
// Selectionner ESP32-WROVER-MODULE
//


// La carte est basée sur TinyGSM
//
#define CARD_TINYGSM

#define GPIO_LED          2
#define GPIO_BUTTON      14

#define LEDS_DATA_PIN    13


#define PCF_SCL 22
#define PCF_SDA 21
#define PCF_00  0x20
#define PCF_01  0x21
#define PCF_02  0x22
#define PCF_03  0x23

// On doit definir le type de modem avant d'inclure TinyGsmClient
//
#define TINY_GSM_MODEM_SIM7000
#include <TinyGsmClient.h>

// Define the serial console for debug prints, if needed
//
#define TINY_GSM_DEBUG SerialMon



// Set serial for AT commands
//
#define SerialAT Serial1

// Define the serial console for debug prints, if needed
//
//#define DUMP_AT_COMMANDS

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif


// TTGO T-SIM7000G pins
//
#define MODEM_RST       5
#define MODEM_PWKEY     4
#define MODEM_POWER_ON  4
#define MODEM_DTR       25
#define MODEM_TX        27
#define MODEM_RX        26

#define I2C_SCL              22
#define GSM_PIN              ""
