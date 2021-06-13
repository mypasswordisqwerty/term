#ifndef GPIO_H
#define GPIO_H

#include <jlib/types.h>
#include <jlib/object.hpp>

class GPIO: public Object {
public:
	GPIO* init(const Pin& pin);
	void set(bool on);
	inline void on(){ set(true); }
	inline void off(){ set(false); }
	inline bool valid() {return pin.port!=0;}
	void toggle();
	bool state();

private:
	Pin pin;
};

#endif
