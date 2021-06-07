

#include <LibTest/TestCase.h>

#include <AKF/SourceLocation.h>
#include <AKF/StringView.h>

TEST_CASE(basic_scenario)
{
    auto location = SourceLocation::current();
    EXPECT_EQ(StringView(__FILE__), location.filename());
    EXPECT_EQ(StringView(__FUNCTION__), location.function_name());
    EXPECT_EQ(__LINE__ - 3u, location.line_number());
}

static StringView test_default_arg(const SourceLocation& loc = SourceLocation::current())
{
    return loc.function_name();
}

TEST_CASE(default_arg_scenario)
{
    auto actual_calling_function = test_default_arg();
    auto expected_calling_function = StringView(__FUNCTION__);

    EXPECT_EQ(expected_calling_function, actual_calling_function);
}
