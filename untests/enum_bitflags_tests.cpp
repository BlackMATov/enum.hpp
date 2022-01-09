/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2022, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enum.hpp/enum_bitflags.hpp>

#include "doctest/doctest.hpp"

#include <cstdint>
#include <type_traits>

namespace
{
    enum class access : std::uint8_t {
        none = 0,
        read = 1 << 0,
        write = 1 << 1,
        execute = 1 << 2,
        read_write = read | write,
        all = read_write | execute
    };

    ENUM_HPP_OPERATORS_DECL(access)
}

TEST_CASE("enum_bitflags") {
    namespace bf = enum_hpp::bitflags;

    SUBCASE("ctors") {
        {
            constexpr bf::bitflags<access> f;
            STATIC_CHECK(!f);
            STATIC_CHECK(f.as_raw() == std::uint8_t{0x0});
            STATIC_CHECK(f.as_enum() == access::none);
        }
        {
            constexpr bf::bitflags f = access::read_write;
            STATIC_CHECK(!!f);
            STATIC_CHECK(f.as_raw() == std::uint8_t{0x3});
            STATIC_CHECK(f.as_enum() == access::read_write);
        }
        {
            constexpr bf::bitflags<access> f{std::uint8_t{0x3}};
            STATIC_CHECK(!!f);
            STATIC_CHECK(f.as_raw() == std::uint8_t{0x3});
            STATIC_CHECK(f.as_enum() == access::read_write);
        }
        {
            constexpr bf::bitflags f = access::read | access::write;
            constexpr bf::bitflags g = f;
            STATIC_CHECK(g.as_raw() == std::uint8_t{0x3});
        }
        {
            constexpr bf::bitflags f = access::read | access::write;
            bf::bitflags<access> g;
            g = f;
            CHECK(g.as_raw() == std::uint8_t{0x3});
        }
    }

    SUBCASE("swap") {
        {
            bf::bitflags f = access::read | access::write;
            bf::bitflags g = access::write | access::execute;
            f.swap(g);
            CHECK(f == (access::write | access::execute));
            CHECK(g == (access::read | access::write));
        }
        {
            bf::bitflags f = access::read | access::write;
            bf::bitflags g = access::write | access::execute;
            swap(f, g);
            CHECK(f == (access::write | access::execute));
            CHECK(g == (access::read | access::write));
        }
    }

    SUBCASE("hash") {
        std::hash<bf::bitflags<access>> hasher1;
        std::hash<bf::bitflags<access>> hasher2;
        CHECK(hasher1(access::read) == hasher2(access::read));
        CHECK(hasher1(access::read) != hasher2(access::read_write));
    }

    SUBCASE("enum_operators") {
        STATIC_CHECK((~access::read) == bf::bitflags<access>(0xFE));
        STATIC_CHECK((access::read | access::write) == bf::bitflags<access>(0x3));
        STATIC_CHECK((access::read_write & access::write) == bf::bitflags<access>(0x2));
        STATIC_CHECK((access::read_write ^ access::write) == bf::bitflags<access>(0x1));
    }

    SUBCASE("logic_operators") {
        STATIC_CHECK(std::uint8_t{0x1} < bf::bitflags{access::write});
        STATIC_CHECK(access::read < bf::bitflags{access::write});
        STATIC_CHECK(bf::bitflags{access::read} < std::uint8_t{0x2});
        STATIC_CHECK(bf::bitflags{access::read} < access::write);
        STATIC_CHECK(bf::bitflags{access::read} < bf::bitflags{access::write});

        STATIC_CHECK_FALSE(std::uint8_t{0x2} < bf::bitflags{access::read});
        STATIC_CHECK_FALSE(access::write < bf::bitflags{access::read});
        STATIC_CHECK_FALSE(bf::bitflags{access::write} < std::uint8_t{0x1});
        STATIC_CHECK_FALSE(bf::bitflags{access::write} < access::read);
        STATIC_CHECK_FALSE(bf::bitflags{access::write} < bf::bitflags{access::read});

        STATIC_CHECK(std::uint8_t{0x1} <= bf::bitflags{access::write});
        STATIC_CHECK(access::read <= bf::bitflags{access::write});
        STATIC_CHECK(bf::bitflags{access::read} <= std::uint8_t{0x2});
        STATIC_CHECK(bf::bitflags{access::read} <= access::write);
        STATIC_CHECK(bf::bitflags{access::read} <= bf::bitflags{access::write});

        STATIC_CHECK_FALSE(std::uint8_t{0x2} <= bf::bitflags{access::read});
        STATIC_CHECK_FALSE(access::write <= bf::bitflags{access::read});
        STATIC_CHECK_FALSE(bf::bitflags{access::write} <= std::uint8_t{0x1});
        STATIC_CHECK_FALSE(bf::bitflags{access::write} <= access::read);
        STATIC_CHECK_FALSE(bf::bitflags{access::write} <= bf::bitflags{access::read});

        STATIC_CHECK(std::uint8_t{0x2} <= bf::bitflags{access::write});
        STATIC_CHECK(access::write <= bf::bitflags{access::write});
        STATIC_CHECK(bf::bitflags{access::write} <= std::uint8_t{0x2});
        STATIC_CHECK(bf::bitflags{access::write} <= access::write);
        STATIC_CHECK(bf::bitflags{access::write} <= bf::bitflags{access::write});

        //

        STATIC_CHECK_FALSE(std::uint8_t{0x1} > bf::bitflags{access::write});
        STATIC_CHECK_FALSE(access::read > bf::bitflags{access::write});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} > std::uint8_t{0x2});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} > access::write);
        STATIC_CHECK_FALSE(bf::bitflags{access::read} > bf::bitflags{access::write});

        STATIC_CHECK(std::uint8_t{0x2} > bf::bitflags{access::read});
        STATIC_CHECK(access::write > bf::bitflags{access::read});
        STATIC_CHECK(bf::bitflags{access::write} > std::uint8_t{0x1});
        STATIC_CHECK(bf::bitflags{access::write} > access::read);
        STATIC_CHECK(bf::bitflags{access::write} > bf::bitflags{access::read});

        STATIC_CHECK_FALSE(std::uint8_t{0x1} >= bf::bitflags{access::write});
        STATIC_CHECK_FALSE(access::read >= bf::bitflags{access::write});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} >= std::uint8_t{0x2});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} >= access::write);
        STATIC_CHECK_FALSE(bf::bitflags{access::read} >= bf::bitflags{access::write});

        STATIC_CHECK(std::uint8_t{0x2} >= bf::bitflags{access::read});
        STATIC_CHECK(access::write >= bf::bitflags{access::read});
        STATIC_CHECK(bf::bitflags{access::write} >= std::uint8_t{0x1});
        STATIC_CHECK(bf::bitflags{access::write} >= access::read);
        STATIC_CHECK(bf::bitflags{access::write} >= bf::bitflags{access::read});

        STATIC_CHECK(std::uint8_t{0x2} >= bf::bitflags{access::write});
        STATIC_CHECK(access::write >= bf::bitflags{access::write});
        STATIC_CHECK(bf::bitflags{access::write} >= std::uint8_t{0x2});
        STATIC_CHECK(bf::bitflags{access::write} >= access::write);
        STATIC_CHECK(bf::bitflags{access::write} >= bf::bitflags{access::write});

        //

        STATIC_CHECK(std::uint8_t{0x1} == bf::bitflags{access::read});
        STATIC_CHECK(access::read == bf::bitflags{access::read});
        STATIC_CHECK(bf::bitflags{access::read} == std::uint8_t{0x1});
        STATIC_CHECK(bf::bitflags{access::read} == access::read);
        STATIC_CHECK(bf::bitflags{access::read} == bf::bitflags{access::read});

        STATIC_CHECK_FALSE(std::uint8_t{0x1} == bf::bitflags{access::write});
        STATIC_CHECK_FALSE(access::read == bf::bitflags{access::write});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} == std::uint8_t{0x2});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} == access::write);
        STATIC_CHECK_FALSE(bf::bitflags{access::read} == bf::bitflags{access::write});

        STATIC_CHECK(std::uint8_t{0x1} != bf::bitflags{access::write});
        STATIC_CHECK(access::read != bf::bitflags{access::write});
        STATIC_CHECK(bf::bitflags{access::read} != std::uint8_t{0x2});
        STATIC_CHECK(bf::bitflags{access::read} != access::write);
        STATIC_CHECK(bf::bitflags{access::read} != bf::bitflags{access::write});

        STATIC_CHECK_FALSE(std::uint8_t{0x1} != bf::bitflags{access::read});
        STATIC_CHECK_FALSE(access::read != bf::bitflags{access::read});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} != std::uint8_t{0x1});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} != access::read);
        STATIC_CHECK_FALSE(bf::bitflags{access::read} != bf::bitflags{access::read});
    }

    SUBCASE("bitflags_operators") {
        STATIC_CHECK(std::uint8_t{0xFE} == ~bf::bitflags{access::read});
        STATIC_CHECK(~bf::bitflags{access::read} == std::uint8_t{0xFE});
        STATIC_CHECK(~bf::bitflags{access::read} == bf::bitflags<access>(0xFE));

        STATIC_CHECK((access::write | bf::bitflags{access::read}) == std::uint8_t{0x3});
        STATIC_CHECK((bf::bitflags{access::read} | access::write) == std::uint8_t{0x3});
        STATIC_CHECK((bf::bitflags{access::read} | bf::bitflags{access::write}) == std::uint8_t{0x3});

        STATIC_CHECK((access::write & bf::bitflags{access::read_write}) == std::uint8_t{0x2});
        STATIC_CHECK((bf::bitflags{access::read_write} & access::write) == std::uint8_t{0x2});
        STATIC_CHECK((bf::bitflags{access::read_write} & bf::bitflags{access::write}) == std::uint8_t{0x2});

        STATIC_CHECK((access::write ^ bf::bitflags{access::read_write}) == std::uint8_t{0x1});
        STATIC_CHECK((bf::bitflags{access::read_write} ^ access::write) == std::uint8_t{0x1});
        STATIC_CHECK((bf::bitflags{access::read_write} ^ bf::bitflags{access::write}) == std::uint8_t{0x1});

        {
            bf::bitflags f{access::read};
            f |= access::write;
            CHECK(f == std::uint8_t{0x3});
        }

        {
            bf::bitflags f{access::read};
            f |= bf::bitflags{access::write};
            CHECK(f == std::uint8_t{0x3});
        }

        {
            bf::bitflags f{access::read_write};
            f &= access::write;
            CHECK(f == std::uint8_t{0x2});
        }

        {
            bf::bitflags f{access::read_write};
            f &= bf::bitflags{access::write};
            CHECK(f == std::uint8_t{0x2});
        }

        {
            bf::bitflags f{access::read_write};
            f ^= access::write;
            CHECK(f == std::uint8_t{0x1});
        }

        {
            bf::bitflags f{access::read_write};
            f ^= bf::bitflags{access::write};
            CHECK(f == std::uint8_t{0x1});
        }
    }

    SUBCASE("has") {
        constexpr bf::bitflags f = access::read_write;
        STATIC_CHECK(f.has(access::read));
        STATIC_CHECK(f.has(access::read_write));
        STATIC_CHECK(f.has(access::read | access::write));
        STATIC_CHECK_FALSE(f.has(access::execute));
        STATIC_CHECK_FALSE(f.has(access::read | access::execute));
    }

    SUBCASE("set") {
        bf::bitflags<access> f;
        CHECK(&f == &f.set(access::read));
        CHECK(f.has(access::read));
        CHECK(&f == &f.set(access::read | access::write));
        CHECK(f.has(access::read | access::write));
        CHECK_FALSE(f.has(access::execute));
    }

    SUBCASE("toggle") {
        bf::bitflags f = access::read_write;

        CHECK(&f == &f.toggle(access::read));
        CHECK_FALSE(f.has(access::read));
        CHECK(f.has(access::write));
        CHECK_FALSE(f.has(access::execute));

        CHECK(&f == &f.toggle(access::read | access::write));
        CHECK(f.has(access::read));
        CHECK_FALSE(f.has(access::write));
        CHECK_FALSE(f.has(access::execute));

        CHECK(&f == &f.toggle(access::execute));
        CHECK(f.has(access::read));
        CHECK_FALSE(f.has(access::write));
        CHECK(f.has(access::execute));
    }

    SUBCASE("clear") {
        {
            bf::bitflags f = access::all;

            CHECK(&f == &f.clear(access::read));
            CHECK_FALSE(f.has(access::read));
            CHECK(f.has(access::write));
            CHECK(f.has(access::execute));

            CHECK(&f == &f.clear(access::read | access::execute));
            CHECK_FALSE(f.has(access::read));
            CHECK(f.has(access::write));
            CHECK_FALSE(f.has(access::execute));
        }
        {
            bf::bitflags f = access::all;
            CHECK(&f == &f.clear(access::write | access::execute));
            CHECK(f.has(access::read));
            CHECK_FALSE(f.has(access::write));
            CHECK_FALSE(f.has(access::execute));

            CHECK(&f == &f.clear(access::read | access::execute));
            CHECK_FALSE(f.has(access::read));
            CHECK_FALSE(f.has(access::write));
            CHECK_FALSE(f.has(access::execute));
        }
    }

    SUBCASE("any") {
        STATIC_CHECK_FALSE(bf::any(access::none));
        STATIC_CHECK(bf::any(access::read));
        STATIC_CHECK(bf::any(access::write));
        STATIC_CHECK(bf::any(access::read_write));
    }

    SUBCASE("none") {
        STATIC_CHECK(bf::none(access::none));
        STATIC_CHECK_FALSE(bf::none(access::read));
        STATIC_CHECK_FALSE(bf::none(access::write));
        STATIC_CHECK_FALSE(bf::none(access::read_write));
    }

    SUBCASE("all_of") {
        STATIC_CHECK(bf::all_of(access::none, access::none));
        STATIC_CHECK_FALSE(bf::all_of(access::none, access::read));
        STATIC_CHECK_FALSE(bf::all_of(access::none, access::write));
        STATIC_CHECK_FALSE(bf::all_of(access::none, access::read_write));

        STATIC_CHECK(bf::all_of(access::read, access::none));
        STATIC_CHECK(bf::all_of(access::read, access::read));
        STATIC_CHECK_FALSE(bf::all_of(access::read, access::write));
        STATIC_CHECK_FALSE(bf::all_of(access::read, access::read_write));

        STATIC_CHECK(bf::all_of(access::write, access::none));
        STATIC_CHECK_FALSE(bf::all_of(access::write, access::read));
        STATIC_CHECK(bf::all_of(access::write, access::write));
        STATIC_CHECK_FALSE(bf::all_of(access::write, access::read_write));

        STATIC_CHECK(bf::all_of(access::read_write, access::none));
        STATIC_CHECK(bf::all_of(access::read_write, access::read));
        STATIC_CHECK(bf::all_of(access::read_write, access::write));
        STATIC_CHECK(bf::all_of(access::read_write, access::read_write));
    }

    SUBCASE("any_of") {
        STATIC_CHECK(bf::any_of(access::none, access::none));
        STATIC_CHECK_FALSE(bf::any_of(access::none, access::read));
        STATIC_CHECK_FALSE(bf::any_of(access::none, access::write));
        STATIC_CHECK_FALSE(bf::any_of(access::none, access::read_write));

        STATIC_CHECK(bf::any_of(access::read, access::none));
        STATIC_CHECK(bf::any_of(access::read, access::read));
        STATIC_CHECK_FALSE(bf::any_of(access::read, access::write));
        STATIC_CHECK(bf::any_of(access::read, access::read_write));

        STATIC_CHECK(bf::any_of(access::write, access::none));
        STATIC_CHECK_FALSE(bf::any_of(access::write, access::read));
        STATIC_CHECK(bf::any_of(access::write, access::write));
        STATIC_CHECK(bf::any_of(access::write, access::read_write));

        STATIC_CHECK(bf::any_of(access::read_write, access::none));
        STATIC_CHECK(bf::any_of(access::read_write, access::read));
        STATIC_CHECK(bf::any_of(access::read_write, access::write));
        STATIC_CHECK(bf::any_of(access::read_write, access::read_write));
    }

    SUBCASE("none_of") {
        STATIC_CHECK_FALSE(bf::none_of(access::none, access::none));
        STATIC_CHECK(bf::none_of(access::none, access::read));
        STATIC_CHECK(bf::none_of(access::none, access::write));
        STATIC_CHECK(bf::none_of(access::none, access::read_write));

        STATIC_CHECK_FALSE(bf::none_of(access::read, access::none));
        STATIC_CHECK_FALSE(bf::none_of(access::read, access::read));
        STATIC_CHECK(bf::none_of(access::read, access::write));
        STATIC_CHECK_FALSE(bf::none_of(access::read, access::read_write));

        STATIC_CHECK_FALSE(bf::none_of(access::write, access::none));
        STATIC_CHECK(bf::none_of(access::write, access::read));
        STATIC_CHECK_FALSE(bf::none_of(access::write, access::write));
        STATIC_CHECK_FALSE(bf::none_of(access::write, access::read_write));

        STATIC_CHECK_FALSE(bf::none_of(access::read_write, access::none));
        STATIC_CHECK_FALSE(bf::none_of(access::read_write, access::read));
        STATIC_CHECK_FALSE(bf::none_of(access::read_write, access::write));
        STATIC_CHECK_FALSE(bf::none_of(access::read_write, access::read_write));
    }

    SUBCASE("any_except") {
        STATIC_CHECK_FALSE(bf::any_except(access::none, access::none));
        STATIC_CHECK_FALSE(bf::any_except(access::none, access::read));
        STATIC_CHECK_FALSE(bf::any_except(access::none, access::write));
        STATIC_CHECK_FALSE(bf::any_except(access::none, access::read_write));

        STATIC_CHECK(bf::any_except(access::read, access::none));
        STATIC_CHECK_FALSE(bf::any_except(access::read, access::read));
        STATIC_CHECK(bf::any_except(access::read, access::write));
        STATIC_CHECK_FALSE(bf::any_except(access::read, access::read_write));

        STATIC_CHECK(bf::any_except(access::write, access::none));
        STATIC_CHECK(bf::any_except(access::write, access::read));
        STATIC_CHECK_FALSE(bf::any_except(access::write, access::write));
        STATIC_CHECK_FALSE(bf::any_except(access::write, access::read_write));

        STATIC_CHECK(bf::any_except(access::read_write, access::none));
        STATIC_CHECK(bf::any_except(access::read_write, access::read));
        STATIC_CHECK(bf::any_except(access::read_write, access::write));
        STATIC_CHECK_FALSE(bf::any_except(access::read_write, access::read_write));
    }

    SUBCASE("none_except") {
        STATIC_CHECK(bf::none_except(access::none, access::none));
        STATIC_CHECK(bf::none_except(access::none, access::read));
        STATIC_CHECK(bf::none_except(access::none, access::write));
        STATIC_CHECK(bf::none_except(access::none, access::read_write));

        STATIC_CHECK_FALSE(bf::none_except(access::read, access::none));
        STATIC_CHECK(bf::none_except(access::read, access::read));
        STATIC_CHECK_FALSE(bf::none_except(access::read, access::write));
        STATIC_CHECK(bf::none_except(access::read, access::read_write));

        STATIC_CHECK_FALSE(bf::none_except(access::write, access::none));
        STATIC_CHECK_FALSE(bf::none_except(access::write, access::read));
        STATIC_CHECK(bf::none_except(access::write, access::write));
        STATIC_CHECK(bf::none_except(access::write, access::read_write));

        STATIC_CHECK_FALSE(bf::none_except(access::read_write, access::none));
        STATIC_CHECK_FALSE(bf::none_except(access::read_write, access::read));
        STATIC_CHECK_FALSE(bf::none_except(access::read_write, access::write));
        STATIC_CHECK(bf::none_except(access::read_write, access::read_write));
    }
}
