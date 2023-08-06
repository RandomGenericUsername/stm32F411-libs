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
#include <PeripheralBaseTypes.hh>
#include <Container.hh>
#include <bitset>


template<typename T>
struct StaticManager { std::size_t classInstanceCounter; };

/**
 * @brief 
 * 
 * @tparam Instance is a pointer to a struct that maps STM32 peripheral's register's address
 * @tparam StatusCode is an enum class listing all the possible errors/malfunctioning (hardware related) for that particular peripheral
 * @tparam Parameter is an object of the type "types"
 */


template<typename StatusCode, std::size_t MandatoryParameters,  typename ParametersLabels, typename ...Parameters>
class PeripheralBase; 


template<EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
class PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>> 
{
    public:
        //<----------------------------------------------Public constructors---------------------------------------------->//
        virtual ~PeripheralBase();
        //<---------------------------------------------------------------------------------------------------------->//

    protected:

        using PeripheralBaseType = PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>;
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
        void setParameterAsMandatory(const ParametersLabels& parameter);

    private:

        //<---------------------------------------------------------------------------------------------------------->//

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
        constexpr static std::bitset<static_cast<size_t>(sizeof...(Parameters))> mandatoryParameter { MandatoryParameters };
        static FunctionsContainerType *const functions;

        StatusCode status { StatusCode::Reset };
        Container<ParametersLabels, Parameters...>*const parameters { nullptr };
};

template<typename StatusCode, std::size_t MandatoryParameters, typename ParametersLabels, typename... Parameters>
StaticManager<PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>> PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::staticManager{0};

template<EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
Container<ParametersLabels, bool(*)(const Parameters&, PeripheralBase<StatusCode,  MandatoryParameters, Container<ParametersLabels, Parameters...>>*) ...>*const PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::functions = new FunctionsContainerType;

template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
template <typename... Functions>
inline constexpr PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::PeripheralBase(const Container<ParametersLabels, Functions...> &functions)
    :  parameters(new Container<ParametersLabels, Parameters...>)
{
    setFunctionsPtr(functions);
    staticManager.classInstanceCounter++;
}


template<EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
PeripheralBase<StatusCode,  MandatoryParameters, Container<ParametersLabels, Parameters...>>::~PeripheralBase()
{
    staticManager.classInstanceCounter--;
    status = StatusCode::Reset;
    delete parameters;
}


template <EnumType StatusCode, std::size_t  MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
constexpr PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::PeripheralBase(const PeripheralBase &other)
    : parameters(new Container<ParametersLabels, Parameters...>(other.parameters))
{
     
}

template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
constexpr PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::PeripheralBase(PeripheralBase &&other)
    : parameters( new Container<ParametersLabels, Parameters...>(std::move(other.parameters)))
{
     
}

template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
inline bool PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::init(const bool& check)
{

}


template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
inline void PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::setParameterAsMandatory(const ParametersLabels &parameter)
{
}

template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
template<typename... Functions>
void PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::setFunctionsPtr(const Container<ParametersLabels, Functions...>& functions)
{
    if (!this->staticManager.classInstanceCounter)
    {
        *(this->functions) = functions;
    }
}

template<EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
void PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::setStatus(const StatusCode& _status)
{
   this->status = _status; 
}

template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
inline void PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::setAllParametersValue(Parameters&&...args)
{
    this->parameters->template setAllMemberValues<Parameters...>(std::forward<Parameters>(args)...);
}

template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
template<ParametersLabels Param, typename Value>
inline void PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::setParameterValue(const Value& value)
{
    parameters->template setMemberValue<Param>(value);
}

template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
template <typename... Args>
inline void PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::setParametersValues(const Args &...args)
{
    parameters->template setMembersValues<Args...>(args...);
}

template<EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
StatusCode PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::getStatus()
{
    return status;    
}

template<EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
template<typename T>
constexpr bool PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::assertArgumentType()
{
    return this->parameters->template isTypeIn<T>();
}

template<EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
template<typename T>
constexpr std::vector<uint8_t> PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::isValueSet(const T& value)
{
    return this->parameters->template isTypeAndValueIn<T>(value);
}

template <EnumType StatusCode, std::size_t  MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
template <typename T>
inline constexpr bool PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::isValueSetBool(const T &value)
{
    if (isValueSet<T>(value).size())
        return true;
    return false;
}

template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
template <ParametersLabels Parameter>
constexpr bool PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::invokeConfigFunction()
{
    auto && arg = this->parameters->template getMemberValue<Parameter>();
    return this->functions->template getMemberValue<Parameter>()(arg, this);
}

template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
template <std::size_t I>
constexpr bool PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::invokeConfigFunction()
{
    constexpr ParametersLabels functionIndex = static_cast<ParametersLabels>(I);
    auto && arg = this->parameters->template getMemberValue<functionIndex>();
    return this->functions->template getMemberValue<functionIndex>()(arg, this);
}

template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
constexpr std::array<bool, sizeof...(Parameters)> PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::invokeAllConfigFunctions()
{
    std::array<bool, numberOfProperties> results;
    invokeAllConfigFunctionsHelper<0>(results);
    return results;
}

template <EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
template <std::size_t I>
constexpr bool PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::invokeAllConfigFunctionsHelper(std::array<bool, sizeof...(Parameters)>& v)
{
    if constexpr(I < numberOfProperties)
    {
        v[I] = invokeConfigFunction<I>();
        return invokeAllConfigFunctionsHelper<I+1>(v);
    }
    return true;
}

template<EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
bool PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>::isResetOrReady()
{
    if(this->status == StatusCode::Ready || this->status == StatusCode::Reset)
        return true;
    return false;    
}

#endif // __PERIPHERALBASE_H__
