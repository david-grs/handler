#include "handler.h"

#include <gtest/gtest.h>

#include <vector>
#include <algorithm>
#include <functional>

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

struct handle
{
    template <typename F>
    explicit handle(F f)
        : _deleter(std::forward<F>(f))
    {}
    handle() =default;

    ~handle() noexcept
    {
        if (_deleter)
        {
            _deleter();
            _deleter = nullptr;}
        }
    }

    handle(const handle&) =delete;
    handle& operator=(const handle&) =delete;

private:
    std::function<void()> _deleter;
};

struct handle_test : public ::testing::Test
{
    handle subscribe()
    {
        int allocated_id = _next_id++;
        _subscriptions.push_back(allocated_id);

        return handle([&]()
        {
            _subscriptions.erase(std::remove(std::begin(_subscriptions), std::end(_subscriptions), allocated_id));
        });
    }

protected:
    std::vector<int> _subscriptions;
    int _next_id = 0;
};

TEST_F(handle_test, basic)
{
    {
        auto h = subscribe();
        EXPECT_EQ(1, int(_subscriptions.size()));
    }

    EXPECT_EQ(0, int(_subscriptions.size()));
}
