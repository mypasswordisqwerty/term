#ifndef SPIDEV_H
#define SPIDEV_H

#include <jlib/types.h>
#include <jlib/object.hpp>
#include <jlib/gpio.hpp>


class SPIDev: public Object{
public:
	SPIDev* init(SPI_HandleTypeDef* spi, const Pin& csPin);
	void read(void* buf, uint16_t size);
	void write(void* buf, uint16_t size);

protected:
	SPI_HandleTypeDef* spi;
	GPIO cs;
};

#endif
