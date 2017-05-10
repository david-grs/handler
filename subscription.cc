#include "subscription.h"

#include <gtest/gtest.h>

struct A
{
    A() { default_constructions++; }
    A(const A&) { copy_constructions++; }
    A(A&&) { move_constructions++; }
    ~A() { destructions++; }

    static void reset()
    {
        default_constructions = 0;
        copy_constructions = 0;
        move_constructions = 0;
        destructions = 0;
    }

    static int default_constructions;
    static int copy_constructions;
    static int move_constructions;
    static int destructions;
};

int A::default_constructions = 0;
int A::copy_constructions = 0;
int A::move_constructions = 0;
int A::destructions = 0;

TEST(subscription, basic)
{

}
