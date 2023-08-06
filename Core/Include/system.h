#ifndef __SYSTEM_H__
#define __SYSTEM_H__


#include <stm32f411xe.h>

enum class AvailablePeripherals 
{ 
    _DMA2, _DMA1, _CRC, _GPIOH, _GPIOE, _GPIOC, _GPIOB, _GPIOA, _OTGFS, _PWR, _I2C3, _I2C2, _I2C1,
    _USART2, _SPI3, _SPI2, _WWDG, _TIM5, _TIM4, _TIM3, _TIM2, _SPI5, _TIM11, _TIM10, _TIM9,
    _SYSCGF, _SPI4, _SPI1, _SDIO, _ADC1, _USART6, _USART1, _TIM1
};


#endif // __SYSTEM_H__