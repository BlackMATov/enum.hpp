# enum.hpp

[![travis][badge.travis]][travis]
[![appveyor][badge.appveyor]][appveyor]
[![codecov][badge.codecov]][codecov]
[![language][badge.language]][language]
[![license][badge.license]][license]
[![paypal][badge.paypal]][paypal]

[badge.travis]: https://img.shields.io/travis/BlackMATov/enum.hpp/master.svg?logo=travis
[badge.appveyor]: https://img.shields.io/appveyor/ci/BlackMATov/enum-hpp/master.svg?logo=appveyor
[badge.codecov]: https://img.shields.io/codecov/c/github/BlackMATov/enum.hpp/master.svg?logo=codecov
[badge.language]: https://img.shields.io/badge/language-C%2B%2B17-yellow.svg
[badge.license]: https://img.shields.io/badge/license-MIT-blue.svg
[badge.paypal]: https://img.shields.io/badge/donate-PayPal-orange.svg?logo=paypal&colorA=00457C

[travis]: https://travis-ci.org/BlackMATov/enum.hpp
[appveyor]: https://ci.appveyor.com/project/BlackMATov/enum-hpp
[codecov]: https://codecov.io/gh/BlackMATov/enum.hpp
[language]: https://en.wikipedia.org/wiki/C%2B%2B17
[license]: https://en.wikipedia.org/wiki/MIT_License
[paypal]: https://www.paypal.me/matov

[enum]: https://github.com/BlackMATov/enum.hpp

## Requirements

- [gcc](https://www.gnu.org/software/gcc/) **>= 7**
- [clang](https://clang.llvm.org/) **>= 7.0**
- [msvc](https://visualstudio.microsoft.com/) **>= 2017**

## Installation

[enum.hpp][enum] is a header-only library. All you need to do is copy the headers files from `headers` directory into your project and include them:

```cpp
#include "enum.hpp/enum.hpp"
```

Also, you can add the root repository directory to your [cmake](https://cmake.org) project:

```cmake
add_subdirectory(external/enum.hpp)
target_link_libraries(your_project_target enum.hpp)
```

## Examples

### Declarations

```cpp
ENUM_HPP_DECL(debug_level, int,
    (level_info)
    (level_warning)
    (level_error))

// equivalent to:

enum debug_level : int {
    level_info,
    level_warning,
    level_error,
};

struct debug_level_traits {
    using underlying_type = int;
    static constexpr std::size_t size = 3;

    static constexpr const debug_level values[] = {
        level_info,
        level_warning,
        level_warning
    };

    static constexpr const std::string_view names[] = {
        "level_info",
        "level_warning",
        "level_warning"
    };

    static constexpr underlying_type to_underlying(debug_level e) noexcept;

    static constexpr std::optional<std::string_view> to_string(debug_level e) noexcept;
    static constexpr std::optional<debug_level> from_string(std::string_view name) noexcept;

    static constexpr std::optional<std::size_t> to_index(debug_level e) noexcept;
    static constexpr std::optional<debug_level> from_index(std::size_t index) noexcept;
};
```

```cpp
ENUM_HPP_CLASS_DECL(color, unsigned,
    (red = 1 << 0)
    (green = 1 << 1)
    (blue = 1 << 2)
    (white = red | green | blue))

// equivalent to:

enum class color : unsigned {
    red = 1 << 0,
    green = 1 << 1,
    blue = 1 << 2,
    white = red | green | blue,
};

struct color_traits {
    using underlying_type = unsigned;
    static constexpr std::size_t size = 4;

    static constexpr const color values[] = {
        color::red,
        color::green,
        color::blue,
        color::white
    };

    static constexpr const std::string_view names[] = {
        "red",
        "green",
        "blue",
        "white"
    };

    static constexpr underlying_type to_underlying(color e) noexcept;

    static constexpr std::optional<std::string_view> to_string(color e) noexcept;
    static constexpr std::optional<color> from_string(std::string_view name) noexcept;

    static constexpr std::optional<std::size_t> to_index(color e) noexcept;
    static constexpr std::optional<color> from_index(std::size_t index) noexcept;
};
```

### Traits using

```cpp
namespace
{
    ENUM_HPP_CLASS_DECL(color, unsigned,
        (red = 0xFF0000)
        (green = 0x00FF00)
        (blue = 0x0000FF)
        (white = red | green | blue))
}

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
    STATIC_REQUIRE(color_traits::to_index(color::blue) == 2);
    STATIC_REQUIRE(color_traits::to_index(color(42)) == std::nullopt);

    // from_index
    STATIC_REQUIRE(color_traits::from_index(2) == color::blue);
    STATIC_REQUIRE(color_traits::from_index(42) == std::nullopt);

    // names
    for ( auto n : color_traits::names ) {
        std::cout << n << ",";
    } // stdout: red,green,blue,
}
```

## Alternatives

[Better Enums](https://github.com/aantron/better-enums)

- Low variadic macros limit (64)
- Replaces C++ enum semantics to custom class

[wise_enum](https://github.com/quicknir/wise_enum)

- Limited by variadic macros ([msvs: 127](https://docs.microsoft.com/en-us/cpp/cpp/compiler-limits?view=vs-2019))

[Magic Enum](https://github.com/Neargye/magic_enum)

- Amazing but requires at least GCC 9.0

## [License (MIT)](./LICENSE.md)
