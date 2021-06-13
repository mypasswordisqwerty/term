/*
 * log.c
 *
 *  Created on: May 29, 2020
 *      Author: john
 */
#include <stdio.h>
#include <stdarg.h>

#include <jlib/hal.h>

static char lbuf[1024]={0};

static void _log(char level, char* fmt, va_list lst){
	vsnprintf(lbuf, 1023, fmt, lst);
	ITM_SendChar(level);
	ITM_SendChar(':');
	ITM_SendChar(' ');
	char *b = lbuf;
	while(*b){
		ITM_SendChar(*b);
		b++;
	}
	ITM_SendChar('\n');
}

void logx(char level, char* fmt, ...){
	va_list va;
	va_start(va, fmt);
	_log(level, fmt, va);
	va_end(va);
}

void logd(char* fmt, ...){
	va_list va;
	va_start(va, fmt);
	_log('D', fmt, va);
	va_end(va);
}
void logi(char* fmt, ...){
	va_list va;
	va_start(va, fmt);
	_log('I', fmt, va);
	va_end(va);
}
void logw(char* fmt, ...){
	va_list va;
	va_start(va, fmt);
	_log('W', fmt, va);
	va_end(va);
}

void loge(char* fmt, ...){
	va_list va;
	va_start(va, fmt);
	_log('E', fmt, va);
	va_end(va);
}
