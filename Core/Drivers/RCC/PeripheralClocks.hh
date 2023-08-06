#ifndef __PERIPHERALCLOCKS_H__
#define __PERIPHERALCLOCKS_H__

#ifdef CCC

#include <stdint.h>
#include <Container.hh>
#include <system.h>
#include <GeneralConcepts.hh>

enum class PeripheralBridges : uint8_t { AHB1, AHB2, APB1, APB2 };

enum class AHB1BridgePeripherals : uint8_t {_DMA2 = 22, _DMA1 = 21, _CRC = 12 , _GPIOH = 7 , _GPIOE = 4, _GPIOD = 3, _GPIOC = 2, _GPIOB = 1, _GPIOA = 0};
enum class AHB2BridgePeripherals : uint8_t { _OTGFS = 7};
enum class APB1BridgePeripherals : uint8_t { _PWR = 28, _I2C3 = 23, _I2C2 = 22, _I2C1 = 21, _USART2 = 17, _SPI3 = 15, _SPI2 = 14, _WWDG = 11, _TIM5 = 3, _TIM4 = 2, _TIM3 = 1, _TIM2 = 0 };
enum class APB2BridgePeripherals : uint8_t { _SPI5 = 20, _TIM11 = 18, _TIM10 = 17, _TIM9 = 16, _SYSCGF = 14, _SPI4 = 13, _SPI1 = 12, _SDIO = 11, _ADC1 = 8, _USART6 = 5, _USART1 = 4, _TIM1 = 0 };

template <typename T>
concept AnyOfPeripheralsBridge = std::same_as<T, AHB1BridgePeripherals> || 
                           std::same_as<T, AHB2BridgePeripherals> || 
                           std::same_as<T, APB1BridgePeripherals> || 
                           std::same_as<T, APB2BridgePeripherals>;


template <PeripheralBridges Bridge>
class PeripheralBridge
{
    public:
        //template<EnumType 
        virtual ~PeripheralBridge() = 0;
        PeripheralBridge(const PeripheralBridge&) = delete;
        PeripheralBridge& operator=(const PeripheralBridge&) = delete;

    protected:

        explicit PeripheralBridge();

        template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
        void enablePeripheralClock(const anyOfPeripheralsBridge& index, const bool& lowPowerMode = false);

        template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
        void disablePeripheralClock(const anyOfPeripheralsBridge& index);

        template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
        void enablePeripheralClockLowPowerMode(const anyOfPeripheralsBridge& index);

        template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
        void disablePeripheralClockLowPowerMode(const anyOfPeripheralsBridge& index);

        template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
        void resetPeripheral(const anyOfPeripheralsBridge& index);

    private:

        static constexpr volatile uint32_t* getResetRegisterPointer();
        static constexpr volatile uint32_t* getEnableRegisterPointer();
        static constexpr volatile uint32_t* getLowPowerEnableRegisterPointer();

        template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
        void setBit(const anyOfPeripheralsBridge& index, volatile uint32_t* const& reg);

        template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
        void resetBit(const anyOfPeripheralsBridge& index, volatile uint32_t* const& reg);

        volatile uint32_t* const PeripheralResetRegister{ getResetRegisterPointer() };
        volatile uint32_t* const PeripheralEnableRegister{ getEnableRegisterPointer() };
        volatile uint32_t* const PeripheralLowPowerEnableRegister { getLowPowerEnableRegisterPointer() };
};


template<PeripheralBridges Bridge>
PeripheralBridge<Bridge>::PeripheralBridge()
{
    
}

template<PeripheralBridges Bridge>
PeripheralBridge<Bridge>::~PeripheralBridge()
{

}



template<PeripheralBridges Bridge>
constexpr volatile uint32_t* PeripheralBridge<Bridge>::getResetRegisterPointer()
{
    volatile uint32_t* ret{nullptr};
    if constexpr(Bridge == PeripheralBridges::AHB1)    
        ret = &RCC->AHB1RSTR;
    else if constexpr(Bridge == PeripheralBridges::AHB2)    
        ret = &RCC->AHB2RSTR;
    else if constexpr(Bridge == PeripheralBridges::APB1)    
        ret = &RCC->APB1RSTR;
    else if constexpr(Bridge == PeripheralBridges::APB2)    
        ret = &RCC->APB2RSTR;
    return ret;
}

template<PeripheralBridges Bridge>
constexpr volatile uint32_t* PeripheralBridge<Bridge>::getEnableRegisterPointer()
{
    volatile uint32_t* ret{nullptr};
    if constexpr(Bridge == PeripheralBridges::AHB1)    
        ret = &RCC->AHB1ENR;
    else if constexpr(Bridge == PeripheralBridges::AHB2)    
        ret = &RCC->AHB2ENR;
    else if constexpr(Bridge == PeripheralBridges::APB1)    
        ret = &RCC->APB1ENR;
    else if constexpr(Bridge == PeripheralBridges::APB2)    
        ret = &RCC->APB2ENR;
    return ret;
}

template<PeripheralBridges Bridge>
constexpr volatile uint32_t* PeripheralBridge<Bridge>::getLowPowerEnableRegisterPointer()
{
    volatile uint32_t* ret{nullptr};
    if constexpr(Bridge == PeripheralBridges::AHB1)    
        ret = &RCC->AHB1LPENR;
    else if constexpr(Bridge == PeripheralBridges::AHB2)    
        ret = &RCC->AHB2LPENR;
    else if constexpr(Bridge == PeripheralBridges::APB1)    
        ret = &RCC->APB1LPENR;
    else if constexpr(Bridge == PeripheralBridges::APB2)    
        ret = &RCC->APB2LPENR;
    return ret;
    
}

template<PeripheralBridges Bridge>
template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
void PeripheralBridge<Bridge>::setBit(const anyOfPeripheralsBridge& index, volatile uint32_t* const& reg)
{
    *reg |= (0x1 << static_cast<uint32_t>(index));
}

template<PeripheralBridges Bridge>
template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
void PeripheralBridge<Bridge>::resetBit(const anyOfPeripheralsBridge& index, volatile uint32_t* const& reg)
{
    *reg &= ~(0x1 << static_cast<uint32_t>(index));
}

template<PeripheralBridges Bridge>
template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
void PeripheralBridge<Bridge>::enablePeripheralClock(const anyOfPeripheralsBridge& index, const bool& lowPowerMode)
{
    if(lowPowerMode)
    {
        enablePeripheralClockLowPowerMode(index);
        return ;
    }
    setBit(index, PeripheralEnableRegister);
}

template<PeripheralBridges Bridge>
template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
void PeripheralBridge<Bridge>::disablePeripheralClock(const anyOfPeripheralsBridge& index)
{
    resetBit(index, PeripheralEnableRegister);
}

template<PeripheralBridges Bridge>
template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
void PeripheralBridge<Bridge>::enablePeripheralClockLowPowerMode(const anyOfPeripheralsBridge& index)
{
    setBit(index, PeripheralLowPowerEnableRegister);
}

template<PeripheralBridges Bridge>
template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
void PeripheralBridge<Bridge>::disablePeripheralClockLowPowerMode(const anyOfPeripheralsBridge& index)
{
    resetBit(index, PeripheralLowPowerEnableRegister);
}

template<PeripheralBridges Bridge>
template<AnyOfPeripheralsBridge anyOfPeripheralsBridge>
void PeripheralBridge<Bridge>::resetPeripheral(const anyOfPeripheralsBridge& index)
{
    setBit(index, PeripheralResetRegister);
}



class AHB1Bridge : public PeripheralBridge<PeripheralBridges::AHB1>
{
    // Implementation for AHB1
    public:
        using baseClass = PeripheralBridge<PeripheralBridges::AHB1>;
        static AHB1Bridge* getInstancePtr();

        virtual ~AHB1Bridge() { };
        void enablePeripheralClock(const AHB1BridgePeripherals& index) { baseClass::enablePeripheralClock(index); } 
        void disablePeripheralClock(const AHB1BridgePeripherals& index) { baseClass::disablePeripheralClock(index); } 
        void enablePeripheralClockLowPoderMode(const AHB1BridgePeripherals& index) { baseClass::enablePeripheralClockLowPowerMode(index); } 
        void disablePeripheralClockLowPoderMode(const AHB1BridgePeripherals& index) { baseClass::disablePeripheralClockLowPowerMode(index); } 
        void resetPeripheral(const AHB1BridgePeripherals& index) { baseClass::resetPeripheral(index); } 
        //void enableGPIOA() { }
    private:
        explicit AHB1Bridge() : baseClass() { };
        static AHB1Bridge* instancePtr;
};




class AHB2Bridge : public PeripheralBridge<PeripheralBridges::AHB2>
{
    public:
        using baseClass = PeripheralBridge<PeripheralBridges::AHB2>;
        static AHB2Bridge* getInstancePtr();

        virtual ~AHB2Bridge() { };
        void enablePeripheralClock(const AHB2BridgePeripherals& index) { baseClass::enablePeripheralClock(index); } 
        void disablePeripheralClock(const AHB2BridgePeripherals& index) { baseClass::disablePeripheralClock(index); } 
        void enablePeripheralClockLowPowerMode(const AHB2BridgePeripherals& index) { baseClass::enablePeripheralClockLowPowerMode(index); } 
        void disablePeripheralClockLowPowerMode(const AHB2BridgePeripherals& index) { baseClass::disablePeripheralClockLowPowerMode(index); } 
        void resetPeripheral(const AHB2BridgePeripherals& index) { baseClass::resetPeripheral(index); } 

    private:
        explicit AHB2Bridge() : baseClass() { };
        static AHB2Bridge* instancePtr;
};


class APB1Bridge : public PeripheralBridge<PeripheralBridges::APB1>
{
    public:
        using baseClass = PeripheralBridge<PeripheralBridges::APB1>;
        static APB1Bridge* getInstancePtr();

        virtual ~APB1Bridge() { };
        void enablePeripheralClock(const APB1BridgePeripherals& index) { baseClass::enablePeripheralClock(index); } 
        void disablePeripheralClock(const APB1BridgePeripherals& index) { baseClass::disablePeripheralClock(index); } 
        void enablePeripheralClockLowPowerMode(const APB1BridgePeripherals& index) { baseClass::enablePeripheralClockLowPowerMode(index); } 
        void disablePeripheralClockLowPowerMode(const APB1BridgePeripherals& index) { baseClass::disablePeripheralClockLowPowerMode(index); } 
        void resetPeripheral(const APB1BridgePeripherals& index) { baseClass::resetPeripheral(index); } 

    private:
        explicit APB1Bridge() : baseClass() { };
        static APB1Bridge* instancePtr;
};



class APB2Bridge : public PeripheralBridge<PeripheralBridges::APB2>
{
    public:
        using baseClass = PeripheralBridge<PeripheralBridges::APB2>;
        static APB2Bridge* getInstancePtr();

        virtual ~APB2Bridge() { };
        void enablePeripheralClock(const APB2BridgePeripherals& index) { baseClass::enablePeripheralClock(index); } 
        void disablePeripheralClock(const APB2BridgePeripherals& index) { baseClass::disablePeripheralClock(index); } 
        void enablePeripheralClockLowPowerMode(const APB2BridgePeripherals& index) { baseClass::enablePeripheralClockLowPowerMode(index); } 
        void disablePeripheralClockLowPowerMode(const APB2BridgePeripherals& index) { baseClass::disablePeripheralClockLowPowerMode(index); } 
        void resetPeripheral(const APB2BridgePeripherals& index) { baseClass::resetPeripheral(index); } 

    private:
        explicit APB2Bridge() : baseClass() { };
        static APB2Bridge* instancePtr;
};


#endif

#endif // __PERIPHERALCLOCKS_H__