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

void SPIDev::_swap(void* buf, uint16_t size){
	uint8_t* s = (uint8_t*)buf;
	uint8_t *e = s + size - 1;
	for (int i=0; i<size/2; i++){
		uint8_t tmp = *s;
		*s++ = *e;
		*e-- = tmp;
	}
}

HAL_StatusTypeDef SPIDev::read(void* buf, uint16_t size, bool swap) {
	if (!cs.valid()){
		return HAL_ERROR;
	}
	cs.off();
	HAL_StatusTypeDef status = HAL_SPI_Receive(spi, (uint8_t*)buf, size, 500);
	cs.on();
	if (swap) _swap(buf, size);
	return status;
}

HAL_StatusTypeDef SPIDev::write(void* buf, uint16_t size, bool swap) {
	if (!cs.valid()){
		return HAL_ERROR;
	}
	if (swap) _swap(buf, size);
	cs.off();
	HAL_StatusTypeDef status = HAL_SPI_Transmit(spi, (uint8_t*)buf, size, 500);
	cs.on();
	return status;
}
