# enum.hpp

> C++17 compile-time enum reflection library

[![travis][badge.travis]][travis]
[![appveyor][badge.appveyor]][appveyor]
[![codecov][badge.codecov]][codecov]
[![language][badge.language]][language]
[![license][badge.license]][license]
[![paypal][badge.paypal]][paypal]

[badge.travis]: https://img.shields.io/travis/BlackMATov/enum.hpp/main.svg?logo=travis
[badge.appveyor]: https://img.shields.io/appveyor/ci/BlackMATov/enum-hpp/main.svg?logo=appveyor
[badge.codecov]: https://img.shields.io/codecov/c/github/BlackMATov/enum.hpp/main.svg?logo=codecov
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

- [Enum declarations](#Enum-declarations)
- [Traits using](#Traits-using)
- [Generic context](#Generic-context)
- [Adapting external enums](#Adapting-external-enums)

### Enum declarations

```cpp
// declaration of unscoped enumeration `debug_level` with traits

ENUM_HPP_DECL(debug_level, int,
    (level_info)
    (level_warning)
    (level_error))

/* equivalent to:

enum debug_level : int {
    level_info,
    level_warning,
    level_error,
};

struct debug_level_traits {
    ...
};*/
```

```cpp
// declaration of scoped enumeration `color` with traits

ENUM_HPP_CLASS_DECL(color, unsigned,
    (red = 1 << 0)
    (green = 1 << 1)
    (blue = 1 << 2)
    (white = red | green | blue))

/* equivalent to:

enum class color : unsigned {
    red = 1 << 0,
    green = 1 << 1,
    blue = 1 << 2,
    white = red | green | blue,
};

struct color_traits {
    ...
};*/
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
    for ( std::string_view n : color_traits::names ) {
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

    // register traits in enum namespace to generic access
    ENUM_HPP_REGISTER_TRAITS(color)
}

int main() {
    using color = some_namespace::color;

    // to string
    static_assert(enum_hpp::to_string(color::red) == "red");

    // from string
    static_assert(enum_hpp::from_string<color>("red") == color::red);

    return 0;
}
```

### Adapting external enums

```cpp
namespace external_ns
{
    enum class external_enum : unsigned short {
        a = 10,
        b,
        c = a + b
    };

    // should be in the same namespace
    ENUM_HPP_TRAITS_DECL(external_enum,
        (a)
        (b)
        (c))

    ENUM_HPP_REGISTER_TRAITS(external_enum)
}

int main() {
    using ee = external_ns::external_enum;
    static_assert(enum_hpp::to_string(ee::a) == "a");
    static_assert(enum_hpp::from_string<ee>("c") == ee::c);
    return 0;
}
```

## API

- `enum.hpp`
  - [Enum traits](#Enum-traits)
  - [Enum generic functions](#Enum-generic-functions)
- `enum_bitflags.hpp`
  - [Enum bitflags](#Enum-bitflags)
  - [Enum operators](#Enum-operators)
  - [Enum bitflags operators](#Enum-bitflags-operators)
  - [Enum bitflags functions](#Enum-bitflags-functions)

### Enum traits

```cpp
// declares unscoped enumeration
ENUM_HPP_DECL(
    /*enum_name*/,
    /*underlying_type*/,
    /*fields*/)

// declares scoped enumeration
ENUM_HPP_CLASS_DECL(
    /*enum_name*/,
    /*underlying_type*/,
    /*fields*/)

// declares only traits for external enumerations
ENUM_HPP_TRAITS_DECL(
    /*enum_name*/,
    /*fields*/)

// declared enumeration traits
struct /*enum_name*/_traits {
    using enum_type = /*enum_name*/;
    using underlying_type = /*underlying_type*/;
    static constexpr std::size_t size = /*field_count*/;

    static constexpr const std::array<enum_type, size> values = {
        /*enum values*/
    };

    static constexpr const std::array<std::string_view, size> names = {
        /*enum value names*/
    };

    static constexpr underlying_type to_underlying(enum_type e) noexcept;

    static constexpr std::optional<std::string_view> to_string(enum_type e) noexcept;
    static constexpr std::string_view to_string_or_empty(enum_type e) noexcept;
    static std::string_view to_string_or_throw(enum_type e);

    static constexpr std::optional<enum_type> from_string(std::string_view name) noexcept;
    static constexpr enum_type from_string_or_default(std::string_view name, enum_type def) noexcept;
    static enum_type from_string_or_throw(std::string_view name);

    static constexpr std::optional<std::size_t> to_index(enum_type e) noexcept;
    static constexpr std::size_t to_index_or_invalid(enum_type e) noexcept;
    static std::size_t to_index_or_throw(enum_type e);

    static constexpr std::optional<enum_type> from_index(std::size_t index) noexcept;
    static constexpr enum_type from_index_or_default(std::size_t index, enum_type def) noexcept;
    static enum_type from_index_or_throw(std::size_t index);
};
```

### Enum generic functions

```cpp
// should be in enum namespace
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

### Enum bitflags

```cpp
namespace enum_hpp::bitflags
{
    template < typename Enum >
    class bitflags final {
    public:
        using enum_type = Enum;
        using underlying_type = std::underlying_type_t<Enum>;

        bitflags() = default;
        bitflags(const bitflags&) = default;
        bitflags& operator=(const bitflags&) = default;

        constexpr bitflags(enum_type flags);
        constexpr explicit bitflags(underlying_type flags);

        constexpr void swap(bitflags& other) noexcept;
        constexpr explicit operator bool() const noexcept;

        constexpr underlying_type as_raw() const noexcept;
        constexpr enum_type as_enum() const noexcept;

        constexpr bool has(bitflags flags) const noexcept;
        constexpr bitflags& set(bitflags flags) noexcept;
        constexpr bitflags& toggle(bitflags flags) noexcept;
        constexpr bitflags& clear(bitflags flags) noexcept;
    };

    template < typename Enum >
    constexpr void swap(bitflags<Enum>& l, bitflags<Enum>& r) noexcept;
}

namespace std
{
    template < typename Enum >
    struct hash<enum_hpp::bitflags::bitflags<Enum>> {
        size_t operator()(enum_hpp::bitflags::bitflags<Enum> bf) const noexcept;
    };
}
```

### Enum operators

```cpp
// declares enumeration operators
ENUM_HPP_OPERATORS_DECL(/*enum_name*/)

// declared enumeration operators
constexpr bitflags<Enum> operator ~ (/*enum_name*/ l) noexcept;
constexpr bitflags<Enum> operator | (/*enum_name*/ l, /*enum_name*/ r) noexcept;
constexpr bitflags<Enum> operator & (/*enum_name*/ l, /*enum_name*/ r) noexcept;
constexpr bitflags<Enum> operator ^ (/*enum_name*/ l, /*enum_name*/ r) noexcept;
```

### Enum bitflags operators

```cpp
namespace enum_hpp::bitflags
{
    template < typename Enum >
    constexpr bool operator < (Enum l, bitflags<Enum> r) noexcept;

    template < typename Enum >
    constexpr bool operator < (bitflags<Enum> l, Enum r) noexcept;

    template < typename Enum >
    constexpr bool operator < (std::underlying_type_t<Enum> l, bitflags<Enum> r) noexcept;

    template < typename Enum >
    constexpr bool operator < (bitflags<Enum> l, std::underlying_type_t<Enum> r) noexcept;

    template < typename Enum >
    constexpr bool operator < (bitflags<Enum> l, bitflags<Enum> r) noexcept;

    // and also for other comparison operators (<, >, <=, >=, ==, !=)
}

namespace enum_hpp::bitflags
{
    template < typename Enum >
    constexpr bitflags<Enum> operator ~ (bitflags<Enum> l) noexcept;

    template < typename Enum >
    constexpr bitflags<Enum> operator | (Enum l, bitflags<Enum> r) noexcept;

    template < typename Enum >
    constexpr bitflags<Enum> operator | (bitflags<Enum> l, Enum r) noexcept;

    template < typename Enum >
    constexpr bitflags<Enum> operator | (bitflags<Enum> l, bitflags<Enum> r) noexcept;

    template < typename Enum >
    constexpr bitflags<Enum>& operator |= (bitflags<Enum>& l, Enum r) noexcept;

    template < typename Enum >
    constexpr bitflags<Enum>& operator |= (bitflags<Enum>& l, bitflags<Enum> r) noexcept;

    // and also for other bitwise logic operators (|, |=, &, &=, ^, ^=)
}
```

### Enum bitflags functions

```cpp
namespace enum_hpp::bitflags
{
    // any

    template < enum Enum >
    constexpr bool any(Enum flags) noexcept;

    template < typename Enum >
    constexpr bool any(bitflags<Enum> flags) noexcept;

    // none

    template < enum Enum >
    constexpr bool none(Enum flags) noexcept;

    template < typename Enum >
    constexpr bool none(bitflags<Enum> flags) noexcept;

    // all_of

    template < enum Enum >
    constexpr bool all_of(Enum flags, Enum mask) noexcept;

    template < typename Enum >
    constexpr bool all_of(Enum flags, bitflags<Enum> mask) noexcept;

    template < typename Enum >
    constexpr bool all_of(bitflags<Enum> flags, Enum mask) noexcept;

    template < typename Enum >
    constexpr bool all_of(bitflags<Enum> flags, bitflags<Enum> mask) noexcept;

    // any_of

    template < enum Enum >
    constexpr bool any_of(Enum flags, Enum mask) noexcept;

    template < typename Enum >
    constexpr bool any_of(Enum flags, bitflags<Enum> mask) noexcept;

    template < typename Enum >
    constexpr bool any_of(bitflags<Enum> flags, Enum mask) noexcept;

    template < typename Enum >
    constexpr bool any_of(bitflags<Enum> flags, bitflags<Enum> mask) noexcept;

    // none_of

    template < enum Enum >
    constexpr bool none_of(Enum flags, Enum mask) noexcept;

    template < typename Enum >
    constexpr bool none_of(Enum flags, bitflags<Enum> mask) noexcept;

    template < typename Enum >
    constexpr bool none_of(bitflags<Enum> flags, Enum mask) noexcept;

    template < typename Enum >
    constexpr bool none_of(bitflags<Enum> flags, bitflags<Enum> mask) noexcept;

    // any_except

    template < enum Enum >
    constexpr bool any_except(Enum flags, Enum mask) noexcept;

    template < typename Enum >
    constexpr bool any_except(Enum flags, bitflags<Enum> mask) noexcept;

    template < typename Enum >
    constexpr bool any_except(bitflags<Enum> flags, Enum mask) noexcept;

    template < typename Enum >
    constexpr bool any_except(bitflags<Enum> flags, bitflags<Enum> mask) noexcept;

    // none_except

    template < enum Enum >
    constexpr bool none_except(Enum flags, Enum mask) noexcept;

    template < typename Enum >
    constexpr bool none_except(Enum flags, bitflags<Enum> mask) noexcept;

    template < typename Enum >
    constexpr bool none_except(bitflags<Enum> flags, Enum mask) noexcept;

    template < typename Enum >
    constexpr bool none_except(bitflags<Enum> flags, bitflags<Enum> mask) noexcept;
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
