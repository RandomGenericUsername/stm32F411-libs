#ifndef __IOPIN_H__
#define __IOPIN_H__

#include <IOPinTypes.hh>
#include <Container.hh>
#include <PeripheralBase.hh>
#include <IOPinTypes.hh>


#define COMPILE
#ifdef COMPILE

#define AVAILABLE_PORTS 7U

enum class IOPinStatusCodes 
{ 
    Reset,
    Ready,
    ArgumentTypeNotAllowed,
    alreadyAllocatedPin,
    handlerNotAllocated,
    initQueuedSettingsFailed,
    modeNotAllowed,
    notReadyNotReset,
    readingDeallocatedPin,
};

//enum class IOPinProperties : uint8_t { port, pin, mode, state, outputType, outputSpeed, pupd, __length };
//using IOPinPropertiesContainer = Container<IOPinProperties, GpioPort, GpioPin, GpioMode, GpioState, GpioOutputType, GpioOutputSpeed, GpioPUPD>;

enum class IOPinProperties : uint8_t { port, pin, mode, state,  __length };
using IOPinPropertiesContainer = Container<IOPinProperties, GpioPort, GpioPin, GpioMode, GpioState>;
using IOPinParent = PeripheralBase<GPIO_TypeDef, IOPinStatusCodes,  IOPinPropertiesContainer>;
using ConfigPortFunctionType = bool(*)(const GpioPort&, IOPinParent*);
using ConfigPinFunctionType = bool(*)(const GpioPin&, IOPinParent*);
using ConfigModeFunctionType = bool(*)(const GpioMode&, IOPinParent*);
using ConfigStateFunctionType = bool(*)(const GpioState&, IOPinParent*);
using IOPinFunctionsContainer = Container<IOPinProperties, ConfigPortFunctionType, ConfigPinFunctionType, ConfigModeFunctionType, ConfigStateFunctionType>;


class IOPin  : public IOPinParent 
{
    public:

        // Constructors & Destructor
        explicit IOPin();
        virtual ~IOPin();

        template <typename... T>
        explicit IOPin(const T &...args);

        // Creates new object by copying the same settings as "other"
        explicit IOPin(const IOPin &other);

        // Create new object by copying the same settings as either other port or pin is allowed
        explicit IOPin(const IOPin &other, const GpioPin &pin);
        explicit IOPin(const IOPin &other, const GpioPort &port);

        // Transferring ownerwhips is allowed. Destroys the other object
        explicit IOPin(IOPin &&other);

        // Copies the settings of the current pin
        IOPin &operator=(const IOPin &other);
        // transfer ownership is allowed
        IOPin &operator=(IOPin &&other);


        // Main functions that the interface offers
        IOPinStatusCodes setPort(const GpioPort &port);
        IOPinStatusCodes setPin(const GpioPin &pin);
        IOPinStatusCodes setMode(const GpioMode &mode);
        IOPinStatusCodes setPUPD(const GpioPUPD &pupd);
        IOPinStatusCodes setOutputType(const GpioOutputType &outputType);
        IOPinStatusCodes setOutputSpeed(const GpioOutputSpeed &outputSpeed);

        IOPinStatusCodes setInputMode();
        IOPinStatusCodes setOutputMode();
        IOPinStatusCodes setPullup();
        IOPinStatusCodes setPulldown();
        IOPinStatusCodes setPullPushOutputType();
        IOPinStatusCodes setOpenDrainOutputType();

        IOPinStatusCodes write(const bool &state);
        IOPinStatusCodes toggle();
        IOPinStatusCodes reset(const bool &forceReset = false);
        bool read();
        bool isReady();

        // Utility public functions
        static bool isAllocated(const GpioPort &port, const GpioPin &pin);
        static bool isAllocated(const GPIO_TypeDef *port, const GpioPin &pin);

    protected:

    private:

        bool isPinSet();
        bool isPortSet();
        bool isReallocating(const GpioPin &pin);
        bool isReallocating(const GpioPort &port);
        GPIO_TypeDef *getGPIOPtrInstance(const GpioPort &port);
        static GpioPort getPortFromGPIOStruct(const GPIO_TypeDef* gpio);
        void allocatePin();
        void deAllocatePin();

        IOPinStatusCodes readInput();
        IOPinStatusCodes writeOutput(const bool& state);
        IOPinStatusCodes toogleOutput();
        IOPinStatusCodes resetPin(const bool& force = false);
        IOPinStatusCodes initQueuedSettings(const bool &triggerError = false, const uint8_t &retries = 5);
        IOPinStatusCodes setGPIOModeWithParameterChecking(const GpioMode &mode, const bool &check);
        IOPinStatusCodes setGpioMode(const GpioMode &mode);

        template<typename T>
        void setConfig(const T& param);
        template <typename T>
        bool setGPIOSettings(const T &param);
        template <typename T>
        bool setPortAndPin(const T &param);


        IOPinStatusCodes init();
        IOPinStatusCodes preInit();
        void setDefaultSettings();
        template <typename Arg>
        IOPinStatusCodes init(const Arg &arg);

        constexpr static bool configPort(const GpioPort& mode, IOPinParent* obj) {

            return true; 
        }
        constexpr static bool configPin(const GpioPin& pin, IOPinParent* obj) {

            uint32_t a = 32;
            uint32_t b = a / 41;
            return true; 
        }
        constexpr static bool configMode(const GpioMode& pin, IOPinParent* obj) {

            uint32_t a = 32;
            uint32_t b = a / 41;
            return true; 
        }
        constexpr static bool configState(const GpioState& pin, IOPinParent* obj) {

            uint32_t a = 32;
            uint32_t b = a / 41;
            return true; 
        }

        void setInstancePtr();

        static AllocatedPin_t allocatedIOPins[AVAILABLE_PORTS];
};
template<typename... T>
IOPin::IOPin(const T &...args)
{

}

template<typename Arg>
IOPinStatusCodes IOPin::init(const Arg &arg)
{

}


#endif

#endif // __IOPIN_H__