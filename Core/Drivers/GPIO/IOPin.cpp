#include <IOPin.hh>


#ifdef COMPILE

IOPin::IOPin() 
    : IOPinParent(IOPinFunctionsContainer{&IOPin::configPort, &IOPin::configPin, &IOPin::configMode, &IOPin::configState})
{
    //setConfigFunctionsExecutionOrder(IOPinFunctionsExecutionOrder{IOPinProperties::mode, IOPinProperties::pin, IOPinProperties::port});
    //setAllParametersValue(GpioPort::A, GpioPin::null, GpioMode::alternateFunction, GpioState::null);
    std::size_t a = this->mandatoryParameter;
    a = a / 32;

}

IOPin::IOPin(const IOPin &other) : IOPin()
{
    
}

IOPin::IOPin(const IOPin &other, const GpioPin &pin) : IOPin()
{
    
}

IOPin::IOPin(const IOPin &other, const GpioPort &port) : IOPin()
{
    
}

IOPin::IOPin(IOPin &&other) : IOPin()
{
    
}

IOPin& IOPin::operator=(const IOPin &other)
{
   return *this; 
}

IOPin& IOPin::operator=(IOPin &&other)
{
    
   return *this; 
}

IOPin::~IOPin()
{
    
}

IOPinStatusCodes IOPin::setPort(const GpioPort &port)
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::setPin(const GpioPin &pin)
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::setMode(const GpioMode &mode)
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::setPUPD(const GpioPUPD &pupd)
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::setOutputType(const GpioOutputType &outputType)
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::setOutputSpeed(const GpioOutputSpeed &outputSpeed)
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::setInputMode()
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::setOutputMode()
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::setPullup()
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::setPulldown()
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::setPullPushOutputType()
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::setOpenDrainOutputType()
{ 
    return IOPinStatusCodes::Reset;
}
bool IOPin::read()
{
    return true;
}
IOPinStatusCodes IOPin::write(const bool &state)
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::toggle()
{ 
    return IOPinStatusCodes::Reset;
}
IOPinStatusCodes IOPin::reset(const bool &forceReset)
{ 
    return IOPinStatusCodes::Reset;
}
bool IOPin::isReady()
{
    return true;
}
bool IOPin::isAllocated(const GpioPort &port, const GpioPin &pin)
{
    return true;
}
bool IOPin::isAllocated(const GPIO_TypeDef *port, const GpioPin &pin)
{
    return true;
}


bool IOPin::isPinSet()
{
    
}

bool IOPin::isPortSet()
{
    
}

bool IOPin::isReallocating(const GpioPin &pin)
{
    
}

bool IOPin::isReallocating(const GpioPort &port)
{
    
}

GPIO_TypeDef* IOPin::getGPIOPtrInstance(const GpioPort &port)
{
    
}

GpioPort IOPin::getPortFromGPIOStruct(const GPIO_TypeDef* gpio)
{
    
}

void IOPin::allocatePin()
{
    
}

void IOPin::deAllocatePin()
{
    
}

IOPinStatusCodes IOPin::init()
{
    return IOPinStatusCodes::Reset;
}

void IOPin::setInstancePtr()
{
    
}
void IOPin::setDefaultSettings()
{
    
}

IOPinStatusCodes IOPin::preInit()
{
    return IOPinStatusCodes::Reset;
}

#endif