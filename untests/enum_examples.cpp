/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#include <enum.hpp/enum.hpp>

#include <string>
#include <iostream>

namespace
{
    ENUM_HPP_CLASS_DECL(color, unsigned,
        (red = 0xFF0000)
        (green = 0x00FF00)
        (blue = 0x0000FF)
        (white = red | green | blue))
}

TEST_CASE("examples") {
    SECTION("traits_using") {
        // size
        STATIC_REQUIRE(color_traits::size == 4);

        // to_underlying
        STATIC_REQUIRE(color_traits::to_underlying(color::white) == 0xFFFFFF);

        // to_string
        STATIC_REQUIRE(color_traits::to_string(color::red) == "red");
        STATIC_REQUIRE(color_traits::to_string(color(42)) == std::nullopt);

        // from_string
        STATIC_REQUIRE(color_traits::from_string("green") == color::green);
        STATIC_REQUIRE(color_traits::from_string("error") == std::nullopt);

        // to_index
        STATIC_REQUIRE(color_traits::to_index(color::blue) == 2u);
        STATIC_REQUIRE(color_traits::to_index(color(42)) == std::nullopt);

        // from_index
        STATIC_REQUIRE(color_traits::from_index(2) == color::blue);
        STATIC_REQUIRE(color_traits::from_index(42) == std::nullopt);

        // names
        for ( auto n : color_traits::names ) {
            std::cout << n << ",";
        }
    }
}
