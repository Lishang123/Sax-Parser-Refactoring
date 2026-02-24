#include <catch2/catch_test_macros.hpp>

#include "../String/generic.hpp"


using namespace M::String;

static_assert(StringType<const char*>);
static_assert(StringType<char*>);

// reference-to-pointer inputs
static_assert(StringType<const char*&>);
static_assert(StringType<char*&>);

static_assert(StringType<std::string>);
static_assert(StringType<std::string_view>);
static_assert(StringType<std::string&>);
static_assert(StringType<std::string_view&>);

// nullptr types
static_assert(!StringType<std::nullptr_t>);

static_assert(StringTypeLegacy<const std::nullptr_t>);  // original implementation
static_assert(!StringType<const std::nullptr_t>); // reject cv-qualified nullptr_t

static_assert(StringTypeLegacy<std::nullptr_t&>); // original implementation
static_assert(!StringType<std::nullptr_t&>); // reject ref-qualified nullptr_t

static_assert(StringTypeLegacy<const std::nullptr_t&>); // original implementation
static_assert(!StringType<const std::nullptr_t&>); // reject cv/ref-qualified nullptr_t


TEST_CASE("sv(const char*) keeps nullptr distinct from empty-string", "[string][generic]")
{
    const char* p = nullptr;
    auto view_from_nptr = sv(p);

    REQUIRE(view_from_nptr.empty());
    REQUIRE(view_from_nptr.data() == nullptr);

    const char* e = "";
    auto view_from_empty_string = sv(e);

    REQUIRE(view_from_empty_string.empty());
    REQUIRE(view_from_empty_string.data() != nullptr);

    REQUIRE_FALSE(equal(p, e)); // keep the original behavior
}

TEST_CASE("sv() on const char*&", "[string][generic]")
{
    const char* p = nullptr;
    const char*& ref = p;

    auto view_from_const_ref_nptr = sv(ref);

    REQUIRE(view_from_const_ref_nptr.empty());
    REQUIRE(view_from_const_ref_nptr.data() == nullptr);
}

TEST_CASE("sv() on char*&","[string][generic]") {
    char* p = nullptr;
    char*& ref = p;

    auto view_from_ref_nptr = sv(ref);

    REQUIRE(view_from_ref_nptr.empty());
    REQUIRE(view_from_ref_nptr.data() == nullptr);
}

TEST_CASE("equal/compare", "[string][generic]")
{
    REQUIRE(compare("123", "123") == 0);
    REQUIRE(compare("123", "124") < 0);
    REQUIRE(compare("123", "124",2) == 0);
    REQUIRE(equal("abc", std::string{"abc"}));
    REQUIRE(equal("abd", std::string{"abc"}, 2));
    // avoid name collision with std::equal
    REQUIRE(M::String::equal(std::string{"abd"}, std::string{"abc"}, 2));
    REQUIRE(equal(std::string_view{"abc"}, "abc"));
}