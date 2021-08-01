#ifndef TERM_APP_HPP_
#define TERM_APP_HPP_

#include <term/config.h>
#include <jlib/lcd.hpp>
#include <jlib/gpio.hpp>
#include <jlib/button.hpp>
#include <jlib/timer.hpp>
#include "power.hpp"
#include "max31855.hpp"
#include "pid.hpp"
#include <vector>

class App: public IPidCallback, IButtonCallback {
public:
	App():tmr(500),inProgram(false){}
	Config* config(){return &cfg;}
	void init();
	void loop();
	void usbReceive(uint8_t* buf, uint32_t len);
	virtual void onButtonToggle(const Button& btn, bool state) override;
	virtual void onPidDone(Pid& pid) override;

private:
	std::string status();
	std::string processCommand(std::string command);
	void stop();
	void startProgram();
	void runProgram();
	int intVal(std::string& p);

private:
	Config cfg;
	LCD lcd;
	Power pw1;
	Power pw2;
	MAX31855 t1;
	MAX31855 t2;
	Timer tmr;
	Pid top;
	Pid bottom;
	GPIO jobLed;
	Button jobButton;


private:
	static const int LOOP_DELAY;
	std::string lastcmd;
	std::vector<std::string> program;
	std::vector<Object*> objects;
	bool inProgram;
};

#endif
