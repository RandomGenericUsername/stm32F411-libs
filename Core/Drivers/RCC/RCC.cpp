#include <RCC.hh>


#ifdef CCC
ResetAndClockControl* ResetAndClockControl::self = nullptr;


ResetAndClockControl::ResetAndClockControl()
{
    setInstancePtr();
    //setDefaults();
}

ResetAndClockControl::~ResetAndClockControl()
{
    self = nullptr;
}

void ResetAndClockControl::setDefaultSettings()
{
    settings->setMemberValue<rccProperties::ClockSource>(rccClockSource::HSI);
    settings->setMemberValue<rccProperties::PLL>(PLL 
    { 
        .m = 0,
        .n = RCC_PLLN_MIN_VALUE,
    });
    settings->setMemberValue<rccProperties::PeripheralClocks>(rccPeripheralClocks
    {
        .AHB1 = AHB1Bridge::getInstancePtr(),
        .AHB2 = AHB2Bridge::getInstancePtr(),
        .APB1 = APB1Bridge::getInstancePtr(),
        .APB2 = APB2Bridge::getInstancePtr(),
    });
}


ResetAndClockControl* ResetAndClockControl::getInstance()
{
    if (self == nullptr) 
    {
        self = new ResetAndClockControl();
    }
    return self; 
}

RCCStatusCodes ResetAndClockControl::preInit()
{
    return RCCStatusCodes::Ready;    
}

void ResetAndClockControl::setInstancePtr()
{
    this->instance = RCC;    
}

void ResetAndClockControl::setClockSource(const rccClockSource& clockSource)
{
    
}

bool ResetAndClockControl::isMainPLLReady()
{
    return PeripheralBase::instance->CR & 0x1 << 25;
}

void ResetAndClockControl::enableMainPLL(const bool& enable)
{
    PeripheralBase::instance->CR |= enable << 24;
}

RCCStatusCodes ResetAndClockControl::init()
{
    return RCCStatusCodes::Ready;    
}

void ResetAndClockControl::enablePeripheralClockGPIOA()
{
    settings->getMemberValue<rccProperties::PeripheralClocks>().AHB1->enablePeripheralClock(AHB1BridgePeripherals::_GPIOA);
}

void ResetAndClockControl::enablePeripheralClockGPIOB()
{
    settings->getMemberValue<rccProperties::PeripheralClocks>().AHB1->enablePeripheralClock(AHB1BridgePeripherals::_GPIOB);
}

void ResetAndClockControl::enablePeripheralClockGPIOC()
{
    settings->getMemberValue<rccProperties::PeripheralClocks>().AHB1->enablePeripheralClock(AHB1BridgePeripherals::_GPIOC);
}

void ResetAndClockControl::enablePeripheralClockGPIOD()
{
    settings->getMemberValue<rccProperties::PeripheralClocks>().AHB1->enablePeripheralClock(AHB1BridgePeripherals::_GPIOD);
}

#endif