#ifndef __S_T_M32_PERIPHERAL_BASE_HH_95TIBHUKGW20__
#define __S_T_M32_PERIPHERAL_BASE_HH_95TIBHUKGW20__

#include <system.h>
#include <PeripheralBase.hh>
#include <GeneralConcepts.hh>

#ifdef COMPILE 
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


template<STM32Peripheral Instance, typename BaseClass>
class STM32PeripheralBase;


template<STM32Peripheral  Instance, EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
class STM32PeripheralBase<Instance, PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>> : public PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>
{
    public:
        virtual ~STM32PeripheralBase();

    protected:

        using PeripheralBaseParentType = PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>;
        using STM32PeripheralBaseType = STM32PeripheralBase<Instance, PeripheralBaseParentType>;

        //<---------------------------------------------- constructors---------------------------------------------->//
        template<typename... Functions>
        constexpr explicit STM32PeripheralBase(const Container<ParametersLabels, Functions...>& functions);

        constexpr explicit STM32PeripheralBase(const STM32PeripheralBase& other);
        constexpr explicit STM32PeripheralBase(STM32PeripheralBase &&other);

        //Children-accessible class members
        Instance* instance { nullptr };

    private:

        void setInstancePtr(const Instance& peripheral);  

        virtual void setDefaultSettings() = 0;
};

template <STM32Peripheral Instance, EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
template <typename... Functions>
inline constexpr STM32PeripheralBase<Instance, PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>>::STM32PeripheralBase(const Container<ParametersLabels, Functions...> &functions)
    : PeripheralBaseParentType(functions)
{
}

template <STM32Peripheral Instance, EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
inline STM32PeripheralBase<Instance, PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>>::~STM32PeripheralBase()
{
    instance = nullptr;
}

template <STM32Peripheral Instance, EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
inline constexpr STM32PeripheralBase<Instance, PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>>::STM32PeripheralBase(const STM32PeripheralBase &other)
{
}

template <STM32Peripheral Instance, EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
inline constexpr STM32PeripheralBase<Instance, PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>>::STM32PeripheralBase(STM32PeripheralBase &&other)
{
}

template <STM32Peripheral Instance, EnumType StatusCode, std::size_t MandatoryParameters, EnumType ParametersLabels, typename... Parameters>
inline void STM32PeripheralBase<Instance, PeripheralBase<StatusCode, MandatoryParameters, Container<ParametersLabels, Parameters...>>>::setInstancePtr(const Instance &peripheral)
{
    constexpr auto instances = PeripheralMap<Instance>::instances();
    std::apply([this, &peripheral](auto&&... instance) {
        ((instance == peripheral ? this->instance = instance : false), ...);
    }, instances);
}

#endif

#endif  // __S_T_M32_PERIPHERAL_BASE_HH_95TIBHUKGW20__