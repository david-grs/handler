#pragma once

#include <functional>

struct [[nodiscard]] handler
{
    template <typename F>
    explicit handler(F f)
        : _deleter(std::forward<F>(f))
    {}
    handler() =default;

    ~handler() noexcept
    {
        if (_deleter)
        {
            _deleter();
            _deleter = nullptr;
        }
    }

    handler(const handler&) =delete;
    handler& operator=(const handler&) =delete;

    handler(handler&&) =default;
    handler& operator=(handler&&) =default;

private:
    std::function<void()> _deleter;
};



#include <vector>
#include <utility>
#include <algorithm>

template <typename Callback>
struct subscription_list
{
    handler add(Callback callback)
    {
        SubscriptionId allocated_id = _next_id++;
        _subscriptions.emplace_back(allocated_id, std::move(callback));

        return handler([this, allocated_id]()
        {
            _subscriptions.erase(std::remove_if(std::begin(_subscriptions), std::end(_subscriptions),
                                                [&](const auto& p) { return p.first == allocated_id; }));
        });
    }

    template <typename... Args>
    void call(Args&&... args)
    {
        for (auto&& f : _subscriptions)
            f.second(std::forward<Args>(args)...);
    }

    auto size() const { return _subscriptions.size(); }

private:
    using SubscriptionId = std::size_t;

    SubscriptionId _next_id = {};
    std::vector<std::pair<SubscriptionId, Callback>> _subscriptions;
};
