#ifndef __CONTAINERBASE_H__
#define __CONTAINERBASE_H__

#include <GeneralConcepts.hh>
#include <tuple>
#include <type_traits>
#include <concepts>
#include <stdint.h>
#include <functional>

/** @brief Base class for a container that stores different types of data
 *  and offers several different methods to interact withn the items in the container
*   @tparam Members...: Container's items' data types 
*/
template <typename... Members>
class ContainerBase
{


    protected:

        constexpr explicit ContainerBase();
        constexpr explicit ContainerBase(Members&&... members); 
        constexpr explicit ContainerBase(const Members&... members);
        constexpr virtual ~ContainerBase();
        constexpr ContainerBase(const ContainerBase<Members...>& other);
        constexpr ContainerBase(ContainerBase<Members...>&& other);
        ContainerBase<Members...>& operator=(const ContainerBase<Members...>&);
        ContainerBase<Members...>& operator=(ContainerBase<Members...>&&);

        template <typename Action, typename... ActionArgs>
        constexpr void applyToAll(const Action& action, ActionArgs&&... arguments);

        template <typename Action, typename... ActionArgs>
        constexpr void applyToEach(const Action& action, ActionArgs&&... arguments);

        template <std::size_t I = 0, typename... Args>
        constexpr void setAllMemberValues(std::tuple<Args...>&& argsTuple);

        template <std::size_t I = 0, std::size_t J = 0, typename... Args>
        constexpr void setMembersValues(std::tuple<Args...>&& argsTuple);

        template <std::size_t Index, typename T>
        void setMemberValue(T&& param);

        template <std::size_t Index>
        auto getMemberValue();

        template<std::size_t I = 0, typename Arg>
        constexpr void isTypeIn(std::vector<uint8_t>& vect);

        template<std::size_t I = 0, typename Arg>
        constexpr void isTypeAndValueIn(const Arg& arg, std::vector<uint8_t>& vect);

        /**
        * @brief Pure virtual function to get the size of the container.
        * @return The size of the container.
        */
        constexpr virtual uint8_t getSize() = 0;

        /**
         * @brief Tuple to store the members.
         */
        std::tuple<Members...> members;

    private:

        template <typename F, typename Tuple, std::size_t... I>
        constexpr void apply_and_fold(F&& f, Tuple&& t, std::index_sequence<I...>);

        template <typename F, typename Tuple>
        constexpr void apply_and_fold(F&& f, Tuple&& t);
        
        template <typename F, typename Tuple1, typename Tuple2, std::size_t... I>
        constexpr void apply_and_fold_d(F&& f, Tuple1&& t1, Tuple2&& t2, std::index_sequence<I...>) ;

        template <typename F, typename Tuple1, typename Tuple2>
        constexpr void apply_and_fold_d(F&& f, Tuple1&& t1, Tuple2&& t2);

        static constexpr bool copyTupleContents(ContainerBase<Members...>&& a, ContainerBase<Members...>&& b);
        template<std::size_t I>
        static constexpr bool copyTupleContents(ContainerBase<Members...>&& a, ContainerBase<Members...>&& b);
};


/**
* @brief Default constructor.
* @tparam Members...: Container's items' data types 
*/
template <typename... Members>
constexpr ContainerBase<Members...>::ContainerBase()
{

}

/**
 * @brief Constructor that initializes the members with rvalue references.
 * @tparam Members...: Container's items' data types 
 * @param members Rvalue references to initialize the members.
 */
template <typename... Members>
constexpr ContainerBase<Members...>::ContainerBase(Members&&... members)
    : members(std::forward<Members>(members)...)
{

}

/**
 * @brief Constructor that initializes the members with lvalue references.
 * @tparam Members...: Container's items' data types 
 * @param members Lvalue references to initialize the members.
 */
template <typename... Members>
constexpr ContainerBase<Members...>::ContainerBase(const Members&... members)
    : members(members...)
{

}

/**
 * @brief Virtual destructor.
 */
template <typename... Members>
constexpr ContainerBase<Members...>::~ContainerBase()
{

}

/**
 * @brief Copy constructor. Takes other by const ref
 * @param other A container from which members want to be copied
*/
template <typename... Members>
constexpr ContainerBase<Members...>::ContainerBase(const ContainerBase<Members...> &other)
    : members(other.members)
{

}

/**
 * @brief Move constructor. Takes other by universal ref
 * @param other A container from which members want to be moved
*/
template <typename... Members>
constexpr ContainerBase<Members...>::ContainerBase(ContainerBase<Members...> &&other)
    : members(std::forward<ContainerBase<Members...>>(other.members))
{

}


/**
 * @brief Assigment operator. Copies other's members on this.
 * @param other A container from which members want to be copied
*/
template <typename... Members>
ContainerBase<Members...>& ContainerBase<Members...>::operator=(const ContainerBase<Members...>& other)
{
    ContainerBase<Members...>::copyTupleContents<0>(*this, other);
    return *this;
}

/**
 * @brief Assigment operator. Moves other's members to this.
 * @param other A container from which members want to be moved 
*/
template <typename... Members>
ContainerBase<Members...>& ContainerBase<Members...>::operator=(ContainerBase<Members...>&& other)
{
    ContainerBase<Members...>::copyTupleContents<0>(*this, std::forward<ContainerBase<Members...>>(other));
    return *this;
}

/**
 * @brief Method to copy the members of tuple b into tuple a.
 * @tparam I Index of the tuples
 * @param a Tuple receiving copied elements from b
 * @param b Tuple from which the elements are being copied
*/
template <typename... Members>
template<std::size_t I>
constexpr bool ContainerBase<Members...>::copyTupleContents(ContainerBase<Members...> &&a, ContainerBase<Members...> &&b)
{
    if constexpr(I < sizeof...(Members))
    {
        auto&& bValue = b.getMemberValue<I>();
        a.setMemberValue<I, decltype(bValue)>(bValue);
    }
    return true;
}

/**
 * @brief Implementation of the copyTupleElements method to copy the members of tuple b into tuple a.
 * @param a Tuple receiving copied elements from b
 * @param b Tuple from which the elements are being copied
*/
template <typename... Members>
constexpr bool ContainerBase<Members...>::copyTupleContents(ContainerBase<Members...> &&a, ContainerBase<Members...> &&b)
{
    return copyTupleContents<0>(std::forward<ContainerBase<Members...>>(a), std::forward<ContainerBase<Members...>>(b));
}


/**
 * @brief Applies a function, one member at time, with the same arguments(the "arguments" paramenter), to all members in the container.
 * @tparam Action The type of the function to apply.
 * @tparam ActionArgs The types of the arguments that the action function takes.
 * @param action The function to apply. Its first argument its a member of the container so its type should be auto&&
 * @param arguments The arguments to the function.
 */
template <typename... Members>
template <typename Action, typename... ActionArgs>
constexpr void  ContainerBase<Members...>::applyToAll(const Action& action, ActionArgs&&... arguments)
{
    apply_and_fold([&action, &arguments...](auto&& member)
        {
            action(member, std::forward<ActionArgs>(arguments)...); 
        }, 
        members);
}

/**
 * @brief Applies a function, one member at time, one argument for each member, to all of the members in the container
 * he first is a member of the container, the second argument is anything, one member at time, to all of the members in the container. 
 * @tparam Action The type of the function to apply.
 * @tparam ActionArgs The types of the arguments to the function. Must match the number of elements in the container
 * @param action The function to apply.
 * @param arguments The arguments to the function.
 */
template<typename... Members>
template <typename Action, typename... ActionArgs>
constexpr void ContainerBase<Members...>::applyToEach(const Action& action, ActionArgs&&... arguments)
{
    std::tuple<ActionArgs&&...> argsTuple{ std::forward<ActionArgs>(arguments)... };
    apply_and_fold_d([&](auto&& member, auto&& arg)
        {
            action(std::forward<decltype(member)>(member), std::forward<decltype(arg)>(arg));
        },
        members, argsTuple);
}

/**
 * @brief Sets the values of all members of the container.
 * 
 * @tparam I The index of the member to set.
 * @tparam Args The types of the values to set.
 * @param argsTuple A tuple containing the values to set.
 */
template<typename... Members>
template <std::size_t I, typename... Args>
constexpr void ContainerBase<Members...>::setAllMemberValues(std::tuple<Args...>&& argsTuple)
{
    if constexpr (I < sizeof...(Members))
    {
        std::get<I>(members) = std::get<I>(std::forward<std::tuple<Args...>>(argsTuple));
        setAllMemberValues<I + 1, Args...>(std::forward<std::tuple<Args...>>(argsTuple));
    }
}

/**
 * @brief Sets the values of specific members of the container.
 * 
 * @tparam I The index of the member to set.
 * @tparam J The index of the value to set.
 * @tparam Args The types of the values to set.
 * @param argsTuple A tuple containing the values to set.
 */
template<typename... Members>
template <std::size_t I, std::size_t J, typename... Args>
constexpr void ContainerBase<Members...>::setMembersValues(std::tuple<Args...>&& argsTuple)
{
    if constexpr (I < sizeof...(Members))
    {
        auto&& member = std::get<I>(std::forward<std::tuple<Members...>>(members));

        if constexpr (J < sizeof...(Args))
        {
            auto&& arg = std::get<J>(std::forward<std::tuple<Args...>>(argsTuple));

            using MemberType = std::decay_t<decltype(member)>;
            using ArgType = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<MemberType, ArgType>)
            {
                member = arg;
            }
            setMembersValues<I, J + 1>(std::forward<std::tuple<Args...>>(argsTuple));  // Move to the next element in argsTuple
        }
        setMembersValues<I + 1, 0>(std::forward<std::tuple<Args...>>(argsTuple));  // Move to the next member in members tuple
    }
}

template <typename... Members>
template <std::size_t Index>
auto ContainerBase<Members...>::getMemberValue()
{
    return std::get<Index>(this->members);
}

template <typename... Members>
template <std::size_t Index, typename T>
void ContainerBase<Members...>::setMemberValue(T &&param)
{
    std::get<Index>(this->members) = std::forward<T>(param);
}

/**
 * @brief Checks if a type is in the container.
 * 
 * @tparam I The index to start the search from.
 * @tparam Arg The type to search for.
 * @param vect A vector to store the indices where the type is found.
 */
template<typename... Members>
template<std::size_t I, typename Arg>
constexpr void ContainerBase<Members...>::isTypeIn(std::vector<uint8_t>& vect)
{
    if constexpr(I < sizeof...(Members))            
    {
        //auto&& member = std::get<I>(std::move(members));
        auto&& member = std::get<I>(members);
        using MemberType = std::decay_t<decltype(member)>;
        if constexpr (std::is_same_v<MemberType, Arg>)
            vect.push_back(static_cast<uint8_t>(I));
        isTypeIn<I+1, Arg>(vect); 
    }
}



/**
 * @brief Checks if a type and value is in the container.
 * 
 * @tparam I The index to start the search from.
 * @tparam Arg The type and value to search for.
 * @param arg The value to search for.
 * @param vect A vector to store the indices where the type and value is found.
 */
template<typename... Members>
template<std::size_t I, typename Arg>
constexpr void ContainerBase<Members...>::isTypeAndValueIn(const Arg& arg, std::vector<uint8_t>& vect)
{
    if constexpr(I < sizeof...(Members))            
    {
        //auto&& member = std::get<I>(std::move(members));
        auto&& member = std::get<I>(members);
        using MemberType = std::decay_t<decltype(member)>;
        using ArgType = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<MemberType, ArgType>)
        {
            if (arg == member)
            {
                vect.push_back(static_cast<uint8_t>(I));
            }
        }
        isTypeAndValueIn<I+1, Arg>(arg, vect); 
    }
}

/**
* @brief Applies a function to each member of a tuple.
* 
* This method uses a fold expression to apply a function to each member of a tuple. The function is applied in a left-to-right order.
* 
* @tparam F The type of the function to apply.
* @tparam Tuple The type of the tuple.
* @tparam I The indices of the tuple elements.
* @param f The function to apply.
* @param t The tuple to apply the function to.
*/
template<typename... Members>
template <typename F, typename Tuple, std::size_t... I>
constexpr void ContainerBase<Members...>::apply_and_fold(F&& f, Tuple&& t, std::index_sequence<I...>) 
{
    (f(std::get<I>(std::forward<Tuple>(t))), ...);
}

/**
* @brief Applies a function to each member of a tuple.
* 
* This method uses a fold expression to apply a function to each member of a tuple. The function is applied in a left-to-right order.
* 
* @tparam F The type of the function to apply.
* @tparam Tuple The type of the tuple.
* @param f The function to apply.
* @param t The tuple to apply the function to.
*/
template<typename... Members>
template <typename F, typename Tuple>
constexpr void ContainerBase<Members...>::apply_and_fold(F&& f, Tuple&& t) 
{
    apply_and_fold(std::forward<F>(f), std::forward<Tuple>(t),
        std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>());
}

/**
* @brief Applies a function to each corresponding pair of members from two tuples.
* 
* This method uses a fold expression to apply a function to each corresponding pair of members from two tuples. The function is applied in a left-to-right order.
* 
* @tparam F The type of the function to apply.
* @tparam Tuple1 The type of the first tuple.
* @tparam Tuple2 The type of the second tuple.
* @tparam I The indices of the tuple elements.
* @param f The function to apply.
* @param t1 The first tuple to apply the function to.
* @param t2 The second tuple to apply the function to.
*/
template<typename... Members>
template <typename F, typename Tuple1, typename Tuple2, std::size_t... I>
constexpr void ContainerBase<Members...>::apply_and_fold_d(F&& f, Tuple1&& t1, Tuple2&& t2, std::index_sequence<I...>) 
{
    (f(std::get<I>(std::forward<Tuple1>(t1)), std::get<I>(std::forward<Tuple2>(t2))), ...);
}

/**
* @brief Applies a function to each corresponding pair of members from two tuples.
* 
* This method uses a fold expression to apply a function to each corresponding pair of members from two tuples. The function is applied in a left-to-right order.
* 
* @tparam F The type of the function to apply.
* @tparam Tuple1 The type of the first tuple.
* @tparam Tuple2 The type of the second tuple.
* @param f The function to apply.
* @param t1 The first tuple to apply the function to.
* @param t2 The second tuple to apply the function to.
*/
template<typename... Members>
template <typename F, typename Tuple1, typename Tuple2>
constexpr void ContainerBase<Members...>::apply_and_fold_d(F&& f, Tuple1&& t1, Tuple2&& t2) 
{
    apply_and_fold_d(std::forward<F>(f), std::forward<Tuple1>(t1), std::forward<Tuple2>(t2),
        std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple1>>>());
}



#endif // __CONTAINERBASE_H__
