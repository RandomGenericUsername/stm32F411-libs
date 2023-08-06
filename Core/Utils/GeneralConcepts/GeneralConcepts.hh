#ifndef __GENERALCONCEPTS_H__
#define __GENERALCONCEPTS_H__

#include <type_traits>
#include <system.h>
/**
 * @brief Concept to assert if a template parameter is an enum in the Container class
 */
template <typename T>
concept EnumType = std::is_enum_v<T>;

/**
 * @brief  Concept to check whether the peripheral being istantiated corresponds to one
 * of the STM32 defined structs for their peripherals 
 * @tparam T 
 */
template<typename T>
concept STM32Peripheral =  
(
    std::is_same_v<T, GPIO_TypeDef> ||
    std::is_same_v<T, USART_TypeDef> ||
    std::is_same_v<T, RCC_TypeDef>
);


#endif // __GENERALCONCEPTS_H__