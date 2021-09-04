#pragma once

#include <type_traits>

template<typename EnumBitmask>
constexpr EnumBitmask operator|(EnumBitmask lhs, EnumBitmask rhs)
{
    static_assert(std::is_enum<EnumBitmask>::value, "arguments must be of an enum type");

    return static_cast<EnumBitmask>(static_cast<typename std::underlying_type<EnumBitmask>::type>(lhs) | static_cast<typename std::underlying_type<EnumBitmask>::type>(rhs));
}

template<typename EnumBitmask>
constexpr EnumBitmask operator&(EnumBitmask lhs, EnumBitmask rhs)
{
    static_assert(std::is_enum<EnumBitmask>::value, "arguments must be of an enum type");

    return static_cast<EnumBitmask>(static_cast<typename std::underlying_type<EnumBitmask>::type>(lhs) & static_cast<typename std::underlying_type<EnumBitmask>::type>(rhs));
}
