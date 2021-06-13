#ifndef PWM_HPP
#define PWM_HPP

#include <jlib/gpio.hpp>

class PWM: public GPIO{
public:
	PWM():power(0),period(30000),pos(0),isOn(false) {}

	void setPower(float value) {
		power = value;
		width = period - (period * value);
	}

	virtual void loop(uint32_t delay) {
		if (!valid()){
			return;
		}
		pos -= delay;
		if (pos <= 0){
			pos = period;
		}
		bool val = pos > width;
		if (val != isOn){
			set(val);
			isOn = val;
		}
	}

private:
	int power;
	int period;
	int width;
	int pos;
	bool isOn;
};


#endif
