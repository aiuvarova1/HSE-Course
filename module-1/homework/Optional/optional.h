#include <cstdlib>
#include <type_traits>

#pragma once

namespace task {

    struct NullOpt {
        NullOpt() = default;

        ~NullOpt() = default;
    };

    constexpr NullOpt kNullOpt{};

    struct InPlace {
        explicit InPlace() = default;

        ~InPlace() = default;
    };

    constexpr InPlace kInPlace{};

    template<typename T, bool>
    class OptionalBase {
    public:
        constexpr explicit OptionalBase(NullOpt) : present_(false) {};

        constexpr OptionalBase() : present_(false) {};

        template<class U = T>
        constexpr explicit OptionalBase(U&& value) : value_(std::forward<U>(value)), present_(true) {};

        template<class... Args>
        constexpr explicit
        OptionalBase(InPlace, Args&& ... args) : value_(std::forward<Args>(args)...), present_(true) {};

    protected:
        template<class U>
        void Set(U&& value) {
            value_ = std::forward<U>(value);
            present_ = true;
        }

        void ResetValue() {
            present_ = false;
        }

        T value_;
        bool present_;
    };

    template<typename T>
    class OptionalBase<T, false> {
    public:
        constexpr explicit OptionalBase(NullOpt) : present_(false) {};

        constexpr OptionalBase() : present_(false) {};

        template<class U = T>
        constexpr explicit OptionalBase(U&& value) : value_(std::forward<U>(value)), present_(true) {};

        template<class... Args>
        constexpr explicit
        OptionalBase(InPlace, Args&& ... args) : value_(std::forward<Args>(args)...), present_(true) {};

    protected:

        template<class U>
        void Set(U&& value) {
            if (present_) {
                value_.~T();
            }
            value_ = std::forward<U>(value);
            present_ = true;
        }

        void ResetValue() {
            if (present_) {
                value_.~T();
            }
            present_ = false;
        }

        T value_;
        bool present_;
    };

    template<typename T>
    class Optional : public OptionalBase<T, std::is_trivially_destructible<T>::value> {
    private:
        typedef OptionalBase<T, std::is_trivially_destructible<T>::value> base;
    public:
        using value_type = T;

        constexpr Optional() noexcept;

        template<typename U = value_type>
        constexpr explicit Optional(U&& value);

        constexpr explicit Optional(NullOpt) noexcept;

        template<typename... Args>
        constexpr explicit Optional(InPlace, Args&& ... args);

        Optional& operator=(NullOpt) noexcept;

        template<typename U = T>
        Optional& operator=(U&& value);

        void Reset() noexcept;

        template<typename U>
        constexpr T ValueOr(U&& default_value) const&;

        template<typename U>
        constexpr T ValueOr(U&& default_value)&&;

        constexpr bool HasValue() const noexcept;

        constexpr explicit operator bool() const noexcept;

        constexpr std::add_pointer_t<const value_type> operator->() const;

        constexpr std::add_pointer_t<value_type> operator->();

        constexpr const value_type& operator*() const&;

        constexpr value_type& operator*()&;

        constexpr const value_type&& operator*() const&&;

        constexpr value_type&& operator*()&&;
    };

    template<typename T>
    constexpr Optional<T>::Optional() noexcept : base() {}

    template<typename T>
    template<typename U>
    constexpr Optional<T>::Optional(U&& value) : base(value) {}

    template<typename T>
    constexpr Optional<T>::Optional(NullOpt) noexcept : base(kNullOpt) {}

    template<typename T>
    template<typename... Args>
    constexpr Optional<T>::Optional(InPlace, Args&& ... args) : base(kInPlace, args...) {}

    template<typename T>
    Optional<T>& Optional<T>::operator=(NullOpt) noexcept {
        Reset();
        return *this;
    }

    template<typename T>
    template<typename U>
    Optional<T>& Optional<T>::operator=(U&& value) {
        Set(std::forward<U>(value));
        return *this;
    }

    template<typename T>
    void Optional<T>::Reset() noexcept {
        this->ResetValue();
    }

    template<typename T>
    template<typename U>
    constexpr T Optional<T>::ValueOr(U&& default_value) const& {
        return HasValue() ? **this : static_cast<T>(std::forward<U>(default_value));
    }

    template<typename T>
    template<typename U>
    constexpr T Optional<T>::ValueOr(U&& default_value)&& {
        return HasValue() ? std::move(**this) : static_cast<T>(std::forward<U>(default_value));
    }

    template<typename T>
    constexpr bool Optional<T>::HasValue() const noexcept {
        return this->present_;
    }

    template<typename T>
    constexpr Optional<T>::operator bool() const noexcept {
        return this->present_;
    }

    template<typename T>
    constexpr std::add_pointer_t<const typename Optional<T>::value_type> Optional<T>::operator->() const {
        return &(this->value_);
    }

    template<typename T>
    constexpr std::add_pointer_t<typename Optional<T>::value_type> Optional<T>::operator->() {
        return &(this->value_);
    }

    template<typename T>
    constexpr const typename Optional<T>::value_type& Optional<T>::operator*() const& {
        return this->value_;
    }

    template<typename T>
    constexpr typename Optional<T>::value_type& Optional<T>::operator*()& {
        return this->value_;
    }

    template<typename T>
    constexpr const typename Optional<T>::value_type&& Optional<T>::operator*() const&& {
        return this->value_;
    }

    template<typename T>
    constexpr typename Optional<T>::value_type&& Optional<T>::operator*()&& {
        return this->value_;
    }
}  // namespace task