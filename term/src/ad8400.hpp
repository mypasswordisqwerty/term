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
		int power = value * 255;
		write(&power, 2);
	}

};


#endif
