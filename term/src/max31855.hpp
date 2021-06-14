#ifndef MAX31855_H
#define MAX31855_H

#include <jlib/spidev.hpp>

#define REQ_TIME 500

class MAX31855: public SPIDev{
public:
	MAX31855():_value(0), err(9), reqtime(0){}

	std::string value() {
		if (err!=0){
			return "ER" + std::to_string(err);
		}

		std::string s = std::to_string(_value);
		while(s.length()<3) s = '0' + s;
		return s;
	}

	int esign(uint32_t val, int bits) {
		int b = bits-1;
		int32_t msk = (1 << b)-1;
		int32_t sign = (val >> b) ? 0 : -1;
		return (int32_t)((sign << b) | (val & msk));
	}

	int convert(int16_t val) {
		err = val & 1;
		val >>= 2;
		//float ref = esign(val & 0xFFF, 12) * 0.0625;
		//val >>= 14;
		float t = val * 0.25;
		return (int)t;
	}



	virtual void loop(uint32_t delay) override {
		reqtime -= delay;
		if (!cs.valid() || reqtime > 0){
			return;
		}
		reqtime = REQ_TIME;
		int16_t val = 0;
		if (read(&val, 2)!=HAL_OK){
			err = 9;
			return;
		}
		_value = convert(val);
	}

private:
	int _value;
	int err;
	int reqtime;

};


#endif
