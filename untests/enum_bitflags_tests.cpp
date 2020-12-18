/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2020, by Matvey Cherevko (blackmatov@gmail.com)
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
        read_write = read | write,
    };

    ENUM_HPP_REGISTER_BITFLAGS_OPERATORS(access)
}

TEST_CASE("enum_bitflags") {
    namespace bf = enum_hpp::bitflags;

    SUBCASE("enum_operators") {
        STATIC_CHECK((~access::read) == bf::bitflags<access>(0xFE));
        STATIC_CHECK((access::read | access::write) == bf::bitflags<access>(0x3));
        STATIC_CHECK((access::read_write & access::write) == bf::bitflags<access>(0x2));
        STATIC_CHECK((access::read_write ^ access::write) == bf::bitflags<access>(0x1));
    }

    SUBCASE("logic_operators") {
        STATIC_CHECK(bf::bitflags{access::read} < bf::bitflags{access::write});
        STATIC_CHECK_FALSE(bf::bitflags{access::write} < bf::bitflags{access::read});
        STATIC_CHECK(bf::bitflags{access::read} <= bf::bitflags{access::write});
        STATIC_CHECK_FALSE(bf::bitflags{access::write} <= bf::bitflags{access::read});
        STATIC_CHECK(bf::bitflags{access::write} <= bf::bitflags{access::write});

        STATIC_CHECK(bf::bitflags{access::write} > bf::bitflags{access::read});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} > bf::bitflags{access::write});
        STATIC_CHECK(bf::bitflags{access::write} >= bf::bitflags{access::read});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} >= bf::bitflags{access::write});
        STATIC_CHECK(bf::bitflags{access::read} >= bf::bitflags{access::read});

        STATIC_CHECK(bf::bitflags{access::read} == bf::bitflags{access::read});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} == bf::bitflags{access::write});

        STATIC_CHECK(bf::bitflags{access::read} != bf::bitflags{access::write});
        STATIC_CHECK_FALSE(bf::bitflags{access::read} != bf::bitflags{access::read});
    }

    SUBCASE("bitflags_operators") {
        STATIC_CHECK(0xFE == ~bf::bitflags{access::read});
        STATIC_CHECK(~bf::bitflags{access::read} == 0xFE);
        STATIC_CHECK(~bf::bitflags{access::read} == bf::bitflags<access>(0xFE));

        STATIC_CHECK((access::write | bf::bitflags{access::read}) == 0x3);
        STATIC_CHECK((bf::bitflags{access::read} | access::write) == 0x3);
        STATIC_CHECK((bf::bitflags{access::read} | bf::bitflags{access::write}) == 0x3);

        STATIC_CHECK((access::write & bf::bitflags{access::read_write}) == 0x2);
        STATIC_CHECK((bf::bitflags{access::read_write} & access::write) == 0x2);
        STATIC_CHECK((bf::bitflags{access::read_write} & bf::bitflags{access::write}) == 0x2);

        STATIC_CHECK((access::write ^ bf::bitflags{access::read_write}) == 0x1);
        STATIC_CHECK((bf::bitflags{access::read_write} ^ access::write) == 0x1);
        STATIC_CHECK((bf::bitflags{access::read_write} ^ bf::bitflags{access::write}) == 0x1);

        {
            bf::bitflags f{access::read};
            f |= access::write;
            CHECK(f == 0x3);
        }

        {
            bf::bitflags f{access::read};
            f |= bf::bitflags{access::write};
            CHECK(f == 0x3);
        }

        {
            bf::bitflags f{access::read_write};
            f &= access::write;
            CHECK(f == 0x2);
        }

        {
            bf::bitflags f{access::read_write};
            f &= bf::bitflags{access::write};
            CHECK(f == 0x2);
        }

        {
            bf::bitflags f{access::read_write};
            f ^= access::write;
            CHECK(f == 0x1);
        }

        {
            bf::bitflags f{access::read_write};
            f ^= bf::bitflags{access::write};
            CHECK(f == 0x1);
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
