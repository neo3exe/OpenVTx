#ifndef __TARGET_DEF_H_
#define __TARGET_DEF_H_

#define MAX_POWER     400 // mW

#define UART_RX       PD5
#define UART_TX       PD5

#define SPI_MOSI      PC7
#define SPI_SS        PC6
#define SPI_CLOCK     PC5

// #define LED1          PA4 // Red (power)
#define LED2          PA3 // Green (connected)
#define LED3          PB5 // Blue (SA message)
#define VREF          PB5
#define VPD           PC4

#define RTC_BIAS      PD2

// /******* Target specific declarations *******/
// #include <gd32f1x0.h>
// #include "gpio.h"
// #include "serial.h"

// uint32_t millis(void);
// void delay(uint32_t ms);
// void delayMicroseconds(uint32_t us);
void checkPowerOutput(void);

#endif /* __TARGET_DEF_H_ */