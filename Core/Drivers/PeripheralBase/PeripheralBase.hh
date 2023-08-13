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
 * @tparam StatusCode is an enum class listing all the possible errors/malfunctioning (hardware related) for that particular peripheral
 * @tparam Parameter is an object of the type "types"
 */


template<
    typename StatusCode,
    template<typename, typename...>class MembersContainer,
    typename ParametersLabels, 
    typename Parameters, 
    typename Functions, 
    typename NumberOfMandatoryParameters
    >
class PeripheralBase; 

template<
    EnumType StatusCode, 
    template<EnumType, typename...>class MembersContainer,
    EnumType ParameterLabels, 
    typename... Parameters, 
    typename... Functions, 
    std::size_t NumberOfMandatoryParameters
    >
class PeripheralBase<
    StatusCode, 
    MembersContainer, 
    ParameterLabels, 
    MembersContainer<ParameterLabels, Parameters...>, 
    MembersContainer<ParameterLabels, Functions...>, 
    std::array<uint8_t, NumberOfMandatoryParameters>
    >
{
    public:
        //<----------------------------------------------Public constructors---------------------------------------------->//
        virtual ~PeripheralBase();
        //<---------------------------------------------------------------------------------------------------------->//

    protected:

        using PeripheralBaseType = PeripheralBase<StatusCode, MembersContainer, ParameterLabels, MembersContainer<ParameterLabels, Parameters...>, MembersContainer<ParameterLabels, Functions...>, std::array<uint8_t, NumberOfMandatoryParameters> >;
        using FunctionsContainerType = MembersContainer<ParameterLabels, StatusCode(*)(const Parameters&, PeripheralBaseType*) ...>;
        using ConfigFunctionsResponseType = std::array<StatusCode, sizeof...(Parameters)>;

        //<---------------------------------------------- constructors---------------------------------------------->//
        constexpr explicit PeripheralBase();
        constexpr explicit PeripheralBase(const Parameters&... initialValues);

        constexpr explicit PeripheralBase(const PeripheralBase& other);
        constexpr explicit PeripheralBase(PeripheralBase &&other);

        //<---------------------------------------------- Getter methods ---------------------------------------------->//
        bool isResetOrReady();
        StatusCode getStatus();

        template<ParameterLabels Parameter>
        auto&& getParameterValue();
        //<---------------------------------------------------------------------------------------------------------->//


        //<---------------------------------------------- Setter methods ---------------------------------------------->//
        void setStatus(const StatusCode& status);

        void setAllParametersValue(Parameters&&... parameters);

        template<ParameterLabels Param, typename Value>
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
        ConfigFunctionsResponseType init();
        //template<std::size_t

        //<---------------------------------------------------------------------------------------------------------->//

        //Children-accessible class members
        void setParameterAsMandatory(const ParameterLabels& parameter);

    private:

        //<---------------------------------------------------------------------------------------------------------->//

        template<std::size_t I = 0>
        constexpr StatusCode invokeAllConfigFunctionsHelper(std::array<StatusCode, sizeof...(Parameters)>& v);

        template<std::size_t I>
        constexpr StatusCode invokeConfigFunction();

        template<ParameterLabels Parameter>
        constexpr StatusCode invokeConfigFunction();

        constexpr ConfigFunctionsResponseType invokeAllConfigFunctions();
        //<---------------------------------------------------------------------------------------------------------->//

        //<-------------------------------------------- Pure virtual methods --------------------------------------->//
        //virtual void setDefaultSettings() = 0;
        //<---------------------------------------------------------------------------------------------------------->//

        static const constexpr std::size_t numberOfProperties { sizeof...(Parameters) };
        static StaticManager<PeripheralBaseType> staticManager;
        constexpr static FunctionsContainerType *const functions { nullptr };

        StatusCode status { StatusCode::Reset };
        MembersContainer<ParameterLabels, Parameters...>*const parameters { nullptr };
};
//PeripheralBase<StatusCode, MembersContainer, ParameterLabels, Container<ParameterLabels, Parameters...>, Container<ParameterLabels, Functions...>, std::array<uint8_t, NumberOfMandatoryParameters> >
template<EnumType StatusCode, template<EnumType, typename...>class MembersContainer, EnumType ParameterLabels, typename... Parameters, typename... Functions, std::size_t NumberOfMandatoryParameters>
StaticManager<PeripheralBase<StatusCode, MembersContainer, ParameterLabels, MembersContainer<ParameterLabels, Parameters...>, MembersContainer<ParameterLabels, Functions...>, std::array<uint8_t, NumberOfMandatoryParameters> >> PeripheralBase<StatusCode, MembersContainer, ParameterLabels, MembersContainer<ParameterLabels, Parameters...>, MembersContainer<ParameterLabels, Functions...>, std::array<uint8_t, NumberOfMandatoryParameters>>::staticManager{0};

//template<EnumType StatusCode, template<EnumType, typename...>class MembersContainer, EnumType ParameterLabels, typename... Parameters, typename... Functions, std::size_t NumberOfMandatoryParameters>
//MembersContainer<ParameterLabels, StatusCode(*)(const Parameters&, PeripheralBase<StatusCode, MembersContainer, ParameterLabels, MembersContainer<ParameterLabels, Parameters...>, MembersContainer<ParameterLabels, Functions...>, std::array<uint8_t, NumberOfMandatoryParameters> >*) ...> PeripheralBase<StatusCode, MembersContainer, ParameterLabels, MembersContainer<ParameterLabels, Parameters...>, MembersContainer<ParameterLabels, Functions...>, std::array<uint8_t, NumberOfMandatoryParameters>>::

template<EnumType StatusCode, template<EnumType, typename...>class MembersContainer, EnumType ParameterLabels, typename... Parameters, typename... Functions, std::size_t NumberOfMandatoryParameters>
constexpr PeripheralBase<StatusCode, MembersContainer, ParameterLabels, MembersContainer<ParameterLabels, Parameters...>, MembersContainer<ParameterLabels, Functions...>, std::array<uint8_t, NumberOfMandatoryParameters>>::PeripheralBase()
{
    if constexpr (this->functions==nullptr)
        this = new MembersContainer<ParameterLabels, Parameters...>();    
}


//template<EnumType StatusCode, template<EnumType, typename...>class MembersContainer, EnumType ParameterLabels, typename... Parameters, typename... Functions, std::size_t NumberOfMandatoryParameters>
//constexpr PeripheralBase<StatusCode, MembersContainer, ParameterLabels, MembersContainer<ParameterLabels, Parameters...>, MembersContainer<ParameterLabels, Functions...>, std::array<uint8_t, NumberOfMandatoryParameters>>::PeripheralBase()




#endif // __PERIPHERALBASE_H__
