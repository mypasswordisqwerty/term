#ifndef SPIDEV_H
#define SPIDEV_H

#include <jlib/types.h>
#include <jlib/object.hpp>
#include <jlib/gpio.hpp>


class SPIDev: public Object{
public:
	SPIDev* init(SPI_HandleTypeDef* spi, const Pin& csPin);
	HAL_StatusTypeDef read(void* buf, uint16_t size);
	HAL_StatusTypeDef write(void* buf, uint16_t size);

protected:
	SPI_HandleTypeDef* spi;
	GPIO cs;
};

#endif
