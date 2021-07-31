#ifndef BUTTON_H
#define BUTTON_H

#include <jlib/gpio.hpp>

class Button;

struct IButtonCallback {
	virtual void onButtonToggle(const Button&, bool) =0;
};

class Button: public GPIO {
public:
	Button* init(const Pin& pin, IButtonCallback* callback);
	virtual void loop(uint32_t delay) override;

private:
	IButtonCallback* callback;
	bool _state;
	bool _debounce;
};

#endif
