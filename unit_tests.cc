#include "handler.h"

#include <gtest/gtest.h>

#include <vector>
#include <iostream>

struct handler_test : public ::testing::Test
{
	handler subscribe()
	{
		++_subscriptions;

		return handler([=]()
		{
			--_subscriptions;
		});
	}

protected:
	int _subscriptions = 0;
};

TEST_F(handler_test, basic)
{
	{
		auto h = subscribe();
		EXPECT_EQ(1, _subscriptions);
	}

	EXPECT_EQ(0, _subscriptions);
}

TEST_F(handler_test, move)
{
	{
		handler moved;

		{
			auto h = subscribe();
			moved = std::move(h);
		}

		EXPECT_EQ(1, _subscriptions);
	}

	EXPECT_EQ(0, _subscriptions);
}

TEST_F(handler_test, dtor)
{
	{
		auto h = subscribe();
	}

	EXPECT_EQ(0, _subscriptions);
}

TEST_F(handler_test, multiple)
{
	handler h2;

	{
		auto h1 = subscribe();
		h2 = subscribe();
		EXPECT_EQ(2, _subscriptions);
	}

	EXPECT_EQ(1, _subscriptions);
}



struct subscription_list_test : public ::testing::Test
{
	using Callback = std::function<void(int)>;

	handler subscribe(Callback callback)
	{
		return _subscriptions.add(callback);
	}

protected:
	subscription_list<Callback> _subscriptions;
};

TEST_F(subscription_list_test, basic)
{
	int sum = 0;

	{
		auto h = subscribe([&](int i) { sum += i; } );
		EXPECT_EQ(1, int(_subscriptions.size()));
	}

	EXPECT_EQ(0, int(_subscriptions.size()));
	EXPECT_EQ(0, sum);
}

TEST_F(subscription_list_test, call)
{
	int sum = 0;

	auto h = subscribe([&](int i) { sum += i; } );
	_subscriptions.call(1);
	_subscriptions.call(3);
	_subscriptions.call(5);

	EXPECT_EQ(9, sum);
}


TEST_F(subscription_list_test, call_after_destruction)
{
	int sum = 0;

	{
		auto h = subscribe([&](int i) { sum += i; } );
		_subscriptions.call(1);
	}

	_subscriptions.call(3);
	_subscriptions.call(5);

	EXPECT_EQ(1, sum);
}

TEST_F(subscription_list_test, discard_return)
{
	int sum = 0;

	subscribe([&](int i) { sum += i; } );
	_subscriptions.call(1);
	_subscriptions.call(3);
	_subscriptions.call(5);

	EXPECT_EQ(0, sum);
}
