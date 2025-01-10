#ifndef __RM1002_H__
#define __RM1002_H__
#include "stdint.h"
//#include "wm_hal.h"
#include "dev_config.h"
#include <stdbool.h>
#include "main.h"
#include "py32f0xx_hal.h"
#include "py32f002b_hal_i2c.h"


uint8_t rm1002_func(void *priv);
bool rm1002_wakeup(void);
bool rm1002_enterlowpower(void);

float* rm1002_debug_func(uint8_t rm1002_addr, float *cxxx0, float *cxxx1, float *cxxx2, float *cxxx3);

void rm1002_init(uint8_t rm1002_addr);

void rm1002_irq_handler(uint8_t rm1002_addr);
void rm1002_timer_handler(uint32_t ms);


typedef struct  {
	I2C_HandleTypeDef hi2c;
	uint8_t i2c_addr;
} i2c_handle;


void gpio_init(void);
void timer_init(void);

void pwm_init(void);

bool i2c_rx(uint8_t reg_start_addr, uint8_t *rx_buffer, uint8_t rx_len);
bool i2c_tx(uint8_t reg_start_addr, uint8_t *tx_buffer, uint8_t tx_len);

bool i2c1_rx(uint8_t reg_start_addr, uint8_t *rx_buffer, uint8_t rx_len);
bool i2c1_tx(uint8_t reg_start_addr, uint8_t *tx_buffer, uint8_t tx_len);

void uart1_init(void);
void uart1_send(uint8_t* tx_buffer, int tx_len);



#endif
