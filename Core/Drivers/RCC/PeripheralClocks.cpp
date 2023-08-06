#include <PeripheralClocks.hh>

#ifdef CCC


// Definitions for static member variables
AHB1Bridge* AHB1Bridge::instancePtr = nullptr;
AHB2Bridge* AHB2Bridge::instancePtr = nullptr;
APB1Bridge* APB1Bridge::instancePtr = nullptr;
APB2Bridge* APB2Bridge::instancePtr = nullptr;

AHB1Bridge* AHB1Bridge::getInstancePtr()
{
    if(instancePtr == nullptr)
    {
        instancePtr = new AHB1Bridge();
    }
    return instancePtr;
}

AHB2Bridge* AHB2Bridge::getInstancePtr()
{
    if(instancePtr == nullptr)
    {
        instancePtr = new AHB2Bridge();
    }
    return instancePtr;
}

APB1Bridge* APB1Bridge::getInstancePtr()
{
    if(instancePtr == nullptr)
    {
        instancePtr = new APB1Bridge();
    }
    return instancePtr;
}

APB2Bridge* APB2Bridge::getInstancePtr()
{
    if(instancePtr == nullptr)
    {
        instancePtr = new APB2Bridge();
    }
    return instancePtr;
}

#endif