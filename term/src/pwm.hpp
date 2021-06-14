#ifndef PWM_HPP
#define PWM_HPP

#include <jlib/gpio.hpp>

#define PERIOD 30000

class PWM: public GPIO{
public:
	PWM():power(0),pos(0),isOn(false) {}

	void setPower(float value) {
		power = value;
		width = PERIOD - (PERIOD * value);
	}

	virtual void loop(uint32_t delay) {
		if (!valid()){
			return;
		}
		pos -= delay;
		if (pos <= 0){
			pos = PERIOD;
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
