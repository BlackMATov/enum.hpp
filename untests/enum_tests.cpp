/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
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
    ENUM_HPP_CLASS_DECL(color, unsigned,
        (red = 2)
        (green)
        (blue = red + 4)
    )

    struct render {
        ENUM_HPP_CLASS_DECL(mask, unsigned char,
            (none)
            (color = 1 << 0)
            (alpha = 1 << 1)
            (all = color | alpha)
        )
    };

    ENUM_HPP_DECL(numbers, int,
        (_0)(_1)(_2)(_3)(_4)(_5)(_6)(_7)(_8)(_9)(_10)(_11)(_12)(_13)(_14)(_15)(_16)(_17)(_18)(_19)(_20)
        (_21)(_22)(_23)(_24)(_25)(_26)(_27)(_28)(_29)(_30)(_31)(_32)(_33)(_34)(_35)(_36)(_37)(_38)(_39)(_40)
        (_41)(_42)(_43)(_44)(_45)(_46)(_47)(_48)(_49)(_50)(_51)(_52)(_53)(_54)(_55)(_56)(_57)(_58)(_59)(_60)
        (_61)(_62)(_63)(_64)(_65)(_66)(_67)(_68)(_69)(_70)(_71)(_72)(_73)(_74)(_75)(_76)(_77)(_78)(_79)(_80)
        (_81)(_82)(_83)(_84)(_85)(_86)(_87)(_88)(_89)(_90)(_91)(_92)(_93)(_94)(_95)(_96)(_97)(_98)(_99)(_100)
        (_101)(_102)(_103)(_104)(_105)(_106)(_107)(_108)(_109)(_110)(_111)(_112)(_113)(_114)(_115)(_116)(_117)(_118)(_119)(_120)
        (_121)(_122)(_123)(_124)(_125)(_126)(_127)(_128)(_129)(_130)(_131)(_132)(_133)(_134)(_135)(_136)(_137)(_138)(_139)(_140)
        (_141)(_142)(_143)(_144)(_145)(_146)(_147)(_148)(_149)(_150)(_151)(_152)(_153)(_154)(_155)(_156)(_157)(_158)(_159)(_160)
        (_161)(_162)(_163)(_164)(_165)(_166)(_167)(_168)(_169)(_170)(_171)(_172)(_173)(_174)(_175)(_176)(_177)(_178)(_179)(_180)
        (_181)(_182)(_183)(_184)(_185)(_186)(_187)(_188)(_189)(_190)(_191)(_192)(_193)(_194)(_195)(_196)(_197)(_198)(_199)(_200)
        (_201)(_202)(_203)(_204)(_205)(_206)(_207)(_208)(_209)(_210)(_211)(_212)(_213)(_214)(_215)(_216)(_217)(_218)(_219)(_220)
        (_221)(_222)(_223)(_224)(_225)(_226)(_227)(_228)(_229)(_230)(_231)(_232)(_233)(_234)(_235)(_236)(_237)(_238)(_239)(_240))

    ENUM_HPP_REGISTER_TRAITS(some_namespace::color)
    ENUM_HPP_REGISTER_TRAITS(some_namespace::numbers)
    ENUM_HPP_REGISTER_TRAITS(some_namespace::render::mask)

    namespace exns
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
}

TEST_CASE("enum") {
    namespace sn = some_namespace;

    SECTION("declaration") {
        {
            STATIC_REQUIRE(std::is_same_v<
                std::underlying_type_t<sn::color>,
                unsigned>);

            STATIC_REQUIRE(std::is_same_v<
                sn::color_traits::underlying_type,
                enum_hpp::underlying_type<sn::color>>);

            STATIC_REQUIRE(std::is_same_v<
                sn::color_traits,
                enum_hpp::traits_t<sn::color>>);

            REQUIRE(enum_to_underlying(sn::color::red) == 2u);
            REQUIRE(enum_to_underlying(sn::color::green) == 3u);
            REQUIRE(enum_to_underlying(sn::color::blue) == 6u);
        }
        {
            STATIC_REQUIRE(std::is_same_v<
                std::underlying_type_t<sn::render::mask>,
                unsigned char>);

            REQUIRE(enum_to_underlying(sn::render::mask::none) == 0u);
            REQUIRE(enum_to_underlying(sn::render::mask::color) == 1u);
            REQUIRE(enum_to_underlying(sn::render::mask::alpha) == 2u);
            REQUIRE(enum_to_underlying(sn::render::mask::all) == 3u);
        }
        {
            STATIC_REQUIRE(std::is_same_v<
                std::underlying_type_t<sn::numbers>,
                int>);

            STATIC_REQUIRE(std::is_same_v<
                sn::numbers_traits::underlying_type,
                enum_hpp::underlying_type<sn::numbers>>);

            STATIC_REQUIRE(std::is_same_v<
                sn::numbers_traits,
                enum_hpp::traits_t<sn::numbers>>);

            REQUIRE(enum_to_underlying(sn::_0) == 0);
            REQUIRE(enum_to_underlying(sn::_100) == 100);
            REQUIRE(enum_to_underlying(sn::_240) == 240);
        }
    }

    SECTION("size") {
        {
            STATIC_REQUIRE(sn::color_traits::size == 3u);
            STATIC_REQUIRE(sn::color_traits::size == std::size(sn::color_traits::names));
            STATIC_REQUIRE(sn::color_traits::size == std::size(sn::color_traits::values));

            STATIC_REQUIRE(enum_hpp::size<sn::color>() == 3u);
            STATIC_REQUIRE(enum_hpp::traits_t<sn::color>::size == 3u);
            STATIC_REQUIRE(std::size(enum_hpp::names<sn::color>()) == 3u);
            STATIC_REQUIRE(std::size(enum_hpp::values<sn::color>()) == 3u);
        }
        {
            STATIC_REQUIRE(sn::render::mask_traits::size == 4u);
            STATIC_REQUIRE(sn::render::mask_traits::size == std::size(sn::render::mask_traits::names));
            STATIC_REQUIRE(sn::render::mask_traits::size == std::size(sn::render::mask_traits::values));
        }
        {
            STATIC_REQUIRE(sn::numbers_traits::size == 241u);
            STATIC_REQUIRE(sn::numbers_traits::size == std::size(sn::numbers_traits::names));
            STATIC_REQUIRE(sn::numbers_traits::size == std::size(sn::numbers_traits::values));

            STATIC_REQUIRE(enum_hpp::size<sn::numbers>() == 241u);
            STATIC_REQUIRE(enum_hpp::traits_t<sn::numbers>::size == 241u);
            STATIC_REQUIRE(std::size(enum_hpp::names<sn::numbers>()) == 241u);
            STATIC_REQUIRE(std::size(enum_hpp::values<sn::numbers>()) == 241u);

            STATIC_REQUIRE(enum_hpp::size<sn::render::mask>() == 4u);
            STATIC_REQUIRE(enum_hpp::traits_t<sn::render::mask>::size == 4u);
            STATIC_REQUIRE(std::size(enum_hpp::names<sn::render::mask>()) == 4u);
            STATIC_REQUIRE(std::size(enum_hpp::values<sn::render::mask>()) == 4u);
        }
    }

    SECTION("to_underlying") {
        {
            STATIC_REQUIRE(sn::color_traits::to_underlying(sn::color::red) == enum_to_underlying(sn::color::red));
            STATIC_REQUIRE(sn::color_traits::to_underlying(sn::color::green) == enum_to_underlying(sn::color::green));
            STATIC_REQUIRE(sn::color_traits::to_underlying(sn::color::blue) == enum_to_underlying(sn::color::blue));
            STATIC_REQUIRE(sn::color_traits::to_underlying(sn::color(42)) == 42);

            STATIC_REQUIRE(enum_hpp::to_underlying(sn::color::red) == enum_to_underlying(sn::color::red));
            STATIC_REQUIRE(enum_hpp::to_underlying(sn::color::green) == enum_to_underlying(sn::color::green));
            STATIC_REQUIRE(enum_hpp::to_underlying(sn::color::blue) == enum_to_underlying(sn::color::blue));
            STATIC_REQUIRE(enum_hpp::to_underlying(sn::color(42)) == 42);
        }
        {
            STATIC_REQUIRE(sn::render::mask_traits::to_underlying(sn::render::mask::none) == enum_to_underlying(sn::render::mask::none));
            STATIC_REQUIRE(sn::render::mask_traits::to_underlying(sn::render::mask::color) == enum_to_underlying(sn::render::mask::color));
            STATIC_REQUIRE(sn::render::mask_traits::to_underlying(sn::render::mask::alpha) == enum_to_underlying(sn::render::mask::alpha));
            STATIC_REQUIRE(sn::render::mask_traits::to_underlying(sn::render::mask::all) == enum_to_underlying(sn::render::mask::all));
        }
        {
            STATIC_REQUIRE(sn::numbers_traits::to_underlying(sn::_0) == enum_to_underlying(sn::_0));
            STATIC_REQUIRE(sn::numbers_traits::to_underlying(sn::_180) == enum_to_underlying(sn::_180));
            STATIC_REQUIRE(sn::numbers_traits::to_underlying(sn::_240) == enum_to_underlying(sn::_240));
            STATIC_REQUIRE(sn::numbers_traits::to_underlying(sn::numbers(100500)) == 100500);

            STATIC_REQUIRE(enum_hpp::to_underlying(sn::_0) == enum_to_underlying(sn::_0));
            STATIC_REQUIRE(enum_hpp::to_underlying(sn::_180) == enum_to_underlying(sn::_180));
            STATIC_REQUIRE(enum_hpp::to_underlying(sn::_240) == enum_to_underlying(sn::_240));
            STATIC_REQUIRE(enum_hpp::to_underlying(sn::numbers(100500)) == 100500);
        }
    }

    SECTION("to_string") {
        {
            STATIC_REQUIRE(sn::color_traits::to_string(sn::color::red) == "red");
            STATIC_REQUIRE(sn::color_traits::to_string(sn::color::green) == "green");
            STATIC_REQUIRE(sn::color_traits::to_string(sn::color::blue) == "blue");

            STATIC_REQUIRE(sn::color_traits::to_string_or_empty(sn::color::red) == "red");
            STATIC_REQUIRE(sn::color_traits::to_string_or_empty(sn::color::green) == "green");
            STATIC_REQUIRE(sn::color_traits::to_string_or_empty(sn::color::blue) == "blue");

            REQUIRE(sn::color_traits::to_string_or_throw(sn::color::red) == "red");
            REQUIRE(sn::color_traits::to_string_or_throw(sn::color::green) == "green");
            REQUIRE(sn::color_traits::to_string_or_throw(sn::color::blue) == "blue");

            STATIC_REQUIRE_FALSE(sn::color_traits::to_string(sn::color(42)));
            STATIC_REQUIRE(sn::color_traits::to_string_or_empty(sn::color(42)) == "");
            REQUIRE_THROWS_AS(sn::color_traits::to_string_or_throw(sn::color(42)), enum_hpp::exception);

            STATIC_REQUIRE(enum_hpp::to_string(sn::color::green) == "green");
            STATIC_REQUIRE(enum_hpp::to_string_or_empty(sn::color::green) == "green");
            REQUIRE(enum_hpp::to_string_or_throw(sn::color::green) == "green");
            REQUIRE_THROWS_AS(enum_hpp::to_string_or_throw(sn::color(42)), enum_hpp::exception);
        }
        {
            STATIC_REQUIRE(sn::render::mask_traits::to_string(sn::render::mask::none) == "none");
            STATIC_REQUIRE(sn::render::mask_traits::to_string(sn::render::mask::color) == "color");
            STATIC_REQUIRE(sn::render::mask_traits::to_string(sn::render::mask::alpha) == "alpha");
            STATIC_REQUIRE(sn::render::mask_traits::to_string(sn::render::mask::all) == "all");

            STATIC_REQUIRE(sn::render::mask_traits::to_string_or_empty(sn::render::mask::none) == "none");
            STATIC_REQUIRE(sn::render::mask_traits::to_string_or_empty(sn::render::mask::color) == "color");
            STATIC_REQUIRE(sn::render::mask_traits::to_string_or_empty(sn::render::mask::alpha) == "alpha");
            STATIC_REQUIRE(sn::render::mask_traits::to_string_or_empty(sn::render::mask::all) == "all");

            REQUIRE(sn::render::mask_traits::to_string_or_throw(sn::render::mask::none) == "none");
            REQUIRE(sn::render::mask_traits::to_string_or_throw(sn::render::mask::color) == "color");
            REQUIRE(sn::render::mask_traits::to_string_or_throw(sn::render::mask::alpha) == "alpha");
            REQUIRE(sn::render::mask_traits::to_string_or_throw(sn::render::mask::all) == "all");
        }
        {
            STATIC_REQUIRE(sn::numbers_traits::to_string(sn::_0) == "_0");
            STATIC_REQUIRE(sn::numbers_traits::to_string(sn::_180) == "_180");
            STATIC_REQUIRE(sn::numbers_traits::to_string(sn::_240) == "_240");

            STATIC_REQUIRE(sn::numbers_traits::to_string_or_empty(sn::_0) == "_0");
            STATIC_REQUIRE(sn::numbers_traits::to_string_or_empty(sn::_180) == "_180");
            STATIC_REQUIRE(sn::numbers_traits::to_string_or_empty(sn::_240) == "_240");

            REQUIRE(sn::numbers_traits::to_string_or_throw(sn::_0) == "_0");
            REQUIRE(sn::numbers_traits::to_string_or_throw(sn::_180) == "_180");
            REQUIRE(sn::numbers_traits::to_string_or_throw(sn::_240) == "_240");

            STATIC_REQUIRE_FALSE(sn::numbers_traits::to_string(sn::numbers(100500)));
            STATIC_REQUIRE(sn::numbers_traits::to_string_or_empty(sn::numbers(100500)) == "");
            REQUIRE_THROWS_AS(sn::numbers_traits::to_string_or_throw(sn::numbers(100500)), enum_hpp::exception);

            STATIC_REQUIRE(enum_hpp::to_string(sn::_180) == "_180");
            STATIC_REQUIRE(enum_hpp::to_string_or_empty(sn::_180) == "_180");
            REQUIRE(enum_hpp::to_string_or_throw(sn::_180) == "_180");
            REQUIRE_THROWS_AS(enum_hpp::to_string_or_throw(sn::numbers(100500)), enum_hpp::exception);
        }
    }

    SECTION("from_string") {
        {
            STATIC_REQUIRE(sn::color_traits::from_string("red") == sn::color::red);
            STATIC_REQUIRE(sn::color_traits::from_string("green") == sn::color::green);
            STATIC_REQUIRE(sn::color_traits::from_string("blue") == sn::color::blue);

            STATIC_REQUIRE(sn::color_traits::from_string_or_default("red", sn::color::green) == sn::color::red);
            STATIC_REQUIRE(sn::color_traits::from_string_or_default("green", sn::color::red) == sn::color::green);
            STATIC_REQUIRE(sn::color_traits::from_string_or_default("blue", sn::color::red) == sn::color::blue);

            REQUIRE(sn::color_traits::from_string_or_throw("red") == sn::color::red);
            REQUIRE(sn::color_traits::from_string_or_throw("green") == sn::color::green);
            REQUIRE(sn::color_traits::from_string_or_throw("blue") == sn::color::blue);

            STATIC_REQUIRE_FALSE(sn::color_traits::from_string("42"));
            STATIC_REQUIRE(sn::color_traits::from_string_or_default("42", sn::color::red) == sn::color::red);
            REQUIRE_THROWS_AS(sn::color_traits::from_string_or_throw("42"), enum_hpp::exception);

            STATIC_REQUIRE(enum_hpp::from_string<sn::color>("green") == sn::color::green);
            STATIC_REQUIRE(enum_hpp::from_string_or_default("green", sn::color::red) == sn::color::green);
            REQUIRE(enum_hpp::from_string_or_throw<sn::color>("green") == sn::color::green);
            REQUIRE_THROWS_AS(enum_hpp::from_string_or_throw<sn::color>("42"), enum_hpp::exception);
        }
        {
            STATIC_REQUIRE(sn::render::mask_traits::from_string("none") == sn::render::mask::none);
            STATIC_REQUIRE(sn::render::mask_traits::from_string("color") == sn::render::mask::color);
            STATIC_REQUIRE(sn::render::mask_traits::from_string("alpha") == sn::render::mask::alpha);
            STATIC_REQUIRE(sn::render::mask_traits::from_string("all") == sn::render::mask::all);
            STATIC_REQUIRE_FALSE(sn::render::mask_traits::from_string("42"));
        }
        {
            STATIC_REQUIRE(sn::numbers_traits::from_string("_10") == sn::_10);
            STATIC_REQUIRE(sn::numbers_traits::from_string("_240") == sn::_240);
            STATIC_REQUIRE_FALSE(sn::numbers_traits::from_string("error"));

            STATIC_REQUIRE(enum_hpp::from_string<sn::numbers>("_10") == sn::_10);
            STATIC_REQUIRE(enum_hpp::from_string<sn::numbers>("_240") == sn::_240);
            STATIC_REQUIRE_FALSE(enum_hpp::from_string<sn::numbers>("error"));
        }
    }

    SECTION("to_index") {
        {
            STATIC_REQUIRE(sn::color_traits::to_index(sn::color::red) == 0u);
            STATIC_REQUIRE(sn::color_traits::to_index(sn::color::green) == 1u);
            STATIC_REQUIRE(sn::color_traits::to_index(sn::color::blue) == 2u);

            STATIC_REQUIRE(sn::color_traits::to_index_or_invalid(sn::color::red) == 0u);
            STATIC_REQUIRE(sn::color_traits::to_index_or_invalid(sn::color::green) == 1u);
            STATIC_REQUIRE(sn::color_traits::to_index_or_invalid(sn::color::blue) == 2u);

            REQUIRE(sn::color_traits::to_index_or_throw(sn::color::red) == 0u);
            REQUIRE(sn::color_traits::to_index_or_throw(sn::color::green) == 1u);
            REQUIRE(sn::color_traits::to_index_or_throw(sn::color::blue) == 2u);

            STATIC_REQUIRE_FALSE(sn::color_traits::to_index(sn::color(42)));
            STATIC_REQUIRE(sn::color_traits::to_index_or_invalid(sn::color(42)) == enum_hpp::invalid_index);
            REQUIRE_THROWS_AS(sn::color_traits::to_index_or_throw(sn::color(42)), enum_hpp::exception);

            STATIC_REQUIRE(enum_hpp::to_index(sn::color::green) == 1u);
            STATIC_REQUIRE(enum_hpp::to_index_or_invalid(sn::color::green) == 1u);
            REQUIRE(enum_hpp::to_index_or_throw(sn::color::green) == 1u);
            REQUIRE_THROWS_AS(enum_hpp::to_index_or_throw(sn::color(42)), enum_hpp::exception);
        }
        {
            STATIC_REQUIRE(sn::render::mask_traits::to_index(sn::render::mask::none) == 0u);
            STATIC_REQUIRE(sn::render::mask_traits::to_index(sn::render::mask::color) == 1u);
            STATIC_REQUIRE(sn::render::mask_traits::to_index(sn::render::mask::alpha) == 2u);
            STATIC_REQUIRE(sn::render::mask_traits::to_index(sn::render::mask::all) == 3u);

            STATIC_REQUIRE(sn::render::mask_traits::to_index_or_invalid(sn::render::mask::none) == 0u);
            STATIC_REQUIRE(sn::render::mask_traits::to_index_or_invalid(sn::render::mask::color) == 1u);
            STATIC_REQUIRE(sn::render::mask_traits::to_index_or_invalid(sn::render::mask::alpha) == 2u);
            STATIC_REQUIRE(sn::render::mask_traits::to_index_or_invalid(sn::render::mask::all) == 3u);

            REQUIRE(sn::render::mask_traits::to_index_or_throw(sn::render::mask::none) == 0u);
            REQUIRE(sn::render::mask_traits::to_index_or_throw(sn::render::mask::color) == 1u);
            REQUIRE(sn::render::mask_traits::to_index_or_throw(sn::render::mask::alpha) == 2u);
            REQUIRE(sn::render::mask_traits::to_index_or_throw(sn::render::mask::all) == 3u);
        }
        {
            STATIC_REQUIRE(sn::numbers_traits::to_index(sn::_0) == 0u);
            STATIC_REQUIRE(sn::numbers_traits::to_index(sn::_180) == 180u);
            STATIC_REQUIRE(sn::numbers_traits::to_index(sn::_240) == 240u);

            STATIC_REQUIRE(sn::numbers_traits::to_index_or_invalid(sn::_0) == 0u);
            STATIC_REQUIRE(sn::numbers_traits::to_index_or_invalid(sn::_180) == 180u);
            STATIC_REQUIRE(sn::numbers_traits::to_index_or_invalid(sn::_240) == 240u);

            REQUIRE(sn::numbers_traits::to_index_or_throw(sn::_0) == 0u);
            REQUIRE(sn::numbers_traits::to_index_or_throw(sn::_180) == 180u);
            REQUIRE(sn::numbers_traits::to_index_or_throw(sn::_240) == 240u);

            STATIC_REQUIRE_FALSE(sn::numbers_traits::to_index(sn::numbers(100500)));
            STATIC_REQUIRE(sn::numbers_traits::to_index_or_invalid(sn::numbers(100500)) == enum_hpp::invalid_index);
            REQUIRE_THROWS_AS(sn::numbers_traits::to_index_or_throw(sn::numbers(100500)), enum_hpp::exception);

            STATIC_REQUIRE(enum_hpp::to_index(sn::_180) == 180u);
            STATIC_REQUIRE(enum_hpp::to_index_or_invalid(sn::_180) == 180u);
            REQUIRE(enum_hpp::to_index_or_throw(sn::_180) == 180u);
            REQUIRE_THROWS_AS(enum_hpp::to_index_or_throw(sn::numbers(100500)), enum_hpp::exception);
        }
    }

    SECTION("from_index") {
        {
            STATIC_REQUIRE(sn::color_traits::from_index(0) == sn::color::red);
            STATIC_REQUIRE(sn::color_traits::from_index(1) == sn::color::green);
            STATIC_REQUIRE(sn::color_traits::from_index(2) == sn::color::blue);

            STATIC_REQUIRE(sn::color_traits::from_index_or_default(0, sn::color::green) == sn::color::red);
            STATIC_REQUIRE(sn::color_traits::from_index_or_default(1, sn::color::red) == sn::color::green);
            STATIC_REQUIRE(sn::color_traits::from_index_or_default(2, sn::color::red) == sn::color::blue);

            REQUIRE(sn::color_traits::from_index_or_throw(0) == sn::color::red);
            REQUIRE(sn::color_traits::from_index_or_throw(1) == sn::color::green);
            REQUIRE(sn::color_traits::from_index_or_throw(2) == sn::color::blue);

            STATIC_REQUIRE_FALSE(sn::color_traits::from_index(42));
            STATIC_REQUIRE(sn::color_traits::from_index_or_default(42, sn::color::red) == sn::color::red);
            REQUIRE_THROWS_AS(sn::color_traits::from_index_or_throw(42), enum_hpp::exception);

            STATIC_REQUIRE(enum_hpp::from_index<sn::color>(1) == sn::color::green);
            STATIC_REQUIRE(enum_hpp::from_index_or_default(1, sn::color::red) == sn::color::green);
            REQUIRE(enum_hpp::from_index_or_throw<sn::color>(1) == sn::color::green);
            REQUIRE_THROWS_AS(enum_hpp::from_index_or_throw<sn::color>(42), enum_hpp::exception);
        }
        {
            STATIC_REQUIRE(sn::render::mask_traits::from_index(0) == sn::render::mask::none);
            STATIC_REQUIRE(sn::render::mask_traits::from_index(1) == sn::render::mask::color);
            STATIC_REQUIRE(sn::render::mask_traits::from_index(2) == sn::render::mask::alpha);
            STATIC_REQUIRE(sn::render::mask_traits::from_index(3) == sn::render::mask::all);
            STATIC_REQUIRE_FALSE(sn::render::mask_traits::from_index(42));
        }
        {
            STATIC_REQUIRE(sn::numbers_traits::from_index(10) == sn::_10);
            STATIC_REQUIRE(sn::numbers_traits::from_index(240) == sn::_240);
            STATIC_REQUIRE_FALSE(sn::numbers_traits::from_index(100500));

            STATIC_REQUIRE(enum_hpp::from_index<sn::numbers>(10) == sn::_10);
            STATIC_REQUIRE(enum_hpp::from_index<sn::numbers>(240) == sn::_240);
            STATIC_REQUIRE_FALSE(enum_hpp::from_index<sn::numbers>(100500));
        }
    }
}

TEST_CASE("external_enum") {
    using ee = some_namespace::exns::external_enum;
    STATIC_REQUIRE(std::is_same_v<enum_hpp::underlying_type<ee>, unsigned short>);
    STATIC_REQUIRE(enum_hpp::size<ee>() == 3);

    STATIC_REQUIRE(enum_hpp::names<ee>()[0] == "a");
    STATIC_REQUIRE(enum_hpp::names<ee>()[2] == "c");

    STATIC_REQUIRE(enum_hpp::values<ee>()[0] == ee::a);
    STATIC_REQUIRE(enum_hpp::values<ee>()[2] == ee::c);

    STATIC_REQUIRE(enum_hpp::to_string(ee::c) == "c");
    STATIC_REQUIRE(enum_hpp::from_string<ee>("b") == ee::b);
}
