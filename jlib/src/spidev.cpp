#include <jlib/spidev.hpp>
#include <jlib/hal.h>


SPIDev* SPIDev::init(SPI_HandleTypeDef* spi, const Pin& csPin) {
	this->spi = spi;
	cs.init(csPin);
	if (cs.valid()) {
		cs.on();
	}
	return this;
}

void SPIDev::read(void* buf, uint16_t size) {
	if (!cs.valid()){
		return;
	}
	cs.off();
	HAL_SPI_Receive(spi, (uint8_t*)buf, size, 1000);
	cs.on();
}

void SPIDev::write(void* buf, uint16_t size) {
	if (!cs.valid()){
		return;
	}
	cs.off();
	HAL_SPI_Transmit(spi, (uint8_t*)buf, size, 1000);
	cs.on();
}
