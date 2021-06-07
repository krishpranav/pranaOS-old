

#include <LibTest/TestCase.h>

#include <AKF/CircularDeque.h>
#include <AKF/StdLibExtras.h>
#include <AKF/String.h>

TEST_CASE(enqueue_begin)
{
    CircularDeque<int, 3> ints;

    ints.enqueue_begin(0);
    EXPECT_EQ(ints.size(), 1u);
    EXPECT_EQ(ints.first(), 0);

    ints.enqueue_begin(1);
    EXPECT_EQ(ints.size(), 2u);
    EXPECT_EQ(ints.first(), 1);
    EXPECT_EQ(ints.last(), 0);

    ints.enqueue_begin(2);
    EXPECT_EQ(ints.size(), 3u);
    EXPECT_EQ(ints.first(), 2);
    EXPECT_EQ(ints.last(), 0);

    ints.enqueue_begin(3);
    EXPECT_EQ(ints.size(), 3u);
    EXPECT_EQ(ints.first(), 3);
    EXPECT_EQ(ints.at(1), 2);
    EXPECT_EQ(ints.last(), 1);
}

TEST_CASE(enqueue_begin_being_moved_from)
{
    CircularDeque<String, 2> strings;

    String str { "test" };
    strings.enqueue_begin(move(str));
    EXPECT(str.is_null());
}

TEST_CASE(deque_end)
{
    CircularDeque<int, 3> ints;
    ints.enqueue(0);
    ints.enqueue(1);
    ints.enqueue(2);
    EXPECT_EQ(ints.size(), 3u);

    EXPECT_EQ(ints.dequeue_end(), 2);
    EXPECT_EQ(ints.size(), 2u);

    EXPECT_EQ(ints.dequeue_end(), 1);
    EXPECT_EQ(ints.size(), 1u);

    EXPECT_EQ(ints.dequeue_end(), 0);
    EXPECT(ints.is_empty());
}
