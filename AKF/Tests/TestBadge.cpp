

#include <LibTest/TestCase.h>

#include <AKF/Badge.h>

TEST_CASE(should_provide_underlying_type)
{
    static_assert(IsSame<int, Badge<int>::Type>);
}
