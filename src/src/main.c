#include <Arduino.h>
#include <EEPROM.h>
#include "openVTxEEPROM.h"

#define SERIAL_PIN PD5
#define TRAMP_BAUD 9600
#define SMARTAUDIO_BAUD 4800

char rxPacket[16] = {0};
char txPacket[18] = {0}; // May need to be increase for SA2.1 with more than 4 power levels

uint8_t pitMode = 1;
bool vtxModeLocked = false;
uint16_t temperature = 0; // Dummy value.

#include "targets.h"
#include "rtc6705.h"
#include "common.h"
#include "tramp.h"
#include "smartAudio.h"

uint16_t dCycle;

void setup()
{
  rfPowerAmpPinSetup();
  setPowerdB(0);

  readEEPROM();
  pitMode = (myEEPROM.pitmodeInRange || myEEPROM.pitmodeOutRange) ? 1 : 0;

  spiPinSetup();  
  rtc6705ResetState(); // During testing registers got messed up. So now it gets reset on boot!
  rtc6705WriteFrequency(myEEPROM.currFreq);

  Serial_begin(myEEPROM.vtxMode == TRAMP ? TRAMP_BAUD : SMARTAUDIO_BAUD);

  while (!Serial)
  {
    ;
  }
  UART1_HalfDuplexCmd(ENABLE);
  pinMode(SERIAL_PIN, INPUT_PULLUP);

  // clear any uart garbage
  clearSerialBuffer();
  

// pinMode(POWER_AMP_6, OUTPUT);
// uint16_t AutoReloadRegister = 255;//1591; // 10khz
// TIM1->PSCRL = 0x00; // 62kHz
// TIM1->ARRH = (uint8_t)(AutoReloadRegister >> 8);
// TIM1->ARRL = (uint8_t)(AutoReloadRegister);
// unsigned char tmp = TIM1->CCER2 & (uint8_t)(~(TIM1_CCER2_CC4E | TIM1_CCER2_CC4P));
// TIM1->CCER2 = tmp | TIM1_CCER2_CC4E;
// TIM1->CCMR4 = TIM1_OCMODE_PWM1 | TIM1_CCMR_OCxPE;
// TIM1->CCR4H = 0x00; // init with 0 duty cycle e.g. 0 volts
// TIM1->CCR4L = 0x00; // init with 0 duty cycle e.g. 0 volts


// dCycle = 1590; // 20dBm 100mW
// dCycle = 1321; // 17dBm 50mW
// dCycle = 1150; // 14dBm 25mW
// dCycle = 1024; // 2.9V
// dCycle = 1; // 0V
// dCycle = AutoReloadRegister / 2;
// dCycle = 155; // 1.463V 
// dCycle = 160; // 1.510V 27mW
// dCycle = 170; // 1.600V 33mW
// dCycle = 175; // 1.643V 34mW
// dCycle = 180; // 1.690V 35mW
// dCycle = 200; // 1.867V 36mW
// dCycle = 210; // 1.962V
// dCycle = 220; // 2.047V
// dCycle = 230; // 2.137V
// dCycle = 255; // 2.344V 44mW

// TIM1->CCR4H = (uint8_t)(dCycle >> 8);
// TIM1->CCR4L = (uint8_t)(dCycle);
}

void loop()
{
  if (myEEPROM.vtxMode == TRAMP)
  {
    trampProcessSerial();
  }
  else
  {
    smartaudioProcessSerial();
  }

  writeEEPROM();
}