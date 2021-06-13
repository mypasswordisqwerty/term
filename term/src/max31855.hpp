#ifndef MAX31855_H
#define MAX31855_H

#include <jlib/spidev.hpp>

class MAX31855: public SPIDev{
public:
	MAX31855():_value(0), err(0){}

	std::string value() {
		if (err!=0){
			return 'ER' + std::to_string(err);
		}

		std::string s = std::to_string(_value);
		while(s.length()<3) s = '0' + s;
		return s;
	}

	int esign(uint32_t val, int bits) {
		int b = bits-1;
		int32_t msk = (1 << b)-1;
		int32_t sign = (val >> b) ? 0 : -1;
		return reinterpret_cast<int32_t>((sign << b) | (val & msk));
	}

	int convert(uint32_t val) {
		err = val & 7;
		val >>= 4;
		float ref = esign(val & 0xFFF, 12) * 0.0625;
		val >>= 14;
		float t = esign(val, 14) * 0.25;
		return (int)t;
	}



	virtual void loop(uint32_t delay) override {
		if (!cs.valid()){
			return;
		}
		uint32_t val = 0;
		read(&val, 4);
		_value = convert(val);
	}

private:
	int _value;
	int err;

};


#endif
