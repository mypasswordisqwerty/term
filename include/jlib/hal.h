#ifndef HAL_H
#define HAL_H

#if defined(STM32F304xB)
#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_spi.h"
#elif defined(STM32F103xB)
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"
#else
#error "Unknown HAL version"
#endif

#endif
