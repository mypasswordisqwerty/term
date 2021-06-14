#ifndef AD8400_H
#define AD8400_H

#include <jlib/spidev.hpp>

class AD8400: public SPIDev{
public:
	AD8400(){}

	void setPower(float value) {
		if (!cs.valid()){
			return;
		}
		uint16_t power = (uint16_t)(value * 0xFF);
		write(&power, 2);
	}

};


#endif
