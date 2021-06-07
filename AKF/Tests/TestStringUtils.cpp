

#include <LibTest/TestCase.h>

#include <AKF/StringUtils.h>

TEST_CASE(matches_null)
{
    EXPECT(AKF::StringUtils::matches(StringView(), StringView()));

    EXPECT(!AKF::StringUtils::matches(StringView(), ""));
    EXPECT(!AKF::StringUtils::matches(StringView(), "*"));
    EXPECT(!AKF::StringUtils::matches(StringView(), "?"));
    EXPECT(!AKF::StringUtils::matches(StringView(), "a"));

    EXPECT(!AKF::StringUtils::matches("", StringView()));
    EXPECT(!AKF::StringUtils::matches("a", StringView()));
}

TEST_CASE(matches_empty)
{
    EXPECT(AKF::StringUtils::matches("", ""));

    EXPECT(AKF::StringUtils::matches("", "*"));
    EXPECT(!AKF::StringUtils::matches("", "?"));
    EXPECT(!AKF::StringUtils::matches("", "a"));

    EXPECT(!AKF::StringUtils::matches("a", ""));
}

TEST_CASE(matches_case_sensitive)
{
    EXPECT(AKF::StringUtils::matches("a", "a", CaseSensitivity::CaseSensitive));
    EXPECT(!AKF::StringUtils::matches("a", "A", CaseSensitivity::CaseSensitive));
    EXPECT(!AKF::StringUtils::matches("A", "a", CaseSensitivity::CaseSensitive));
}

TEST_CASE(matches_case_insensitive)
{
    EXPECT(!AKF::StringUtils::matches("aa", "a"));
    EXPECT(AKF::StringUtils::matches("aa", "*"));
    EXPECT(!AKF::StringUtils::matches("cb", "?a"));
    EXPECT(AKF::StringUtils::matches("adceb", "a*b"));
    EXPECT(!AKF::StringUtils::matches("acdcb", "a*c?b"));
}

TEST_CASE(matches_with_positions)
{
    Vector<AKF::MaskSpan> spans;
    EXPECT(AKF::StringUtils::matches("abbb", "a*", CaseSensitivity::CaseSensitive, &spans));
    EXPECT(spans == Vector<AKF::MaskSpan>({ { 1, 3 } }));

    spans.clear();
    EXPECT(AKF::StringUtils::matches("abbb", "?*", CaseSensitivity::CaseSensitive, &spans));
    EXPECT_EQ(spans, Vector<AKF::MaskSpan>({ { 0, 1 }, { 1, 3 } }));

    spans.clear();
    EXPECT(AKF::StringUtils::matches("acdcxb", "a*c?b", CaseSensitivity::CaseSensitive, &spans));
    EXPECT_EQ(spans, Vector<AKF::MaskSpan>({ { 1, 2 }, { 4, 1 } }));

    spans.clear();
    EXPECT(AKF::StringUtils::matches("aaaa", "A*", CaseSensitivity::CaseInsensitive, &spans));
    EXPECT_EQ(spans, Vector<AKF::MaskSpan>({ { 1, 3 } }));
}

// #4607
TEST_CASE(matches_trailing)
{
    EXPECT(AKF::StringUtils::matches("ab", "ab*"));
    EXPECT(AKF::StringUtils::matches("ab", "ab****"));
    EXPECT(AKF::StringUtils::matches("ab", "*ab****"));
}

TEST_CASE(convert_to_int)
{
    auto value = AKF::StringUtils::convert_to_int(StringView());
    EXPECT(!value.has_value());

    value = AKF::StringUtils::convert_to_int("");
    EXPECT(!value.has_value());

    value = AKF::StringUtils::convert_to_int("a");
    EXPECT(!value.has_value());

    value = AKF::StringUtils::convert_to_int("+");
    EXPECT(!value.has_value());

    value = AKF::StringUtils::convert_to_int("-");
    EXPECT(!value.has_value());

    auto actual = AKF::StringUtils::convert_to_int("0");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), 0);

    actual = AKF::StringUtils::convert_to_int("1");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), 1);

    actual = AKF::StringUtils::convert_to_int("+1");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), 1);

    actual = AKF::StringUtils::convert_to_int("-1");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), -1);

    actual = AKF::StringUtils::convert_to_int("01");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), 1);

    actual = AKF::StringUtils::convert_to_int("12345");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), 12345);

    actual = AKF::StringUtils::convert_to_int("-12345");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), -12345);

    actual = AKF::StringUtils::convert_to_int(" \t-12345 \n\n");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), -12345);

    auto actual_i8 = AKF::StringUtils::convert_to_int<i8>("-1");
    EXPECT(actual_i8.has_value());
    EXPECT_EQ(actual_i8.value(), -1);
    EXPECT_EQ(sizeof(actual_i8.value()), (size_t)1);
    actual_i8 = AKF::StringUtils::convert_to_int<i8>("128");
    EXPECT(!actual_i8.has_value());

    auto actual_i16 = AKF::StringUtils::convert_to_int<i16>("-1");
    EXPECT(actual_i16.has_value());
    EXPECT_EQ(actual_i16.value(), -1);
    EXPECT_EQ(sizeof(actual_i16.value()), (size_t)2);
    actual_i16 = AKF::StringUtils::convert_to_int<i16>("32768");
    EXPECT(!actual_i16.has_value());

    auto actual_i32 = AKF::StringUtils::convert_to_int<i32>("-1");
    EXPECT(actual_i32.has_value());
    EXPECT_EQ(actual_i32.value(), -1);
    EXPECT_EQ(sizeof(actual_i32.value()), (size_t)4);
    actual_i32 = AKF::StringUtils::convert_to_int<i32>("2147483648");
    EXPECT(!actual_i32.has_value());

    auto actual_i64 = AKF::StringUtils::convert_to_int<i64>("-1");
    EXPECT(actual_i64.has_value());
    EXPECT_EQ(actual_i64.value(), -1);
    EXPECT_EQ(sizeof(actual_i64.value()), (size_t)8);
    actual_i64 = AKF::StringUtils::convert_to_int<i64>("9223372036854775808");
    EXPECT(!actual_i64.has_value());
}

TEST_CASE(convert_to_uint)
{
    auto value = AKF::StringUtils::convert_to_uint(StringView());
    EXPECT(!value.has_value());

    value = AKF::StringUtils::convert_to_uint("");
    EXPECT(!value.has_value());

    value = AKF::StringUtils::convert_to_uint("a");
    EXPECT(!value.has_value());

    value = AKF::StringUtils::convert_to_uint("+");
    EXPECT(!value.has_value());

    value = AKF::StringUtils::convert_to_uint("-");
    EXPECT(!value.has_value());

    value = AKF::StringUtils::convert_to_uint("+1");
    EXPECT(!value.has_value());

    value = AKF::StringUtils::convert_to_uint("-1");
    EXPECT(!value.has_value());

    auto actual = AKF::StringUtils::convert_to_uint("0");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), 0u);

    actual = AKF::StringUtils::convert_to_uint("1");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), 1u);

    actual = AKF::StringUtils::convert_to_uint("01");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), 1u);

    actual = AKF::StringUtils::convert_to_uint("12345");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), 12345u);

    actual = AKF::StringUtils::convert_to_uint(" \t12345 \n\n");
    EXPECT_EQ(actual.has_value(), true);
    EXPECT_EQ(actual.value(), 12345u);

    auto actual_u8 = AKF::StringUtils::convert_to_uint<u8>("255");
    EXPECT(actual_u8.has_value());
    EXPECT_EQ(actual_u8.value(), 255u);
    EXPECT_EQ(sizeof(actual_u8.value()), (size_t)1);
    actual_u8 = AKF::StringUtils::convert_to_uint<u8>("256");
    EXPECT(!actual_u8.has_value());

    auto actual_u16 = AKF::StringUtils::convert_to_uint<u16>("65535");
    EXPECT(actual_u16.has_value());
    EXPECT_EQ(actual_u16.value(), 65535u);
    EXPECT_EQ(sizeof(actual_u16.value()), (size_t)2);
    actual_u16 = AKF::StringUtils::convert_to_uint<u16>("65536");
    EXPECT(!actual_u16.has_value());

    auto actual_u32 = AKF::StringUtils::convert_to_uint<u32>("4294967295");
    EXPECT(actual_u32.has_value());
    EXPECT_EQ(actual_u32.value(), 4294967295ul);
    EXPECT_EQ(sizeof(actual_u32.value()), (size_t)4);
    actual_u32 = AKF::StringUtils::convert_to_uint<u32>("4294967296");
    EXPECT(!actual_u32.has_value());

    auto actual_u64 = AKF::StringUtils::convert_to_uint<u64>("18446744073709551615");
    EXPECT(actual_u64.has_value());
    EXPECT_EQ(actual_u64.value(), 18446744073709551615ull);
    EXPECT_EQ(sizeof(actual_u64.value()), (size_t)8);
    actual_u64 = AKF::StringUtils::convert_to_uint<u64>("18446744073709551616");
    EXPECT(!actual_u64.has_value());
}

TEST_CASE(ends_with)
{
    String test_string = "ABCDEF";
    EXPECT(AKF::StringUtils::ends_with(test_string, "DEF", CaseSensitivity::CaseSensitive));
    EXPECT(AKF::StringUtils::ends_with(test_string, "ABCDEF", CaseSensitivity::CaseSensitive));
    EXPECT(!AKF::StringUtils::ends_with(test_string, "ABCDE", CaseSensitivity::CaseSensitive));
    EXPECT(!AKF::StringUtils::ends_with(test_string, "ABCDEFG", CaseSensitivity::CaseSensitive));
    EXPECT(AKF::StringUtils::ends_with(test_string, "def", CaseSensitivity::CaseInsensitive));
    EXPECT(!AKF::StringUtils::ends_with(test_string, "def", CaseSensitivity::CaseSensitive));
}

TEST_CASE(starts_with)
{
    String test_string = "ABCDEF";
    EXPECT(AKF::StringUtils::starts_with(test_string, "ABC", CaseSensitivity::CaseSensitive));
    EXPECT(AKF::StringUtils::starts_with(test_string, "ABCDEF", CaseSensitivity::CaseSensitive));
    EXPECT(!AKF::StringUtils::starts_with(test_string, "BCDEF", CaseSensitivity::CaseSensitive));
    EXPECT(!AKF::StringUtils::starts_with(test_string, "ABCDEFG", CaseSensitivity::CaseSensitive));
    EXPECT(AKF::StringUtils::starts_with(test_string, "abc", CaseSensitivity::CaseInsensitive));
    EXPECT(!AKF::StringUtils::starts_with(test_string, "abc", CaseSensitivity::CaseSensitive));
}

TEST_CASE(contains)
{
    String test_string = "ABCDEFABCXYZ";
    EXPECT(AKF::StringUtils::contains(test_string, "ABC", CaseSensitivity::CaseSensitive));
    EXPECT(AKF::StringUtils::contains(test_string, "ABC", CaseSensitivity::CaseInsensitive));
    EXPECT(AKF::StringUtils::contains(test_string, "AbC", CaseSensitivity::CaseInsensitive));
    EXPECT(AKF::StringUtils::contains(test_string, "BCX", CaseSensitivity::CaseSensitive));
    EXPECT(AKF::StringUtils::contains(test_string, "BCX", CaseSensitivity::CaseInsensitive));
    EXPECT(AKF::StringUtils::contains(test_string, "BcX", CaseSensitivity::CaseInsensitive));
    EXPECT(!AKF::StringUtils::contains(test_string, "xyz", CaseSensitivity::CaseSensitive));
    EXPECT(AKF::StringUtils::contains(test_string, "xyz", CaseSensitivity::CaseInsensitive));
    EXPECT(!AKF::StringUtils::contains(test_string, "EFG", CaseSensitivity::CaseSensitive));
    EXPECT(!AKF::StringUtils::contains(test_string, "EfG", CaseSensitivity::CaseInsensitive));
    EXPECT(AKF::StringUtils::contains(test_string, "", CaseSensitivity::CaseSensitive));
    EXPECT(AKF::StringUtils::contains(test_string, "", CaseSensitivity::CaseInsensitive));
    EXPECT(!AKF::StringUtils::contains("", test_string, CaseSensitivity::CaseSensitive));
    EXPECT(!AKF::StringUtils::contains("", test_string, CaseSensitivity::CaseInsensitive));
    EXPECT(!AKF::StringUtils::contains(test_string, "L", CaseSensitivity::CaseSensitive));
    EXPECT(!AKF::StringUtils::contains(test_string, "L", CaseSensitivity::CaseInsensitive));
}

TEST_CASE(is_whitespace)
{
    EXPECT(AKF::StringUtils::is_whitespace(""));
    EXPECT(AKF::StringUtils::is_whitespace("   "));
    EXPECT(AKF::StringUtils::is_whitespace("  \t"));
    EXPECT(AKF::StringUtils::is_whitespace("  \t\n"));
    EXPECT(AKF::StringUtils::is_whitespace("  \t\n\r\v"));
    EXPECT(!AKF::StringUtils::is_whitespace("  a "));
    EXPECT(!AKF::StringUtils::is_whitespace("a\t"));
}

TEST_CASE(find)
{
    String test_string = "1234567";
    EXPECT_EQ(AKF::StringUtils::find(test_string, "1").value_or(1), 0u);
    EXPECT_EQ(AKF::StringUtils::find(test_string, "2").value_or(2), 1u);
    EXPECT_EQ(AKF::StringUtils::find(test_string, "3").value_or(3), 2u);
    EXPECT_EQ(AKF::StringUtils::find(test_string, "4").value_or(4), 3u);
    EXPECT_EQ(AKF::StringUtils::find(test_string, "5").value_or(5), 4u);
    EXPECT_EQ(AKF::StringUtils::find(test_string, "34").value_or(3), 2u);
    EXPECT_EQ(AKF::StringUtils::find(test_string, "78").has_value(), false);
}

TEST_CASE(to_snAKFecase)
{
    EXPECT_EQ(AKF::StringUtils::to_snAKFecase("foobar"), "foobar");
    EXPECT_EQ(AKF::StringUtils::to_snAKFecase("Foobar"), "foobar");
    EXPECT_EQ(AKF::StringUtils::to_snAKFecase("FOOBAR"), "foobar");
    EXPECT_EQ(AKF::StringUtils::to_snAKFecase("fooBar"), "foo_bar");
    EXPECT_EQ(AKF::StringUtils::to_snAKFecase("FooBar"), "foo_bar");
    EXPECT_EQ(AKF::StringUtils::to_snAKFecase("fooBAR"), "foo_bar");
    EXPECT_EQ(AKF::StringUtils::to_snAKFecase("FOOBar"), "foo_bar");
    EXPECT_EQ(AKF::StringUtils::to_snAKFecase("foo_bar"), "foo_bar");
    EXPECT_EQ(AKF::StringUtils::to_snAKFecase("FBar"), "f_bar");
    EXPECT_EQ(AKF::StringUtils::to_snAKFecase("FooB"), "foo_b");
}
