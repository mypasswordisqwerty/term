#ifndef POWER_HPP
#define POWER_HPP

#include <jlib/object.hpp>
#include <term/config.h>
#include "ad8400.hpp"
#include "pwm.hpp"

#define MAX_POWER 0.9f

class Power: public Object {
public:
	Power():_power(0){};
	~Power(){};

	Power* init(SPI_HandleTypeDef* spi, const PowerConfig& cfg) {
		adOn.init(cfg.onPin);
		ad8400.init(spi, cfg.csPin);
		pwm.init(cfg.pwmPin);
		return this;
	}

	inline std::string value() {
		std::string s = std::to_string((int)(_power*100));
		while(s.length()<3) s = '0' + s;
		return s;
	}

	void setPower(float power) {
		power = std::min(MAX_POWER, std::max(0.f, power));
		_power = power;
		if (adOn.valid()){
			adOn.set(power!=0);
			ad8400.setPower(power);
		}
		pwm.setPower(power);
	}

	virtual void loop(uint32_t delay) override {
		pwm.loop(delay);
		ad8400.loop(delay);
	}

private:
	PWM pwm;
	GPIO adOn;
	AD8400 ad8400;
	float _power;
};


#endif
