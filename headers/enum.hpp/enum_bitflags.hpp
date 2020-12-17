/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <type_traits>

namespace enum_hpp::bitflags
{
    template < typename Enum >
    class bitflags {
        static_assert(std::is_enum_v<Enum>);
    public:
        using enum_type = Enum;
        using underlying_type = std::underlying_type_t<Enum>;

        constexpr bitflags(enum_type flags)
        : flags_(static_cast<underlying_type>(flags)) {}

        constexpr explicit bitflags(underlying_type flags)
        : flags_(flags) {}

        constexpr explicit operator bool() const noexcept {
            return !!flags_;
        }

        constexpr underlying_type as_raw() const noexcept {
            return flags_;
        }

        constexpr enum_type as_enum() const noexcept {
            return static_cast<enum_type>(flags_);
        }
    private:
        underlying_type flags_{};
    };
}

namespace enum_hpp::bitflags
{
    #define ENUM_HPP_DEFINE_BINARY_OPERATOR(op)\
        template < typename Enum >\
        constexpr bool operator op(Enum l, bitflags<Enum> r) noexcept {\
            return l op r.as_raw();\
        }\
        template < typename Enum >\
        constexpr bool operator op(bitflags<Enum> l, Enum r) noexcept {\
            return l.as_raw() op r;\
        }\
        template < typename Enum >\
        constexpr bool operator op(std::underlying_type_t<Enum> l, bitflags<Enum> r) noexcept {\
            return l op r.as_raw();\
        }\
        template < typename Enum >\
        constexpr bool operator op(bitflags<Enum> l, std::underlying_type_t<Enum> r) noexcept {\
            return l.as_raw() op r;\
        }\
        template < typename Enum >\
        constexpr bool operator op(bitflags<Enum> l, bitflags<Enum> r) noexcept {\
            return l.as_raw() op r.as_raw();\
        }
        ENUM_HPP_DEFINE_BINARY_OPERATOR(<)
        ENUM_HPP_DEFINE_BINARY_OPERATOR(>)
        ENUM_HPP_DEFINE_BINARY_OPERATOR(<=)
        ENUM_HPP_DEFINE_BINARY_OPERATOR(>=)
        ENUM_HPP_DEFINE_BINARY_OPERATOR(==)
        ENUM_HPP_DEFINE_BINARY_OPERATOR(!=)
    #undef ENUM_HPP_DEFINE_BINARY_OPERATOR
}

namespace enum_hpp::bitflags
{
    template < typename Enum >
    constexpr bitflags<Enum> operator~(bitflags<Enum> l) noexcept {
        return static_cast<Enum>(~l.as_raw());
    }

    #define ENUM_HPP_DEFINE_BINARY_OPERATOR(op)\
        template < typename Enum >\
        constexpr bitflags<Enum> operator op (Enum l, bitflags<Enum> r) noexcept {\
            return bitflags{l} op r;\
        }\
        template < typename Enum >\
        constexpr bitflags<Enum> operator op (bitflags<Enum> l, Enum r) noexcept {\
            return l op bitflags{r};\
        }\
        template < typename Enum >\
        constexpr bitflags<Enum> operator op (bitflags<Enum> l, bitflags<Enum> r) noexcept {\
            return static_cast<Enum>(l.as_raw() op r.as_raw());\
        }\
        template < typename Enum >\
        constexpr bitflags<Enum>& operator op##= (bitflags<Enum>& l, Enum r) noexcept {\
            return l = l op bitflags{r};\
        }\
        template < typename Enum >\
        constexpr bitflags<Enum>& operator op##= (bitflags<Enum>& l, bitflags<Enum> r) noexcept {\
            return l = l op r;\
        }
        ENUM_HPP_DEFINE_BINARY_OPERATOR(|)
        ENUM_HPP_DEFINE_BINARY_OPERATOR(&)
        ENUM_HPP_DEFINE_BINARY_OPERATOR(^)
    #undef ENUM_HPP_DEFINE_BINARY_OPERATOR
}

namespace enum_hpp::bitflags
{
    //
    // any
    //

    template < typename Enum
             , std::enable_if_t<std::is_enum_v<Enum>, int> = 0 >
    constexpr bool any(Enum flags) noexcept {
        return any(bitflags{flags});
    }

    template < typename Enum >
    constexpr bool any(bitflags<Enum> flags) noexcept {
        return 0 != flags.as_raw();
    }

    //
    // none
    //

    template < typename Enum
             , std::enable_if_t<std::is_enum_v<Enum>, int> = 0 >
    constexpr bool none(Enum flags) noexcept {
        return none(bitflags{flags});
    }

    template < typename Enum >
    constexpr bool none(bitflags<Enum> flags) noexcept {
        return 0 == flags.as_raw();
    }

    //
    // all_of
    //

    template < typename Enum
             , std::enable_if_t<std::is_enum_v<Enum>, int> = 0 >
    constexpr bool all_of(Enum l, Enum r) noexcept {
        return all_of(bitflags{l}, bitflags{r});
    }

    template < typename Enum >
    constexpr bool all_of(Enum l, bitflags<Enum> r) noexcept {
        return all_of(bitflags{l}, r);
    }

    template < typename Enum >
    constexpr bool all_of(bitflags<Enum> l, Enum r) noexcept {
        return all_of(l, bitflags{r});
    }

    template < typename Enum >
    constexpr bool all_of(bitflags<Enum> flags, bitflags<Enum> mask) noexcept {
        return (flags.as_raw() & mask.as_raw()) == mask.as_raw();
    }

    //
    // any_of
    //

    template < typename Enum
             , std::enable_if_t<std::is_enum_v<Enum>, int> = 0 >
    constexpr bool any_of(Enum l, Enum r) noexcept {
        return any_of(bitflags{l}, bitflags{r});
    }

    template < typename Enum >
    constexpr bool any_of(Enum l, bitflags<Enum> r) noexcept {
        return any_of(bitflags{l}, r);
    }

    template < typename Enum >
    constexpr bool any_of(bitflags<Enum> l, Enum r) noexcept {
        return any_of(l, bitflags{r});
    }

    template < typename Enum >
    constexpr bool any_of(bitflags<Enum> l, bitflags<Enum> r) noexcept {
        return r.as_raw() == 0
            || (l.as_raw() & r.as_raw()) != 0;
    }

    //
    // none_of
    //

    template < typename Enum
             , std::enable_if_t<std::is_enum_v<Enum>, int> = 0 >
    constexpr bool none_of(Enum l, Enum r) noexcept {
        return none_of(bitflags{l}, bitflags{r});
    }

    template < typename Enum >
    constexpr bool none_of(Enum l, bitflags<Enum> r) noexcept {
        return none_of(bitflags{l}, r);
    }

    template < typename Enum >
    constexpr bool none_of(bitflags<Enum> l, Enum r) noexcept {
        return none_of(l, bitflags{r});
    }

    template < typename Enum >
    constexpr bool none_of(bitflags<Enum> l, bitflags<Enum> r) noexcept {
        return r.as_raw() != 0
            && (l.as_raw() & r.as_raw()) == 0;
    }

    //
    // any_except
    //

    template < typename Enum
             , std::enable_if_t<std::is_enum_v<Enum>, int> = 0 >
    constexpr bool any_except(Enum l, Enum r) noexcept {
        return any_except(bitflags{l}, bitflags{r});
    }

    template < typename Enum >
    constexpr bool any_except(Enum l, bitflags<Enum> r) noexcept {
        return any_except(bitflags{l}, r);
    }

    template < typename Enum >
    constexpr bool any_except(bitflags<Enum> l, Enum r) noexcept {
        return any_except(l, bitflags{r});
    }

    template < typename Enum >
    constexpr bool any_except(bitflags<Enum> l, bitflags<Enum> r) noexcept {
        return any_of(l, ~r);
    }

    //
    // none_except
    //

    template < typename Enum
             , std::enable_if_t<std::is_enum_v<Enum>, int> = 0 >
    constexpr bool none_except(Enum l, Enum r) noexcept {
        return none_except(bitflags{l}, bitflags{r});
    }

    template < typename Enum >
    constexpr bool none_except(Enum l, bitflags<Enum> r) noexcept {
        return none_except(bitflags{l}, r);
    }

    template < typename Enum >
    constexpr bool none_except(bitflags<Enum> l, Enum r) noexcept {
        return none_except(l, bitflags{r});
    }

    template < typename Enum >
    constexpr bool none_except(bitflags<Enum> l, bitflags<Enum> r) noexcept {
        return none_of(l, ~r);
    }
}

//
// ENUM_HPP_REGISTER_BITFLAGS_OPERATORS
//

#define ENUM_HPP_REGISTER_BITFLAGS_OPERATORS(Enum)\
    constexpr ::enum_hpp::bitflags::bitflags<Enum> operator~ [[maybe_unused]] (Enum l) noexcept {\
        return ~::enum_hpp::bitflags::bitflags(l);\
    }\
    constexpr ::enum_hpp::bitflags::bitflags<Enum> operator| [[maybe_unused]] (Enum l, Enum r) noexcept {\
        return ::enum_hpp::bitflags::bitflags(l) | ::enum_hpp::bitflags::bitflags(r);\
    }\
    constexpr ::enum_hpp::bitflags::bitflags<Enum> operator& [[maybe_unused]] (Enum l, Enum r) noexcept {\
        return ::enum_hpp::bitflags::bitflags(l) & ::enum_hpp::bitflags::bitflags(r);\
    }\
    constexpr ::enum_hpp::bitflags::bitflags<Enum> operator^ [[maybe_unused]] (Enum l, Enum r) noexcept {\
        return ::enum_hpp::bitflags::bitflags(l) ^ ::enum_hpp::bitflags::bitflags(r);\
    }
