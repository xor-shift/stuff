#pragma once

#include <exception>

namespace stf {

template<typename T>
struct bad_expected_access;

template<>
struct bad_expected_access<void> : std::exception {
    const char* what() const noexcept override { return "bad_expected_access"; }
};

template<typename E>
struct bad_expected_access : bad_expected_access<void> {
    explicit bad_expected_access(E error)
        : m_error(std::move(error)) {}

    auto error() const& noexcept -> const E& { return m_error; }
    auto error() & noexcept -> E& { return m_error; }
    auto error() const&& noexcept -> const E&& { return std::move(m_error); }
    auto error() && noexcept -> E&& { return std::move(m_error); }

private:
    E m_error;
};

}
