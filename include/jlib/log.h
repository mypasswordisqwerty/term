/*
 * log.h
 *
 *  Created on: May 29, 2020
 *      Author: john
 */

#ifndef INC_LOG_H_
#define INC_LOG_H_

void logx(char level, char* fmt, ...);
void logd(char* fmt, ...);
void logi(char* fmt, ...);
void logw(char* fmt, ...);
void loge(char* fmt, ...);


#endif /* INC_LOG_H_ */
