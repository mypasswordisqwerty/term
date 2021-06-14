#ifndef SPIDEV_H
#define SPIDEV_H

#include <jlib/types.h>
#include <jlib/object.hpp>
#include <jlib/gpio.hpp>


class SPIDev: public Object{
public:
	SPIDev* init(SPI_HandleTypeDef* spi, const Pin& csPin);
	HAL_StatusTypeDef read(void* buf, uint16_t size, bool swap = true);
	HAL_StatusTypeDef write(void* buf, uint16_t size, bool swap = true);
	inline HAL_StatusTypeDef sread(void* buf, uint16_t size) {return read(buf, size, false);}
	inline HAL_StatusTypeDef swrite(void* buf, uint16_t size) {return write(buf, size, false);}

protected:
	void _swap(void* buf, uint16_t size);
	SPI_HandleTypeDef* spi;
	GPIO cs;
};

#endif
