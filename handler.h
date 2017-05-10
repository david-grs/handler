#pragma once

#include <functional>

struct handler
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

private:
    std::function<void()> _deleter;
};
