#include "handler.h"

#include <gtest/gtest.h>

#include <vector>
#include <algorithm>
#include <iostream>

struct handler_test : public ::testing::Test
{
    handler subscribe()
    {
        int allocated_id = _next_id++;
        _subscriptions.push_back(allocated_id);

        return handler([this, allocated_id]()
        {
            std::cout << "removing id " << allocated_id << std::endl;
            _subscriptions.erase(std::remove(std::begin(_subscriptions), std::end(_subscriptions), allocated_id));
        });
    }

protected:
    std::vector<int> _subscriptions;
    int _next_id = 0;
};

TEST_F(handler_test, basic)
{
    {
        auto h = subscribe();
        EXPECT_EQ(1, int(_subscriptions.size()));
    }

    EXPECT_EQ(0, int(_subscriptions.size()));
}

TEST_F(handler_test, move)
{
    {
        handler moved;

        {
            auto h = subscribe();
            moved = std::move(h);
        }

        EXPECT_EQ(1, int(_subscriptions.size()));
    }

    EXPECT_EQ(0, int(_subscriptions.size()));
}

TEST_F(handler_test, dtor)
{
    {
        auto h = subscribe();
        h.~handler();
    }

    EXPECT_EQ(0, int(_subscriptions.size()));
}

TEST_F(handler_test, multiple)
{
    handler h2;

    {
        auto h1 = subscribe();
        h2 = subscribe();
        EXPECT_EQ(2, int(_subscriptions.size()));
    }

    EXPECT_EQ(1, int(_subscriptions.size()));
    EXPECT_EQ(1, _subscriptions.back());
}
