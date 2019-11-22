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
- [clang](https://clang.llvm.org/) **>= 5.0**
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
    /*...*/
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
    /*...*/
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

int main() {
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
    static_assert(color_traits::to_index(color::blue) == 2);
    static_assert(color_traits::to_index(color(42)) == std::nullopt);

    // from_index
    static_assert(color_traits::from_index(2) == color::blue);
    static_assert(color_traits::from_index(42) == std::nullopt);

    // names
    for ( auto n : color_traits::names ) {
        std::cout << n << ",";
    } // stdout: red,green,blue,

    return 0;
}
```

### Generic context

```cpp
namespace some_namespace
{
    ENUM_HPP_CLASS_DECL(color, unsigned,
        (red = 0xFF0000)
        (green = 0x00FF00)
        (blue = 0x0000FF)
        (white = red | green | blue))
}

// register traits in global namespace to generic access
ENUM_HPP_REGISTER_TRAITS(color)

int main() {
    // to string
    static_assert(enum_hpp::to_string(color::red) == "red");

    // from string
    static_assert(enum_hpp::from_string<color>("red") == color::red);

    return 0;
}
```

## API

### Enum traits

```cpp
// declare enum
ENUM_HPP_DECL(
    /*enum_name*/,
    /*underlying_type*/,
    /*fields*/)

// declare enum class
ENUM_HPP_DECL(
    /*enum_name*/,
    /*underlying_type*/,
    /*fields*/)

struct /*enum_name*/_traits {
    using underlying_type = /*underlying_type*/;
    static constexpr std::size_t size = /*field_count*/;

    static constexpr const std::array</*enum_name*/, /*field_count*/ > values = {
        /*enum values*/
    };

    static constexpr const std::array<std::string_view, /*field_count*/> names = {
        /*enum names*/
    };

    static constexpr /*underlying_type*/ to_underlying(/*enum_name*/ e) noexcept;

    static constexpr std::optional<std::string_view> to_string(/*enum_name*/ e) noexcept;
    static constexpr std::string_view to_string_or_empty(/*enum_name*/ e) noexcept;
    static std::string_view to_string_or_throw(/*enum_name*/ e);

    static constexpr std::optional</*enum_name*/> from_string(std::string_view name) noexcept;
    static constexpr /*enum_name*/ from_string_or_default(std::string_view name, /*enum_name*/ def) noexcept;
    static /*enum_name*/ from_string_or_throw(std::string_view name);

    static constexpr std::optional<std::size_t> to_index(/*enum_name*/ e) noexcept;
    static constexpr std::size_t to_index_or_invalid(/*enum_name*/ e) noexcept;
    static std::size_t to_index_or_throw(/*enum_name*/ e);

    static constexpr std::optional</*enum_name*/> from_index(std::size_t index) noexcept;
    static constexpr /*enum_name*/ from_index_or_default(std::size_t index, /*enum_name*/ def) noexcept;
    static /*enum_name*/ from_index_or_throw(std::size_t index);
};
```

### Generic functions

```cpp
// should be in global namespace
ENUM_HPP_REGISTER_TRAITS(/*declared_enum_name*/)

namespace enum_hpp
{
    template < typename Enum >
    using traits_t = typename traits<Enum>::type;

    template < typename Enum >
    using underlying_type = typename traits_t<Enum>::underlying_type;

    template < typename Enum >
    constexpr std::size_t size() noexcept;

    template < typename Enum >
    constexpr const std::array<Enum, size<Enum>()>& values() noexcept;

    template < typename Enum >
    constexpr const std::array<std::string_view, size<Enum>()>& names() noexcept;

    template < typename Enum >
    constexpr typename traits_t<Enum>::underlying_type to_underlying(Enum e) noexcept;

    template < typename Enum >
    constexpr std::optional<std::string_view> to_string(Enum e) noexcept;

    template < typename Enum >
    constexpr std::string_view to_string_or_empty(Enum e) noexcept;

    template < typename Enum >
    std::string_view to_string_or_throw(Enum e);

    template < typename Enum >
    constexpr std::optional<Enum> from_string(std::string_view name) noexcept;

    template < typename Enum >
    constexpr Enum from_string_or_default(std::string_view name, Enum def) noexcept;

    template < typename Enum >
    Enum from_string_or_throw(std::string_view name);

    template < typename Enum >
    constexpr std::optional<std::size_t> to_index(Enum e) noexcept;

    template < typename Enum >
    constexpr std::size_t to_index_or_invalid(Enum e) noexcept;

    template < typename Enum >
    std::size_t to_index_or_throw(Enum e);

    template < typename Enum >
    constexpr std::optional<Enum> from_index(std::size_t index) noexcept;

    template < typename Enum >
    constexpr Enum from_index_or_default(std::size_t index, Enum def) noexcept;

    template < typename Enum >
    Enum from_index_or_throw(std::size_t index);
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
