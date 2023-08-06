#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <ContainerBase.hh>

/**
 * @brief Container class to store several different types and perform single, focused and iterative operations on all the members
 * 
 * @tparam Parameters: is an enum class that contains the labels for all the members in the container, plus, one extra mandatory member __length. If this member is not in the enum, compiling this class will fail 
 * @tparam Members: is a argument pack of all the instantations to be stored in the container 
 */
template <EnumType Parameters, typename... Members>
class Container : public ContainerBase<Members...>
{
    public:

        constexpr explicit Container();
        constexpr explicit Container(Members&&... members);
        constexpr explicit Container(const Members&... members);
        virtual ~Container() override;
        constexpr explicit Container(Container<Parameters, Members...>&& other);
        constexpr explicit Container(const Container<Parameters, Members...>& other);
        Container<Parameters, Members...>& operator=(const Container<Parameters, Members...>& other);
        Container<Parameters, Members...>& operator=(Container<Parameters, Members...>&& other);

        template <typename Action, typename... ActionArgs>
        constexpr void applyToAll(const Action& action, ActionArgs&&... actionArguments);

        template <typename Action, typename... ActionArgs>
        constexpr void applyToEach(const Action& action, ActionArgs&&... arguments);

        template <typename... Args>
        constexpr void setAllMemberValues(Args&&... args);

        template <typename... Args>
        constexpr void setMembersValues(Args&&... args);

        template <Parameters Index, typename T>
        void setMemberValue(T&& param);

        template <Parameters Index>
        auto getMemberValue();

        template <typename Type>
        auto isTypeIn();

        //<-----------------------------------------------------------------------------------------------------||----------------------------------------------------------------------------------------------------->
        template<typename Type>
        constexpr std::vector<uint8_t> isTypeAndValueIn(const Type& arg);
        

        constexpr uint8_t getSize() override;
        //    return sizeof...(Members);

    private:

        static constexpr uint32_t numberOfElements{sizeof...(Members)};

        static_assert(
            sizeof...(Members) == static_cast<std::size_t>(Parameters::__length),
            "[number of parameters in the enum doesn't match the class's arguments in the constructor]"
        );
};


/** @brief Constructor requiring only the types to be stored, so no particular instance of an object has to be passed at the momment of instantation of the class*/
template <EnumType Parameters, typename... Members>
constexpr Container<Parameters, Members...>::Container()
    : ContainerBase<Members...>()
{

}

/** 
 * @brief Constructor requiring objects' particular instances to instantiate this class
 * @param members Variadic arguments - Objects of any type. Arguments are taken as universal references(&&)
 */
template <EnumType Parameters, typename... Members>
constexpr Container<Parameters, Members...>::Container(Members &&...members)
    : ContainerBase<Members...>(std::forward<Members>(members)...)
{

}

/** @brief Constructor requiring objects' particular instances to instantiate this class
 * @param members Variadic arguments - Objects of any type. Arguments are taken as const references(const&)
*/
template <EnumType Parameters, typename... Members>
constexpr Container<Parameters, Members...>::Container(const Members &...members)
    : ContainerBase<Members...>(members...)
{

}

/** @brief Virtual destructor*/
template <EnumType Parameters, typename... Members>
Container<Parameters, Members...>::~Container()
{
    
}

template <EnumType Parameters, typename... Members>
constexpr Container<Parameters, Members...>::Container(Container<Parameters, Members...> &&other)
    : ContainerBase<Parameters, Members...>(std::move(other))
{

}

template <EnumType Parameters, typename... Members>
constexpr Container<Parameters, Members...>::Container(const Container<Parameters, Members...> &other)
    : ContainerBase<Parameters, Members...>(other)
{

}

template <EnumType Parameters, typename... Members>
Container<Parameters, Members...> &Container<Parameters, Members...>::operator=(const Container<Parameters, Members...> &other)
{
    this->members = other.members;
    return *this;
}

template <EnumType Parameters, typename... Members>
Container<Parameters, Members...> &Container<Parameters, Members...>::operator=(Container<Parameters, Members...>&& other)
{
    this->members = std::forward<Container<Parameters, Members...>>(other);
    return *this;
}

/**
 * @brief Method to apply a function iteratively to all the members in the container. action(containerMember_i, argument1, argument2, ...., argumentn)
 * 
 * @param action Callable. A function taking any number of arguments. Each member on the container will be invoked as the first argument to this function followed by the arguments passed in "actionArguments" argument. auto&& is advised to be first argument to this function in order to match any member in the container
 * @param actionArguments The arguments with which the callable will be invoked along with the container member 
*/
template <EnumType Parameters, typename... Members>
constexpr uint8_t Container<Parameters, Members...>::getSize()
{
    return sizeof...(Members);
}

template <EnumType Parameters, typename... Members>
template <typename Action, typename... ActionArgs>
constexpr void Container<Parameters, Members...>::applyToAll(const Action &action, ActionArgs &&...actionArguments)
{
    ContainerBase<Members...>::template applyToAll(action, std::forward<ActionArgs>(actionArguments)...);
}

/**
 * @brief Method to apply a function iteratively to all the members in the container. action(containerMember_i, argument)
 * 
 * @param action Callable. A function to be applied to all the members in the container, taking only two arguments: The first argument is a member of the container, so auto&& is advised. The second argument can be anything.
 * @param arguments Variadic argument that match each of the members in the container. 
*/
template <EnumType Parameters, typename... Members>
template <typename Action, typename... ActionArgs>
constexpr void Container<Parameters, Members...>::applyToEach(const Action &action, ActionArgs &&...arguments)
{
    static_assert(sizeof...(Members) == sizeof...(arguments), "Number of arguments must match the number of elements in the container");
    ContainerBase<Members...>::template applyToEach(action, std::forward<ActionArgs>(arguments)...);
}

template <EnumType Parameters, typename... Members>
template <typename... Args>
constexpr void Container<Parameters, Members...>::setAllMemberValues(Args &&...args)
{
    static_assert((sizeof...(Args) == sizeof...(Members)),
                "The number of parameters should be equal to the number of members in the container");

    std::tuple<Args...> argsTuple{ std::forward<Args>(args)... };
    ContainerBase<Members...>::template setAllMemberValues<0>(std::forward<decltype(argsTuple)>(argsTuple));
}

template <EnumType Parameters, typename... Members>
template <typename... Args>
constexpr void Container<Parameters, Members...>::setMembersValues(Args &&...args)
{
    static_assert(sizeof...(args) <= sizeof...(Members), "out of bounds");
    std::tuple<Args...> argsTuple{ std::forward<Args>(args)... };
    ContainerBase<Members...>::template setMembersValues<0,0>(std::forward<decltype(argsTuple)>(argsTuple));
}

template <EnumType Parameters, typename... Members>
template <Parameters Index, typename T>
void Container<Parameters, Members...>::setMemberValue(T &&param)
{
    static_assert(Index < Parameters::__length, "out of bounds");
    constexpr std::size_t index = static_cast<std::size_t>(Index);
    ContainerBase<Members...>::template setMemberValue<index, T>(std::forward<T>(param));
}


template <EnumType Parameters, typename... Members>
template <Parameters Index>
auto Container<Parameters, Members...>::getMemberValue()
{
    static_assert(Index < Parameters::__length, "out of bounds");
    constexpr std::size_t index = static_cast<std::size_t>(Index);
    return ContainerBase<Members...>::template getMemberValue<index>();
}

template <EnumType Parameters, typename... Members>
template <typename Type>
auto Container<Parameters, Members...>::isTypeIn()
{
    std::vector<uint8_t> result;
    ContainerBase<Members...>::template isTypeIn<0, Type>(result);
    return result;
}

template <EnumType Parameters, typename... Members>
template <typename Type>
constexpr std::vector<uint8_t> Container<Parameters, Members...>::isTypeAndValueIn(const Type &arg)
{
    std::vector<uint8_t> result;
    ContainerBase<Members...>::template isTypeAndValueIn<0, Type>(arg, result);
    return result;
}

#endif // __CONTAINER_H__
