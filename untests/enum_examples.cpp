/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enum.hpp/enum.hpp>

#include "doctest/doctest.hpp"

#include <string>
#include <iostream>

namespace
{
    ENUM_HPP_CLASS_DECL(color, unsigned,
        (red = 0xFF0000)
        (green = 0x00FF00)
        (blue = 0x0000FF)
        (white = red | green | blue))

    ENUM_HPP_REGISTER_TRAITS(color)
}

namespace some_namespace
{
    ENUM_HPP_CLASS_DECL(color, unsigned,
        (red = 0xFF0000)
        (green = 0x00FF00)
        (blue = 0x0000FF)
        (white = red | green | blue))

    ENUM_HPP_REGISTER_TRAITS(color)
}

namespace external_ns
{
    enum class external_enum : unsigned short {
        a = 10,
        b,
        c = a + b
    };

    ENUM_HPP_TRAITS_DECL(external_enum,
        (a)
        (b)
        (c))

    ENUM_HPP_REGISTER_TRAITS(external_enum)
}

TEST_CASE("examples") {
    SUBCASE("traits using") {
        // size
        static_assert(color_traits::size == 4);

        // to_underlying
        static_assert(color_traits::to_underlying(color::white) == 0xFFFFFF);

        // to_string
        static_assert(color_traits::to_string(color::red) == "red");
        static_assert(color_traits::to_string(color(42)) == std::nullopt);

        // from_string
        static_assert(color_traits::from_string("green") == color::green);
        static_assert(color_traits::from_string("error") == std::nullopt);

        // to_index
        static_assert(color_traits::to_index(color::blue) == 2u);
        static_assert(color_traits::to_index(color(42)) == std::nullopt);

        // from_index
        static_assert(color_traits::from_index(2) == color::blue);
        static_assert(color_traits::from_index(42) == std::nullopt);

        // names
        for ( std::string_view n : color_traits::names ) {
            std::cout << n << ",";
        } // stdout: red,green,blue,
    }

    SUBCASE("generic context") {
        using color = some_namespace::color;

        // to string
        static_assert(enum_hpp::to_string(color::red) == "red");

        // from string
        static_assert(enum_hpp::from_string<color>("red") == color::red);
    }

    SUBCASE("external enums") {
        using ee = external_ns::external_enum;
        static_assert(enum_hpp::to_string(ee::a) == "a");
        static_assert(enum_hpp::from_string<ee>("c") == ee::c);
    }
}
