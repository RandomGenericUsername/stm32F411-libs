#ifndef __RCC_H__
#define __RCC_H__

#ifdef COMPILE

#include <stdint.h>
#include <Container.hh>
#include <PeripheralBase.hh>
#include <PeripheralClocks.hh>




enum class rccProperties : uint8_t { ClockSource, PLL,  PeripheralClocks, __length };

enum class rccClockSource : uint8_t { MainPLL, HSI, HSE };

struct PLL
{
    uint32_t m;
    uint32_t n;
    uint32_t p;
    uint32_t q;
};

struct rccPeripheralClocks
{
    AHB1Bridge* AHB1 { nullptr };
    AHB2Bridge* AHB2 { nullptr };
    APB1Bridge* APB1 { nullptr };
    APB2Bridge* APB2 { nullptr };
};

using RCCSettingsContainer = Container<rccProperties, rccClockSource, PLL, rccPeripheralClocks>;

enum class RCCStatusCodes : uint8_t { Reset, Ready };

class ResetAndClockControl : public PeripheralBase<RCC_TypeDef, RCCStatusCodes, RCCSettingsContainer>
{
    public:
    
        static ResetAndClockControl* getInstance();
        virtual ~ResetAndClockControl();

        // No copy constructor or assignment operator
        ResetAndClockControl(const ResetAndClockControl& other) = delete;
        ResetAndClockControl& operator=(const ResetAndClockControl& other) = delete;

        RCCStatusCodes init();
        void enablePeripheralClockGPIOA();
        void enablePeripheralClockGPIOB();
        void enablePeripheralClockGPIOC();
        void enablePeripheralClockGPIOD();

    protected:


    private:

        explicit ResetAndClockControl();
        static ResetAndClockControl* self;

        void setDefaultSettings();
        RCCStatusCodes preInit();
        void setInstancePtr();
        void setClockSource(const rccClockSource& clockSource);
        bool isMainPLLReady();
        void enableMainPLL(const bool& enable);


};



//PeripheralBase<GPIO_TypeDef*, TestPeripheralEnumError, setup > asdf{ GPIOA };
//Container

#endif

#endif // __RCC_H__