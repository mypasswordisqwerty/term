#include <jlib/gpio.hpp>


GPIO* GPIO::init(const Pin& pin){
	this->pin = pin;
	return this;
}

void GPIO::set(bool on){
	HAL_GPIO_WritePin((GPIO_TypeDef*)pin.port, pin.pin, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void GPIO::toggle(){
	HAL_GPIO_TogglePin((GPIO_TypeDef*)pin.port, pin.pin);
}

bool GPIO::state(){
	return HAL_GPIO_ReadPin((GPIO_TypeDef*)pin.port, pin.pin) == GPIO_PIN_SET;
}
