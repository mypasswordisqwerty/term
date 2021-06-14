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

HAL_StatusTypeDef SPIDev::read(void* buf, uint16_t size) {
	if (!cs.valid()){
		return HAL_ERROR;
	}
	cs.off();
	HAL_StatusTypeDef status = HAL_SPI_Receive(spi, (uint8_t*)buf, size, 500);
	cs.on();
	return status;
}

HAL_StatusTypeDef SPIDev::write(void* buf, uint16_t size) {
	if (!cs.valid()){
		return HAL_ERROR;
	}
	cs.off();
	HAL_StatusTypeDef status = HAL_SPI_Transmit(spi, (uint8_t*)buf, size, 1000);
	cs.on();
	return status;
}
