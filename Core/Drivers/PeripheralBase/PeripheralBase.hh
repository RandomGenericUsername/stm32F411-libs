#ifndef __PERIPHERALBASE_H__
#define __PERIPHERALBASE_H__

/**
 * @file PeripheralBase.hh
 * @author Juan David Tangarife Hernandez (you@domain.com)
 * @brief Base class driver for all of the peripherals on STM32 microcontroller
 * @version 0.1
 * @date 2023-04-15
 * 
 * @copyright Copyright (c) 2023
 * 
*/

/**
 * @brief  Required inclued headers
 */
#include <system.h>
#include <PeripheralBaseTypes.hh>
#include <Container.hh>
#include <bitset>

template <typename Instance>
struct PeripheralMap;

template <>
struct PeripheralMap<GPIO_TypeDef> {
    static auto instances() 
    {
        return std::make_tuple(GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOH);
    }
};

template <>
struct PeripheralMap<USART_TypeDef> {
    static auto instances()
    {
        std::make_tuple(USART1, USART2, USART6);
    }
};

template <>
struct PeripheralMap<RCC_TypeDef> {
    static auto instances()
    {
        std::make_tuple(RCC);
    }
};

template<typename T>
struct StaticManager { std::size_t classInstanceCounter; };

/**
 * @brief 
 * 
 * @tparam Instance is a pointer to a struct that maps STM32 peripheral's register's address
 * @tparam StatusCode is an enum class listing all the possible errors/malfunctioning (hardware related) for that particular peripheral
 * @tparam Parameter is an object of the type "types"
 */


template<STM32Peripheral Instance, typename StatusCode, typename ParametersLabels, typename ...Parameters>
class PeripheralBase; 


template<STM32Peripheral  Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
class PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>> 
{
    public:
        //<----------------------------------------------Public constructors---------------------------------------------->//
        virtual ~PeripheralBase();
        //<---------------------------------------------------------------------------------------------------------->//

    protected:

        using PeripheralBaseType = PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>;
        using FunctionsContainerType = Container<ParametersLabels, bool(*)(const Parameters&, PeripheralBaseType*) ...>;
        using ConfigFunctionsResponseType = std::array<bool, sizeof...(Parameters)>;

        //<---------------------------------------------- constructors---------------------------------------------->//
        template<typename... Functions>
        constexpr explicit PeripheralBase(const Container<ParametersLabels, Functions...>& functions);

        constexpr explicit PeripheralBase(const PeripheralBase& other);
        constexpr explicit PeripheralBase(PeripheralBase &&other);
        //<---------------------------------------------------------------------------------------------------------->//

        //<---------------------------------------------- Getter methods ---------------------------------------------->//
        bool isResetOrReady();
        StatusCode getStatus();

        template<ParametersLabels Parameter>
        auto&& getParameterValue();
        //<---------------------------------------------------------------------------------------------------------->//


        //<---------------------------------------------- Setter methods ---------------------------------------------->//
        void setStatus(const StatusCode& status);

        void setAllParametersValue(Parameters&&... parameters);

        template<ParametersLabels Param, typename Value>
        void setParameterValue(const Value& value);

        template<typename... Args>
        void setParametersValues(const Args&... args);
        //<---------------------------------------------------------------------------------------------------------->//

        //<---------------------------------------------- Utility methods ---------------------------------------------->//

        template<typename T>
        constexpr bool assertArgumentType();

        template<typename T>
        constexpr std::vector<uint8_t> isValueSet(const T& value);

        template<typename T>
        constexpr bool isValueSetBool(const T& value);
        //<---------------------------------------------------------------------------------------------------------->//

        //<---------------------------------------------- Hardware initialization methods --------------------------->//
        bool init(const bool& checkMandatoryParameters = true);

        //<---------------------------------------------------------------------------------------------------------->//

        //Children-accessible class members
        Instance* instance { nullptr };
        void setParameterAsMandatory(const ParametersLabels& parameter);

    private:

        //<---------------------------------------------------------------------------------------------------------->//
        void setInstancePtr(const Instance& peripheral);  

        template<typename... Functions>
        void setFunctionsPtr(const Container<ParametersLabels, Functions...>& functions);

        template<std::size_t I = 0>
        constexpr bool invokeAllConfigFunctionsHelper(std::array<bool, sizeof...(Parameters)>& v);

        template<std::size_t I>
        constexpr bool invokeConfigFunction();

        template<ParametersLabels Parameter>
        constexpr bool invokeConfigFunction();

        constexpr ConfigFunctionsResponseType invokeAllConfigFunctions();
        //<---------------------------------------------------------------------------------------------------------->//

        //<-------------------------------------------- Pure virtual methods --------------------------------------->//
        virtual void setDefaultSettings() = 0;
        //<---------------------------------------------------------------------------------------------------------->//

        static StaticManager<PeripheralBaseType> staticManager;
        static const constexpr std::size_t numberOfProperties { sizeof...(Parameters) };
        static std::bitset<static_cast<size_t>(sizeof...(Parameters))> mandatoryParameter;
        static FunctionsContainerType *const functions;

        StatusCode status { StatusCode::Reset };
        Container<ParametersLabels, Parameters...>*const parameters { nullptr };
};

template<STM32Peripheral Instance, typename StatusCode, typename ParametersLabels, typename... Parameters>
StaticManager<PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>> PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::staticManager{0};

template<STM32Peripheral  Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
std::bitset<static_cast<size_t>(sizeof...(Parameters))> PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::mandatoryParameter{0};


template<STM32Peripheral  Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
Container<ParametersLabels, bool(*)(const Parameters&, PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>*) ...>*const PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::functions = new FunctionsContainerType;

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
template <typename... Functions>
inline constexpr PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::PeripheralBase(const Container<ParametersLabels, Functions...> &functions)
    :  parameters(new Container<ParametersLabels, Parameters...>)
{
    setFunctionsPtr(functions);
    staticManager.classInstanceCounter++;
    bool zz;
    for(uint8_t i = 0; i < numberOfProperties; i++)
    {

        if(mandatoryParameter.test(i))
        {
            if(i ==0)
            {
                constexpr ParametersLabels index = static_cast<ParametersLabels>(0);
                using aa = decltype(parameters->template getMemberValue<index>());
                zz = isValueSetBool<aa>(aa::null);
            }
            if(i ==1)
            {
                constexpr ParametersLabels index = static_cast<ParametersLabels>(1);
                using aa = decltype(parameters->template getMemberValue<index>());
                zz = isValueSetBool<aa>(aa::null);
            }
            if(i ==2)
            {
                constexpr ParametersLabels index = static_cast<ParametersLabels>(2);
                using aa = decltype(parameters->template getMemberValue<index>());
                zz = isValueSetBool<aa>(aa::null);
            }
            if(i ==3)
            {
                constexpr ParametersLabels index = static_cast<ParametersLabels>(3);
                using aa = decltype(parameters->template getMemberValue<index>());
                zz = isValueSetBool<aa>(aa::null);
            }
        }
    }
}


template<STM32Peripheral  Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::~PeripheralBase()
{
    staticManager.classInstanceCounter--;
    mandatoryParameter = 0;

    instance = nullptr;
    status = StatusCode::Reset;
    delete parameters;
}


template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
constexpr PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::PeripheralBase(const PeripheralBase &other)
    : parameters(new Container<ParametersLabels, Parameters...>(other.parameters))
{
     
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
constexpr PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::PeripheralBase(PeripheralBase &&other)
    : parameters( new Container<ParametersLabels, Parameters...>(std::move(other.parameters)))
{
     
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
inline bool PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::init(const bool& check)
{

}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
inline void PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::setInstancePtr(const Instance &peripheral)
{
    constexpr auto instances = PeripheralMap<Instance>::instances();
    std::apply([this, &peripheral](auto&&... instance) {
        ((instance == peripheral ? this->instance = instance : false), ...);
    }, instances);
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
inline void PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::setParameterAsMandatory(const ParametersLabels &parameter)
{
    mandatoryParameter.set(static_cast<size_t>(parameter));
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
template<typename... Functions>
void PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::setFunctionsPtr(const Container<ParametersLabels, Functions...>& functions)
{
    if (!this->staticManager.classInstanceCounter)
    {
        *(this->functions) = functions;
    }
}

template<STM32Peripheral  Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
void PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::setStatus(const StatusCode& _status)
{
   this->status = _status; 
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
inline void PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::setAllParametersValue(Parameters&&...args)
{
    this->parameters->template setAllMemberValues<Parameters...>(std::forward<Parameters>(args)...);
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
template<ParametersLabels Param, typename Value>
inline void PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::setParameterValue(const Value& value)
{
    parameters->template setMemberValue<Param>(value);
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
template <typename... Args>
inline void PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::setParametersValues(const Args &...args)
{
    parameters->template setMembersValues<Args...>(args...);
}

template<STM32Peripheral  Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
StatusCode PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::getStatus()
{
    return status;    
}

template<STM32Peripheral  Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
template<typename T>
constexpr bool PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::assertArgumentType()
{
    return this->parameters->template isTypeIn<T>();
}

template<STM32Peripheral  Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
template<typename T>
constexpr std::vector<uint8_t> PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::isValueSet(const T& value)
{
    return this->parameters->template isTypeAndValueIn<T>(value);
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
template <typename T>
inline constexpr bool PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::isValueSetBool(const T &value)
{
    if (isValueSet<T>(value).size())
        return true;
    return false;
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
template <ParametersLabels Parameter>
constexpr bool PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::invokeConfigFunction()
{
    auto && arg = this->parameters->template getMemberValue<Parameter>();
    return this->functions->template getMemberValue<Parameter>()(arg, this);
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
template <std::size_t I>
constexpr bool PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::invokeConfigFunction()
{
    constexpr ParametersLabels functionIndex = static_cast<ParametersLabels>(I);
    auto && arg = this->parameters->template getMemberValue<functionIndex>();
    return this->functions->template getMemberValue<functionIndex>()(arg, this);
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
constexpr std::array<bool, sizeof...(Parameters)> PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::invokeAllConfigFunctions()
{
    std::array<bool, numberOfProperties> results;
    invokeAllConfigFunctionsHelper<0>(results);
    return results;
}

template <STM32Peripheral Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
template <std::size_t I>
constexpr bool PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::invokeAllConfigFunctionsHelper(std::array<bool, sizeof...(Parameters)>& v)
{
    if constexpr(I < numberOfProperties)
    {
        v[I] = invokeConfigFunction<I>();
        return invokeAllConfigFunctionsHelper<I+1>(v);
    }
    return true;
}

template<STM32Peripheral  Instance, EnumType StatusCode, EnumType ParametersLabels, typename... Parameters>
bool PeripheralBase<Instance, StatusCode, Container<ParametersLabels, Parameters...>>::isResetOrReady()
{
    if(this->status == StatusCode::Ready || this->status == StatusCode::Reset)
        return true;
    return false;    
}

#endif // __PERIPHERALBASE_H__
