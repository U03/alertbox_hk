

// GPIO 22: SCL
// GPIO 21: SDA
//


#include "PCF8574.h"

// 
//
PCF8574 PCF[] = {
  PCF8574(0x20),
  PCF8574(0x21),
  PCF8574(0x22),
  PCF8574(0x23)
};

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("PCF8574_LIB_VERSION:\t");
  Serial.println(PCF8574_LIB_VERSION);

  for (int i = 0; i <=3; i++) {
    Serial.print("PCF #");
    Serial.print(i);
    
    PCF[i].begin(21, 22, 0xFF);
    if (PCF[i].isConnected()) {
      Serial.println(" => présent");
    } else {
      Serial.println(" => absent");
    }
  }
}

uint32_t lireClavier() {

  uint32_t clavier = 0x0000;

  for (int i = 3; i >= 0; i--) {
    clavier = clavier << 8;
    if (PCF[i].isConnected()) {
      clavier = clavier | PCF[i].read8();
    } else {
      clavier = clavier | 0xFF;
    }
  }
  clavier = ~clavier;
  return(clavier);
}

void loop()
{
  uint32_t clavier;

  clavier = lireClavier();

  for (int i = 0; i <= 31; i++) {
    if (clavier & 0x0001) { 
      Serial.print('1');
    } else {
      Serial.print('0');
    }
    if (i % 8 == 7) {
      Serial.print(' ');
    }
    clavier = clavier >> 1;
  }
  Serial.println();
  delay(100);
}
