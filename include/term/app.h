#ifndef TERM_APP_H_
#define TERM_APP_H_

#include "config.h"

Config* APP_Config();
void APP_Init();
void APP_Loop();
void APP_USB_Receive(uint8_t* buf, uint32_t len);

#endif
