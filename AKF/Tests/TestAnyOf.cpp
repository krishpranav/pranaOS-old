

#include <LibTest/TestCase.h>

#include <AKF/AnyOf.h>
#include <AKF/Array.h>

TEST_CASE(should_determine_if_predicate_applies_to_any_element_in_container)
{
    constexpr Array<int, 10> a { 1 };

    static_assert(any_of(a.begin(), a.end(), [](auto elem) { return elem == 0; }));
    static_assert(any_of(a.begin(), a.end(), [](auto elem) { return elem == 1; }));
    static_assert(!any_of(a.begin(), a.end(), [](auto elem) { return elem == 2; }));

    EXPECT(any_of(a.begin(), a.end(), [](auto elem) { return elem == 0; }));
    EXPECT(any_of(a.begin(), a.end(), [](auto elem) { return elem == 1; }));
    EXPECT(!any_of(a.begin(), a.end(), [](auto elem) { return elem == 2; }));
}
