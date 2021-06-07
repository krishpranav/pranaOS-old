

#include <LibTest/TestCase.h>

#include <AKF/NeverDestroyed.h>
#include <AKF/StdLibExtras.h>

struct Counter {
    Counter() = default;

    ~Counter() { ++num_destroys; }

    Counter(const Counter&)
    {
        ++num_copies;
    }

    Counter(Counter&&) { ++num_moves; }

    int num_copies {};
    int num_moves {};
    int num_destroys {};
};

TEST_CASE(should_construct_by_copy)
{
    Counter c {};
    AKF::NeverDestroyed<Counter> n { c };

    EXPECT_EQ(1, n->num_copies);
    EXPECT_EQ(0, n->num_moves);
}

TEST_CASE(should_construct_by_move)
{
    Counter c {};
    AKF::NeverDestroyed<Counter> n { move(c) };

    EXPECT_EQ(0, n->num_copies);
    EXPECT_EQ(1, n->num_moves);
}

TEST_CASE(should_not_destroy)
{
    Counter* c = nullptr;
    {
        AKF::NeverDestroyed<Counter> n {};
        c = &n.get();
    }
    EXPECT_EQ(0, c->num_destroys);
}

TEST_CASE(should_provide_dereference_operator)
{
    AKF::NeverDestroyed<Counter> n {};
    EXPECT_EQ(0, n->num_destroys);
}

TEST_CASE(should_provide_indirection_operator)
{
    AKF::NeverDestroyed<Counter> n {};
    EXPECT_EQ(0, (*n).num_destroys);
}

TEST_CASE(should_provide_basic_getter)
{
    AKF::NeverDestroyed<Counter> n {};
    EXPECT_EQ(0, n.get().num_destroys);
}
