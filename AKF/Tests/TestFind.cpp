

#include <LibTest/TestCase.h>

#include <AKF/Array.h>
#include <AKF/Find.h>
#include <AKF/Vector.h>

TEST_CASE(should_return_end_if_not_in_container)
{
    constexpr Array<int, 10> a {};

    static_assert(a.end() == AKF::find(a.begin(), a.end(), 1));

    EXPECT(a.end() == AKF::find(a.begin(), a.end(), 1));
}

TEST_CASE(should_return_iterator_to_first_matching_value_in_container)
{
    static constexpr Array<int, 10> a { 1, 2, 3, 4, 0, 6, 7, 8, 0, 0 };

    constexpr auto expected = a.begin() + 4;

    static_assert(expected == AKF::find(a.begin(), a.end(), 0));

    EXPECT(expected == AKF::find(a.begin(), a.end(), 0));
}

TEST_CASE(should_return_iterator_to_first_predicate_matching_value_in_container)
{
    static constexpr Array<int, 10> a { 1, 2, 3, 4, 0, 6, 7, 8, 0, 0 };

    constexpr auto expected = a.begin() + 4;

    static_assert(expected == AKF::find_if(a.begin(), a.end(), [](auto v) { return v == 0; }));

    EXPECT(expected == AKF::find_if(a.begin(), a.end(), [](auto v) { return v == 0; }));

    auto find_me = 8;
    EXPECT(find_me == *AKF::find_if(a.begin(), a.end(), [&](auto v) { return v == find_me; }));
}

TEST_CASE(should_return_index_to_first_predicate_matching_value_in_container)
{
    static constexpr Array<int, 10> a { 1, 2, 3, 4, 0, 6, 7, 8, 0, 0 };

    static_assert(4 == AKF::find_index(a.begin(), a.end(), 0));

    EXPECT(4 == AKF::find_index(a.begin(), a.end(), 0));
}
