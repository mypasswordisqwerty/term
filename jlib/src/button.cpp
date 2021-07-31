#include <jlib/button.hpp>

Button* Button::init(const Pin& pin, IButtonCallback* callback){
	GPIO::init(pin);
	_state = false;
	_debounce = false;
	this->callback = callback;
	return this;
}

void Button::loop(uint32_t delay) {
	if (!valid()){
		return;
	}
	if (state() == _state) {
		if (_debounce){
			_debounce = false;
		}
		return;
	}
	if (!_debounce){
		_debounce = true;
		return;
	}
	_state = state();
	_debounce = false;
	if (callback) {
		callback->onButtonToggle(*this, _state);
	}
}
