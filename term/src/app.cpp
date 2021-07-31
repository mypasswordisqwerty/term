#include "app.hpp"
#include <string>
#include <string.h>
#include <jlib/hal.h>
#include "utils.hpp"

App app;

const int App::LOOP_DELAY = 100;
#define READY "READY"
#define OK "ok"

void App::init(){
	lastcmd = READY;
	lcd.init(cfg.i2c);
	jobLed.init(cfg.ledJob);

	objects.push_back(pw1.init(cfg.spi, cfg.pw1));
	objects.push_back(pw2.init(cfg.spi, cfg.pw2));
	objects.push_back(t1.init(cfg.spi, cfg.csT1));
	objects.push_back(t2.init(cfg.spi, cfg.csT2));
	objects.push_back(top.init(pw1));
	objects.push_back(bottom.init(pw2));
	objects.push_back(jobButton.init(cfg.btnStart, this));
}

std::string App::status(){
	return pw1.value() + " " + t1.value()+ " " + t2.value() + " " + pw2.value();
}

void App::loop(){
	for (Object* obj: objects){
		obj->loop(LOOP_DELAY);
	}
	if (tmr.update(LOOP_DELAY)){
		lcd.print(lastcmd);
		lcd.print2(status());
	}
	HAL_Delay(LOOP_DELAY);
}

void App::onButtonToggle(const Button& btn, bool state) {
	stop();
}

void App::onPidDone(Pid& pid) {
	pid.clearCallback();
	runProgram();
}

void App::stop(){
	program.clear();
	inProgram = false;
	top.clearTarget();
	bottom.clearTarget();
	pw1.setPower(0);
	pw2.setPower(0);
	jobLed.off();
	lastcmd = READY;
}

void App::runProgram() {
	if (program.empty()) {
		jobLed.off();
		inProgram = false;
		return;
	}
	string cmd = program[0];
	program.erase(program.begin());
	cmd = processCommand(cmd);
	if (cmd!=OK){
		lastcmd = cmd;
	}
}

void App::startProgram() {
	if (inProgram) {
		return;
	}
	inProgram = true;
	jobLed.on();
}

int App::intVal(string& p) {
	if (p[0]=='p'){
		p[1]=='2' ? pw2.power100() : pw1.power100();
	}
	if (p[0]=='t'){
		return p[1]=='2' ? t2.temperature() : t1.temperature();
	}
	return std::stoi(p);
}

std::string App::processCommand(std::string command){
	std::string save = command;
	string cmd = Utils::shift(command);
	if (cmd=="kill") {
		stop();
	} else if (cmd=="status") {
		return status();
	}
	if (inProgram) {
		program.push_back(save);
		return OK;
	}
	if (cmd == "heat") {
		startProgram();
		string p = Utils::shift(command);
		Pid* pid = &bottom;
		MAX31855* src = &t1;
		if (p=="top") {
			pid = &top;
		}
		p = Utils::shift(command);
		int val = intVal(p);
		p = Utils::shift(command);
		if (p[1]=='2') {
			src = &t2;
		}
		pid->setTarget(src, val, this);
	} else if (cmd == "pw") {
		Power* pw = &pw1;
		string p = Utils::shift(command);
		if (p=="2"){
			pw = &pw2;
		}
		p = Utils::shift(command);
		int val = intVal(p);
		pw->setPower(val/100.f);
	} else if (cmd=="off") {
		stop();
	} else {
		return "Bad command " + cmd;
	}
	lastcmd = save;
	return OK;
}

void App::usbReceive(uint8_t* buf, uint32_t len){
	std::string pack((const char*)buf, len);
	pack = processCommand(pack) + "\n";
	cfg.usb_transmit((uint8_t*)pack.c_str(), (uint32_t)pack.length());
}


// App c interface
extern "C" Config* APP_Config(){
	Config* cfg = app.config();
	memset(cfg, 0, sizeof(Config));
	return cfg;
}

extern "C" void APP_Init(){
	app.init();
}

extern "C" void APP_Loop(){
	app.loop();
}

extern "C" void APP_USB_Receive(uint8_t* buf, uint32_t len){
	app.usbReceive(buf, len);
}

