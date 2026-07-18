#pragma once
#include <variant>
#include <utility>
#include <cassert>

template <typename T, typename E>
class Expected {
public:
    constexpr Expected(const T& val) : data_(val) {}
    constexpr Expected(T&& val) : data_(std::move(val)) {}
    constexpr Expected(const E& err) : data_(err) {}
    constexpr Expected(E&& err) : data_(std::move(err)) {}

    [[nodiscard]] constexpr bool has_value() const noexcept {
        return std::holds_alternative<T>(data_);
    }

    [[nodiscard]] explicit constexpr operator bool() const noexcept {
        return has_value();
    }

    [[nodiscard]] constexpr const T& value() const& {
        assert(has_value() && "bad expected access");
        return std::get<T>(data_);
    }

    [[nodiscard]] constexpr T& value() & {
        assert(has_value() && "bad expected access");
        return std::get<T>(data_);
    }

    [[nodiscard]] constexpr const E& error() const& {
        assert(!has_value() && "bad expected access");
        return std::get<E>(data_);
    }

    [[nodiscard]] constexpr E& error() & {
        assert(!has_value() && "bad expected access");
        return std::get<E>(data_);
    }

    [[nodiscard]] constexpr const T& operator*() const& { return value(); }
    [[nodiscard]] constexpr T& operator*() & { return value(); }
    [[nodiscard]] constexpr const T* operator->() const { return &value(); }
    [[nodiscard]] constexpr T* operator->() { return &value(); }

    template <typename U>
    [[nodiscard]] constexpr T value_or(U&& default_value) const& {
        return has_value() ? std::get<T>(data_) : static_cast<T>(std::forward<U>(default_value));
    }

    template <typename U>
    [[nodiscard]] constexpr T value_or(U&& default_value) && {
        return has_value() ? std::move(std::get<T>(data_)) : static_cast<T>(std::forward<U>(default_value));
    }

private:
    std::variant<T, E> data_;
};

// Specialization for void
template <typename E>
class Expected<void, E> {
public:
    constexpr Expected() : has_value_(true) {}
    constexpr Expected(const E& err) : error_(err), has_value_(false) {}
    constexpr Expected(E&& err) : error_(std::move(err)), has_value_(false) {}

    [[nodiscard]] constexpr bool has_value() const noexcept { return has_value_; }
    [[nodiscard]] explicit constexpr operator bool() const noexcept { return has_value_; }

    [[nodiscard]] constexpr const E& error() const {
        assert(!has_value_ && "bad expected access");
        return error_;
    }

private:
    E error_{};
    bool has_value_;
};
