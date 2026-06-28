#pragma once
#include <variant>
#include <utility>
#include <stdexcept>

template <typename T, typename E>
class Expected {
public:
    Expected(const T& val) : data_(val) {}
    Expected(T&& val) : data_(std::move(val)) {}
    Expected(const E& err) : data_(err) {}
    Expected(E&& err) : data_(std::move(err)) {}

    bool has_value() const noexcept {
        return std::holds_alternative<T>(data_);
    }

    explicit operator bool() const noexcept {
        return has_value();
    }

    const T& value() const& {
        if (!has_value()) throw std::runtime_error("bad expected access");
        return std::get<T>(data_);
    }

    T& value() & {
        if (!has_value()) throw std::runtime_error("bad expected access");
        return std::get<T>(data_);
    }

    const E& error() const& {
        if (has_value()) throw std::runtime_error("bad expected access");
        return std::get<E>(data_);
    }

    E& error() & {
        if (has_value()) throw std::runtime_error("bad expected access");
        return std::get<E>(data_);
    }

    const T& operator*() const& { return value(); }
    T& operator*() & { return value(); }
    const T* operator->() const { return &value(); }
    T* operator->() { return &value(); }

private:
    std::variant<T, E> data_;
};

// Specialization for void
template <typename E>
class Expected<void, E> {
public:
    Expected() : has_value_(true) {}
    Expected(const E& err) : error_(err), has_value_(false) {}
    Expected(E&& err) : error_(std::move(err)), has_value_(false) {}

    bool has_value() const noexcept { return has_value_; }
    explicit operator bool() const noexcept { return has_value_; }

    const E& error() const {
        if (has_value_) throw std::runtime_error("bad expected access");
        return error_;
    }

private:
    E error_{};
    bool has_value_;
};
