/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <cstddef>

#include <stdexcept>
#include <string_view>

#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

namespace enum_hpp
{
    class exception final : public std::runtime_error {
    public:
        explicit exception(const char* what)
        : std::runtime_error(what) {}
    };
}

namespace enum_hpp::detail
{
    template < typename Enum >
    struct ignore_assign {
        Enum value;

        constexpr explicit ignore_assign(Enum value) noexcept
        : value(value) {}

        template < typename Other >
        constexpr const ignore_assign& operator=(Other) const noexcept {
            return *this;
        }
    };

    constexpr bool is_end_of_name(char ch) noexcept {
        switch ( ch ) {
            case ' ':
            case '=':
            case '\r':
            case '\n':
            case '\t':
                return true;
            default:
                return false;
        }
    }

    constexpr std::string_view trim_raw_name(std::string_view raw_name) noexcept {
        for ( std::size_t i = 0; i < raw_name.size(); ++i ) {
            if ( is_end_of_name(raw_name[i]) ) {
                return raw_name.substr(0, i);
            }
        }
        return raw_name;
    }
}

//
// ENUM_HPP_GENERATE_ENUM_FIELDS
//

#define ENUM_HPP_GENERATE_ENUM_FIELDS_OP(r, d, x)\
    x,

#define ENUM_HPP_GENERATE_ENUM_FIELDS(Fields)\
    BOOST_PP_SEQ_FOR_EACH(ENUM_HPP_GENERATE_ENUM_FIELDS_OP, _, Fields)

//
// ENUM_HPP_GENERATE_VALUES
//

#define ENUM_HPP_GENERATE_VALUES_OP(r, Enum, x)\
    ((::enum_hpp::detail::ignore_assign<Enum>)Enum::x).value,

#define ENUM_HPP_GENERATE_VALUES(Enum, Fields)\
    BOOST_PP_SEQ_FOR_EACH(ENUM_HPP_GENERATE_VALUES_OP, Enum, Fields)

//
// ENUM_HPP_GENERATE_NAMES
//

#define ENUM_HPP_GENERATE_NAMES_OP(r, d, x)\
    ::enum_hpp::detail::trim_raw_name(ENUM_HPP_PP_STRINGIZE(x)),

#define ENUM_HPP_GENERATE_NAMES(Fields)\
    BOOST_PP_SEQ_FOR_EACH(ENUM_HPP_GENERATE_NAMES_OP, _, Fields)

//
// ENUM_HPP_ENUM_CLASS
//

#define ENUM_HPP_ENUM_CLASS(Enum, Type, Fields)\
    enum class Enum : Type {\
        ENUM_HPP_GENERATE_ENUM_FIELDS(Fields)\
    };\
    struct Enum##_traits {\
    private:\
        enum enum_names_for_this_score_ { ENUM_HPP_GENERATE_ENUM_FIELDS(Fields) };\
    public:\
        static constexpr std::size_t size = BOOST_PP_SEQ_SIZE(Fields);\
        static constexpr const Enum values[] = { ENUM_HPP_GENERATE_VALUES(Enum, Fields) };\
        static constexpr const std::string_view names[] = { ENUM_HPP_GENERATE_NAMES(Fields) };\
    public:\
        static constexpr std::string_view to_string(Enum e) noexcept {\
            for ( std::size_t i = 0; i < size; ++i) {\
                if ( e == values[i] ) {\
                    return names[i];\
                }\
            }\
            return std::string_view();\
        }\
        \
        static Enum from_string(std::string_view name){\
            for ( std::size_t i = 0; i < size; ++i) {\
                if ( name == names[i] ) {\
                    return values[i];\
                }\
            }\
            throw ::enum_hpp::exception(#Enum "_traits::from_string(): invalid argument");\
        }\
        \
        static constexpr bool from_string_nothrow(std::string_view name, Enum& result) noexcept {\
            for ( std::size_t i = 0; i < size; ++i) {\
                if ( name == names[i] ) {\
                    result = values[i];\
                    return true;\
                }\
            }\
            return false;\
        }\
    };

// -----------------------------------------------------------------------------
//
// ENUM_HPP_PP
//
// -----------------------------------------------------------------------------

#define ENUM_HPP_PP_STRINGIZE(x) ENUM_HPP_PP_STRINGIZE_I(x)
#define ENUM_HPP_PP_STRINGIZE_I(x) #x
