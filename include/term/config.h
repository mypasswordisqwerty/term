#ifndef TERM_CONFIG_H_
#define TERM_CONFIG_H_

#include <sys/types.h>
#include <jlib/hal.h>
#include <jlib/types.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef uint8_t (*USB_Transmit)(uint8_t* Buf, uint16_t Len);

typedef struct _Power {
	Pin csPin;
	Pin onPin;
	Pin pwmPin;
	Pin led;
} PowerConfig;

typedef struct _Config {
	USB_Transmit usb_transmit;
	I2C_HandleTypeDef *i2c;
	SPI_HandleTypeDef *spi;
	PowerConfig pw1;
	PowerConfig pw2;
	Pin csT1;
	Pin csT2;
	Pin btnUp;
	Pin btnDown;
	Pin btnStart;
	Pin ledJob;
} Config;



#ifdef __cplusplus
}
#endif

#endif
