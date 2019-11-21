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
        {
            STATIC_REQUIRE(std::is_same_v<
                std::underlying_type_t<sn::numbers>,
                int>);

            REQUIRE(enum_to_underlying(sn::_0) == 0);
            REQUIRE(enum_to_underlying(sn::_100) == 100);
            REQUIRE(enum_to_underlying(sn::_240) == 240);
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
        {
            REQUIRE(sn::numbers_traits::size == 241);
            REQUIRE(sn::numbers_traits::size == std::size(sn::numbers_traits::names));
            REQUIRE(sn::numbers_traits::size == std::size(sn::numbers_traits::values));
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
        {
            STATIC_REQUIRE(sn::numbers_traits::to_string(sn::_0) == "_0");
            STATIC_REQUIRE(sn::numbers_traits::to_string(sn::_180) == "_180");
            STATIC_REQUIRE(sn::numbers_traits::to_string(sn::_240) == "_240");
            STATIC_REQUIRE(sn::numbers_traits::to_string(sn::numbers(100500)) == "");
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
        {
            REQUIRE(sn::numbers_traits::from_string("_10") == sn::_10);
            REQUIRE(sn::numbers_traits::from_string("_240") == sn::_240);
            REQUIRE_THROWS_AS(sn::numbers_traits::from_string("error"), enum_hpp::exception);
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
        {
            sn::numbers result{100500};
            REQUIRE(sn::numbers_traits::from_string_nothrow("_240", result));
            REQUIRE(result == sn::_240);
            REQUIRE_FALSE(sn::numbers_traits::from_string_nothrow("error", result));
            REQUIRE(result == sn::_240);
        }
    }
}
