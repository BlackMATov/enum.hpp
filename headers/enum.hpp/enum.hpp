/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <stdexcept>
#include <string_view>

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

#define ENUM_HPP_GENERATE_ENUM_FIELDS_OP(_, x)\
    x,

#define ENUM_HPP_GENERATE_ENUM_FIELDS(Fields)\
    ENUM_HPP_PP_SEQ_FOR_EACH(ENUM_HPP_GENERATE_ENUM_FIELDS_OP, _, Fields)

//
// ENUM_HPP_GENERATE_VALUES
//

#define ENUM_HPP_GENERATE_VALUES_OP(Enum, x)\
    ((::enum_hpp::detail::ignore_assign<Enum>)Enum::x).value,

#define ENUM_HPP_GENERATE_VALUES(Enum, Fields)\
    ENUM_HPP_PP_SEQ_FOR_EACH(ENUM_HPP_GENERATE_VALUES_OP, Enum, Fields)

//
// ENUM_HPP_GENERATE_NAMES
//

#define ENUM_HPP_GENERATE_NAMES_OP(_, x)\
    ::enum_hpp::detail::trim_raw_name(ENUM_HPP_PP_STRINGIZE(x)),

#define ENUM_HPP_GENERATE_NAMES(Fields)\
    ENUM_HPP_PP_SEQ_FOR_EACH(ENUM_HPP_GENERATE_NAMES_OP, _, Fields)

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
        static constexpr std::size_t size = ENUM_HPP_PP_SEQ_SIZE(Fields);\
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

//
// ENUM_HPP_PP_CAT
//

#define ENUM_HPP_PP_CAT(x, y) ENUM_HPP_PP_CAT_I(x, y)
#define ENUM_HPP_PP_CAT_I(x, y) x ## y

//
// ENUM_HPP_PP_STRINGIZE
//

#define ENUM_HPP_PP_STRINGIZE(x) ENUM_HPP_PP_STRINGIZE_I(x)
#define ENUM_HPP_PP_STRINGIZE_I(x) #x

//
// ENUM_HPP_PP_SEQ_HEAD
//

#define ENUM_HPP_PP_SEQ_HEAD(seq) ENUM_HPP_PP_SEQ_HEAD_II((ENUM_HPP_PP_SEQ_HEAD_I seq))
#define ENUM_HPP_PP_SEQ_HEAD_I(x) x, ENUM_HPP_PP_NOTHING
#define ENUM_HPP_PP_SEQ_HEAD_II(p) ENUM_HPP_PP_SEQ_HEAD_III p
#define ENUM_HPP_PP_SEQ_HEAD_III(x, _) x

//
// ENUM_HPP_PP_SEQ_TAIL
//

#define ENUM_HPP_PP_SEQ_TAIL(seq) ENUM_HPP_PP_SEQ_TAIL_I seq
#define ENUM_HPP_PP_SEQ_TAIL_I(_)

//
// ENUM_HPP_PP_SEQ_SIZE
//

#define ENUM_HPP_PP_SEQ_SIZE(seq)\
    ENUM_HPP_PP_CAT(ENUM_HPP_PP_SEQ_SIZE_, ENUM_HPP_PP_SEQ_SIZE_0 seq)

#define ENUM_HPP_PP_SEQ_SIZE_0(_) ENUM_HPP_PP_SEQ_SIZE_1
#define ENUM_HPP_PP_SEQ_SIZE_1(_) ENUM_HPP_PP_SEQ_SIZE_2
#define ENUM_HPP_PP_SEQ_SIZE_2(_) ENUM_HPP_PP_SEQ_SIZE_3
#define ENUM_HPP_PP_SEQ_SIZE_3(_) ENUM_HPP_PP_SEQ_SIZE_4
#define ENUM_HPP_PP_SEQ_SIZE_4(_) ENUM_HPP_PP_SEQ_SIZE_5

# define ENUM_HPP_PP_SEQ_SIZE_ENUM_HPP_PP_SEQ_SIZE_0 0
# define ENUM_HPP_PP_SEQ_SIZE_ENUM_HPP_PP_SEQ_SIZE_1 1
# define ENUM_HPP_PP_SEQ_SIZE_ENUM_HPP_PP_SEQ_SIZE_2 2
# define ENUM_HPP_PP_SEQ_SIZE_ENUM_HPP_PP_SEQ_SIZE_3 3
# define ENUM_HPP_PP_SEQ_SIZE_ENUM_HPP_PP_SEQ_SIZE_4 4
# define ENUM_HPP_PP_SEQ_SIZE_ENUM_HPP_PP_SEQ_SIZE_5 5

//
// ENUM_HPP_PP_SEQ_FOR_EACH
//

#define ENUM_HPP_PP_SEQ_FOR_EACH(m, d, s) ENUM_HPP_PP_SEQ_FOR_EACH_I(m, d, ENUM_HPP_PP_SEQ_SIZE(s), s)
#define ENUM_HPP_PP_SEQ_FOR_EACH_I(m, d, n, s) ENUM_HPP_PP_SEQ_FOR_EACH_II(m, d, n, s)
#define ENUM_HPP_PP_SEQ_FOR_EACH_II(m, d, n, s) ENUM_HPP_PP_SEQ_FOR_EACH_ ## n (m, d, s)

#define ENUM_HPP_PP_SEQ_FOR_EACH_1(m, d, s) m(d, ENUM_HPP_PP_SEQ_HEAD(s))
#define ENUM_HPP_PP_SEQ_FOR_EACH_2(m, d, s) m(d, ENUM_HPP_PP_SEQ_HEAD(s)) ENUM_HPP_PP_SEQ_FOR_EACH_1(m, d, ENUM_HPP_PP_SEQ_TAIL(s))
#define ENUM_HPP_PP_SEQ_FOR_EACH_3(m, d, s) m(d, ENUM_HPP_PP_SEQ_HEAD(s)) ENUM_HPP_PP_SEQ_FOR_EACH_2(m, d, ENUM_HPP_PP_SEQ_TAIL(s))
#define ENUM_HPP_PP_SEQ_FOR_EACH_4(m, d, s) m(d, ENUM_HPP_PP_SEQ_HEAD(s)) ENUM_HPP_PP_SEQ_FOR_EACH_3(m, d, ENUM_HPP_PP_SEQ_TAIL(s))
#define ENUM_HPP_PP_SEQ_FOR_EACH_5(m, d, s) m(d, ENUM_HPP_PP_SEQ_HEAD(s)) ENUM_HPP_PP_SEQ_FOR_EACH_4(m, d, ENUM_HPP_PP_SEQ_TAIL(s))
