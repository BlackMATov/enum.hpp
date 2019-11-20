/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#define CATCH_CONFIG_FAST_COMPILE
#include <catch2/catch.hpp>

#include <enum.hpp/enum.hpp>

#include <iterator>
#include <string_view>
#include <type_traits>

using namespace std::string_view_literals;

namespace
{
    template < typename Enum >
    constexpr std::underlying_type_t<Enum> enum_to_underlying(Enum e) noexcept {
        return static_cast<std::underlying_type_t<Enum>>(e);
    }
}

namespace some_namespace
{
    ENUM_HPP_ENUM_CLASS(color, unsigned,
        (red = 2)
        (green)
        (blue = red + 4)
    )

    struct render {
        ENUM_HPP_ENUM_CLASS(mask, unsigned char,
            (none)
            (color = 1 << 0)
            (alpha = 1 << 1)
            (all = color | alpha)
        )
    };
}

TEST_CASE("enum") {
    namespace sn = some_namespace;

    SECTION("declaration") {
        {
            STATIC_REQUIRE(std::is_same_v<
                std::underlying_type_t<sn::color>,
                unsigned>);

            REQUIRE(enum_to_underlying(sn::color::red) == 2);
            REQUIRE(enum_to_underlying(sn::color::green) == 3);
            REQUIRE(enum_to_underlying(sn::color::blue) == 6);
        }
        {
            STATIC_REQUIRE(std::is_same_v<
                std::underlying_type_t<sn::render::mask>,
                unsigned char>);

            REQUIRE(enum_to_underlying(sn::render::mask::none) == 0);
            REQUIRE(enum_to_underlying(sn::render::mask::color) == 1);
            REQUIRE(enum_to_underlying(sn::render::mask::alpha) == 2);
            REQUIRE(enum_to_underlying(sn::render::mask::all) == 3);
        }
    }

    SECTION("size") {
        {
            REQUIRE(sn::color_traits::size == 3);
            REQUIRE(sn::color_traits::size == std::size(sn::color_traits::names));
            REQUIRE(sn::color_traits::size == std::size(sn::color_traits::values));
        }
        {
            REQUIRE(sn::render::mask_traits::size == 4);
            REQUIRE(sn::render::mask_traits::size == std::size(sn::render::mask_traits::names));
            REQUIRE(sn::render::mask_traits::size == std::size(sn::render::mask_traits::values));
        }
    }

    SECTION("to_string") {
        {
            STATIC_REQUIRE(sn::color_traits::to_string(sn::color::red) == "red");
            STATIC_REQUIRE(sn::color_traits::to_string(sn::color::green) == "green");
            STATIC_REQUIRE(sn::color_traits::to_string(sn::color::blue) == "blue");
            STATIC_REQUIRE(sn::color_traits::to_string(sn::color(42)) == "");
        }
        {
            STATIC_REQUIRE(sn::render::mask_traits::to_string(sn::render::mask::none) == "none");
            STATIC_REQUIRE(sn::render::mask_traits::to_string(sn::render::mask::color) == "color");
            STATIC_REQUIRE(sn::render::mask_traits::to_string(sn::render::mask::alpha) == "alpha");
            STATIC_REQUIRE(sn::render::mask_traits::to_string(sn::render::mask::all) == "all");
        }
    }

    SECTION("from_string") {
        {
            REQUIRE(sn::color_traits::from_string("red") == sn::color::red);
            REQUIRE(sn::color_traits::from_string("green") == sn::color::green);
            REQUIRE(sn::color_traits::from_string("blue") == sn::color::blue);
            REQUIRE_THROWS_AS(sn::color_traits::from_string("42"), enum_hpp::exception);
        }
        {
            REQUIRE(sn::render::mask_traits::from_string("none") == sn::render::mask::none);
            REQUIRE(sn::render::mask_traits::from_string("color") == sn::render::mask::color);
            REQUIRE(sn::render::mask_traits::from_string("alpha") == sn::render::mask::alpha);
            REQUIRE(sn::render::mask_traits::from_string("all") == sn::render::mask::all);
            REQUIRE_THROWS_AS(sn::render::mask_traits::from_string("42"), enum_hpp::exception);
        }
    }

    SECTION("from_string_nothrow") {
        {
            sn::color result{42};
            REQUIRE(sn::color_traits::from_string_nothrow("red", result));
            REQUIRE(result == sn::color::red);
            REQUIRE(sn::color_traits::from_string_nothrow("green", result));
            REQUIRE(result == sn::color::green);
            REQUIRE(sn::color_traits::from_string_nothrow("blue", result));
            REQUIRE(result == sn::color::blue);
            REQUIRE_FALSE(sn::color_traits::from_string_nothrow("42", result));
            REQUIRE(result == sn::color::blue);
        }
        {
            sn::render::mask result{42};
            REQUIRE(sn::render::mask_traits::from_string_nothrow("none", result));
            REQUIRE(result == sn::render::mask::none);
            REQUIRE(sn::render::mask_traits::from_string_nothrow("color", result));
            REQUIRE(result == sn::render::mask::color);
            REQUIRE(sn::render::mask_traits::from_string_nothrow("alpha", result));
            REQUIRE(result == sn::render::mask::alpha);
            REQUIRE(sn::render::mask_traits::from_string_nothrow("all", result));
            REQUIRE(result == sn::render::mask::all);
            REQUIRE_FALSE(sn::render::mask_traits::from_string_nothrow("42", result));
            REQUIRE(result == sn::render::mask::all);
        }
    }
}
