/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2019-2023, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <enum.hpp/enum_bitflags.hpp>

#include "enum_tests.hpp"

#include <string>
#include <iostream>

namespace
{
    enum class perms : unsigned {
        execute = 1 << 0,
        write = 1 << 1,
        read = 1 << 2,
    };
    ENUM_HPP_OPERATORS_DECL(perms)
}

TEST_CASE("bitflags_examples") {
    SUBCASE("Enum operators using") {
        namespace bf = enum_hpp::bitflags;

        bf::bitflags<perms> flags = perms::read | perms::write;

        if ( flags.has(perms::write) ) {
            flags.clear(perms::write);
        }

        flags.set(perms::write | perms::execute);

        if ( flags & perms::execute ) {
            flags ^= perms::execute; // flags.toggle(perms::execute);
        }

        CHECK(flags == (perms::read | perms::write));
    }

    SUBCASE("Additional bitflags functions") {
        namespace bf = enum_hpp::bitflags;
        bf::bitflags<perms> flags = perms::read | perms::write;
        CHECK(bf::any_of(flags, perms::write | perms::execute));
        CHECK(bf::any_except(flags, perms::write | perms::execute));
    }
}
