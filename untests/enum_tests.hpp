/*******************************************************************************
 * This file is part of the "https://github.com/blackmatov/enum.hpp"
 * For conditions of distribution and use, see copyright notice in LICENSE.md
 * Copyright (C) 2020-2022, by Matvey Cherevko (blackmatov@gmail.com)
 ******************************************************************************/

#include <doctest/doctest.h>

#define STATIC_CHECK(...)\
    static_assert(__VA_ARGS__, #__VA_ARGS__);\
    CHECK(__VA_ARGS__)

#define STATIC_CHECK_FALSE(...)\
    static_assert(!(__VA_ARGS__), "!(" #__VA_ARGS__ ")");\
    CHECK(!(__VA_ARGS__))
