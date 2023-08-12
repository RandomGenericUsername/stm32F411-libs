#include <stdint.h>

class Utils
{
    private:

        template<std::size_t CurrentIndex, std::size_t DecimalNumber>
        constexpr std::size_t getMaxIndex()
        {
            constexpr std::size_t bitPosition{0x1U << CurrentIndex} ;
            if constexpr(DecimalNumber / bitPosition == 1)
                return CurrentIndex;
            else if constexpr(DecimalNumber / bitPosition < 1)
                return -1;
            return getMaxIndex<CurrentIndex + 1, DecimalNumber>();
        }

    public:

        template<std::size_t CurrentIndex, std::size_t BytesRepresentation, template<typename ...>typename Action, typename... Args>
        static auto fun(const Action<Args...>& action, Args&&... args)
        {  
            if constexpr( (0x1 << CurrentIndex) & BytesRepresentation)
            {
                action(args...);
            }
        };

        template<std::size_t CurrentIndex, const std::size_t MaxIndex, std::size_t BytesRepresentation, template<typename ...>typename Action, typename... Args>
        static auto fun(const Action<Args...>& action, Args&&... args)
        {
            if constexpr(CurrentIndex < MaxIndex)
            {
                fun<CurrentIndex + 1, MaxIndex, BytesRepresentation, Action<Args...>, Args...>(action, args...);
            }
        }

        template<std::size_t MaxIndex, std::size_t BytesRepresentation, template<typename ...>typename Action, typename... Args>
        static auto funn(const Action<Args...>& action, Args&&... args)
        {
            fun<0, MaxIndex, BytesRepresentation, Action<Args...>, Args...>(action, args...);
        }


        template<std::size_t DecimalNumber>
        constexpr std::size_t getMaxIndex()
        {
            return getMaxIndex<0, DecimalNumber>();
        }
};