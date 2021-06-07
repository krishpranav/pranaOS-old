
#include <LibTest/TestCase.h>

#include <AKF/String.h>
#include <AKF/WeAKFPtr.h>
#include <AKF/WeAKFable.h>

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wunused-private-field"
#endif

class SimpleWeAKFable : public WeAKFable<SimpleWeAKFable>
    , public RefCounted<SimpleWeAKFable> {
public:
    SimpleWeAKFable() = default;

private:
    int m_member { 123 };
};

#ifdef __clang__
#    pragma clang diagnostic pop
#endif

TEST_CASE(basic_weAKF)
{
    WeAKFPtr<SimpleWeAKFable> weAKF1;
    WeAKFPtr<SimpleWeAKFable> weAKF2;

    {
        auto simple = adopt_ref(*new SimpleWeAKFable);
        weAKF1 = simple;
        weAKF2 = simple;
        EXPECT_EQ(weAKF1.is_null(), false);
        EXPECT_EQ(weAKF2.is_null(), false);
        EXPECT_EQ(weAKF1.strong_ref().ptr(), simple.ptr());
        EXPECT_EQ(weAKF1.strong_ref().ptr(), weAKF2.strong_ref().ptr());
    }

    EXPECT_EQ(weAKF1.is_null(), true);
    EXPECT_EQ(weAKF1.strong_ref().ptr(), nullptr);
    EXPECT_EQ(weAKF1.strong_ref().ptr(), weAKF2.strong_ref().ptr());
}

TEST_CASE(weAKFptr_move)
{
    WeAKFPtr<SimpleWeAKFable> weAKF1;
    WeAKFPtr<SimpleWeAKFable> weAKF2;

    {
        auto simple = adopt_ref(*new SimpleWeAKFable);
        weAKF1 = simple;
        weAKF2 = move(weAKF1);
        EXPECT_EQ(weAKF1.is_null(), true);
        EXPECT_EQ(weAKF2.is_null(), false);
        EXPECT_EQ(weAKF2.strong_ref().ptr(), simple.ptr());
    }

    EXPECT_EQ(weAKF2.is_null(), true);
}
