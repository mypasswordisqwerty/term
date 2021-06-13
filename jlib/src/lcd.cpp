
#include <jlib/lcd.hpp>

#include "lcd_hd44780_i2c.h"

LCD* LCD::init(I2C_HandleTypeDef* i2c){
	lcdInit(i2c, 0x3F, 2, 16);
	return this;
}

void LCD::print(const std::string& text){
	if (text1 == text){
		return;
	}
	text1 = text;
	std::string txt = text;
	while(txt.length() < 16) txt+=" ";
	lcdSetCursorPosition(0, 0);
	lcdPrintStr((uint8_t*)txt.c_str(), txt.length());
}

void LCD::print2(const std::string& text){
	if (text2 == text){
		return;
	}
	text2 = text;
	std::string txt = text;
	while(txt.length() < 16) txt+=" ";
	lcdSetCursorPosition(0, 1);
	lcdPrintStr((uint8_t*)txt.c_str(), txt.length());
}
