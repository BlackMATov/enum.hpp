/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#pragma once

#include <cstddef>

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

//
// ENUM_HPP_GENERATE_INTERNAL_NAMES
//

#define ENUM_HPP_GENERATE_INTERNAL_NAMES(Fields)

//
// ENUM_HPP_GENERATE_ENUM_FIELDS
//

#define ENUM_HPP_GENERATE_ENUM_FIELDS(Fields)

//
// ENUM_HPP_GENERATE_VALUES
//

#define ENUM_HPP_GENERATE_VALUES(Enum, Fields)\
    Enum(0),

//
// ENUM_HPP_GENERATE_NAMES
//

#define ENUM_HPP_GENERATE_NAMES(Fields)\
    "",

//
// ENUM_HPP_ENUM_CLASS
//

#define ENUM_HPP_ENUM_CLASS(Enum, Type, Fields)\
    enum class Enum : Type {\
        ENUM_HPP_GENERATE_ENUM_FIELDS(Fields)\
    };\
    struct Enum##_traits {\
    private:\
        enum enum_names_for_this_score_ { ENUM_HPP_GENERATE_INTERNAL_NAMES(Fields) };\
    public:\
        static constexpr std::size_t size = ENUM_HPP_PP_SEQ_SIZE(Fields);\
        static constexpr const Enum values[] = { ENUM_HPP_GENERATE_VALUES(Enum, Fields) }; \
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
// ENUM_HPP_PP_APPLY
//

#define ENUM_HPP_PP_APPLY(m, ...)\
    m(__VA_ARGS__)

//
// ENUM_HPP_PP_SEQ_HEAD
//

#define ENUM_HPP_PP_SEQ_HEAD(Seq) ENUM_HPP_PP_SEQ_HEAD_I(ENUM_HPP_PP_SEQ_HEAD_II Seq)
#define ENUM_HPP_PP_SEQ_HEAD_I(x) ENUM_HPP_PP_SEQ_HEAD_III(x)
#define ENUM_HPP_PP_SEQ_HEAD_II(x) x, ENUM_HPP_PP_SEQ_NOTHING
#define ENUM_HPP_PP_SEQ_HEAD_III(x, _) x

//
// ENUM_HPP_PP_SEQ_TAIL
//

#define ENUM_HPP_PP_SEQ_TAIL(Seq) ENUM_HPP_PP_SEQ_TAIL_I Seq
#define ENUM_HPP_PP_SEQ_TAIL_I(_)

//
// ENUM_HPP_PP_SEQ_SIZE
//

#define ENUM_HPP_PP_SEQ_SIZE(Seq)\
    ENUM_HPP_PP_CAT(ENUM_HPP_PP_SEQ_SIZE_, ENUM_HPP_PP_SEQ_SIZE_0 Seq)

#define ENUM_HPP_PP_SEQ_SIZE_0(_) ENUM_HPP_PP_SEQ_SIZE_1
#define ENUM_HPP_PP_SEQ_SIZE_1(_) ENUM_HPP_PP_SEQ_SIZE_2
#define ENUM_HPP_PP_SEQ_SIZE_2(_) ENUM_HPP_PP_SEQ_SIZE_3
#define ENUM_HPP_PP_SEQ_SIZE_3(_) ENUM_HPP_PP_SEQ_SIZE_4

#define ENUM_HPP_PP_SEQ_SIZE_ENUM_HPP_PP_SEQ_SIZE_0 0
#define ENUM_HPP_PP_SEQ_SIZE_ENUM_HPP_PP_SEQ_SIZE_1 1
#define ENUM_HPP_PP_SEQ_SIZE_ENUM_HPP_PP_SEQ_SIZE_2 2
#define ENUM_HPP_PP_SEQ_SIZE_ENUM_HPP_PP_SEQ_SIZE_3 3
#define ENUM_HPP_PP_SEQ_SIZE_ENUM_HPP_PP_SEQ_SIZE_4 4
