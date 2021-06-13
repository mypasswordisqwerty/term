#ifndef TERM_APP_HPP_
#define TERM_APP_HPP_

#include <term/config.h>
#include <jlib/lcd.hpp>
#include <jlib/gpio.hpp>
#include <jlib/timer.hpp>
#include "power.hpp"
#include "max31855.hpp"

class App{
public:
	App():tmr(500){}
	Config* config(){return &cfg;}
	void init();
	void loop();
	void usbReceive(uint8_t* buf, uint32_t len);

private:
	std::string status();
	std::string processCommand(std::string command);

private:
	Config cfg;
	LCD lcd;
	Power pw1;
	Power pw2;
	MAX31855 t1;
	MAX31855 t2;
	Object* objects[5];
	Timer tmr;


private:
	static const int LOOP_DELAY;
	std::string lastcmd;
};

#endif
