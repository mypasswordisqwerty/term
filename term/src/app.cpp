#include "app.hpp"
#include <string>
#include <string.h>
#include <jlib/hal.h>
#include "utils.hpp"

App app;

const int App::LOOP_DELAY = 100;

void App::init(){
	lastcmd = "READY";
	int i=0;
	lcd.init(cfg.i2c);

	objects[i++] = pw1.init(cfg.spi, cfg.pw1);
	objects[i++] = pw2.init(cfg.spi, cfg.pw2);
	objects[i++] = t1.init(cfg.spi, cfg.csT1);
	objects[i++] = t2.init(cfg.spi, cfg.csT2);
}

bool on = false;

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

std::string App::processCommand(std::string command){
	lastcmd = command;
	string cmd = Utils::shift(command);
	if (cmd == "pw") {
		Power& pw = pw1;
		string p = Utils::shift(command);
		if (p=="2"){
			pw = pw2;
		}else if (p!="1"){
			return "Bad power "+p;
		}
		p = Utils::shift(command);
		int val = std::stoi(p);
		pw.setPower(val/100.f);
	} else if (cmd=="status") {
		return status();
	} else if (cmd=="off") {
		pw1.setPower(0);
		pw2.setPower(0);
	} else {
		return "Bad command " + cmd;
	}
	return "ok";
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

