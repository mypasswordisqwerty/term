#ifndef PWM_HPP
#define PWM_HPP

#include <jlib/gpio.hpp>

#define PWM_PERIOD 30000

class PWM: public GPIO{
public:
	PWM():power(0),pos(0),isOn(false) {}

	PWM* init(const Pin& pin){
		GPIO::init(pin);
		setPower(.0);
		set(false);
		return this;
	}

	void setPower(float value) {
		power = value;
		width = PWM_PERIOD - (PWM_PERIOD * value);
	}

	virtual void loop(uint32_t delay) {
		if (!valid()){
			return;
		}
		pos -= delay;
		if (pos <= 0){
			pos = PWM_PERIOD;
		}
		bool val = pos > width;
		if (val != isOn){
			set(val);
			isOn = val;
		}
	}

private:
	int power;
	int width;
	int pos;
	bool isOn;
};


#endif
