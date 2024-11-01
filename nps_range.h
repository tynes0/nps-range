#pragma once
#ifndef _NPS_RANGE_
#define _NPS_RANGE_

#include <cstdlib>
#include <type_traits>
#include <utility>
#include <cassert>

#if _HAS_CXX17
    #define _NPS_CONSTEXPR17 constexpr 
#else  // _HAS_CXX17
    #define _NPS_CONSTEXPR17
#endif // _HAS_CXX17

#if _HAS_NODISCARD
    #define _NPS_NODISCARD [[nodiscard]]
#else  // _HAS_NODISCARD
    #define _NPS_NODISCARD
#endif // _HAS_NODISCARD

#if !defined(_NPS_ASSERT)
    #if defined(_DEBUG) 
        #define _NPS_ASSERT(cond, message) assert(cond && message)
    #else // _DEBUG
        #define _NPS_ASSERT(cond, message)
    #endif // _DEBUG
#endif //  !defined(_NPS_ASSERT)

#if defined(_MSC_VER) && !_HAS_CXX17
#pragma warning(push)
#pragma warning(disable : 4984)
#endif // !_HAS_CXX17


namespace nps
{
    template <class _Ty, class _Sty, std::enable_if_t<std::is_arithmetic_v<_Ty> && std::is_arithmetic_v<_Sty>, int> = 0>
    class range_iterator
    {
    public:

        _NPS_CONSTEXPR17 range_iterator(_Ty value, _Sty step) : m_value(value), m_step(step) {}

        _NPS_CONSTEXPR17 range_iterator(const range_iterator& other) : m_value(other.m_value), m_step(other.m_step) {}

        _NPS_CONSTEXPR17 range_iterator& operator=(const range_iterator& other)
        {
            m_value = other.m_value;
            m_step = other.m_step;
        }

        _NPS_CONSTEXPR17 range_iterator(range_iterator&& other) : m_value(std::move(other.m_value)), m_step(std::move(other.m_step)) {}

        _NPS_CONSTEXPR17 _Ty operator*() const
        {
            return m_value;
        }

        _NPS_CONSTEXPR17 range_iterator& operator++()
        {
            m_value += static_cast<_Ty>(m_step);
            return (*this);
        }

        _NPS_CONSTEXPR17 range_iterator operator++(int)
        {
            range_iterator temp = *this;
            m_value += static_cast<_Ty>(m_step);
            return temp;
        }

        _NPS_CONSTEXPR17 range_iterator& operator--()
        {
            m_value -= static_cast<_Ty>(m_step);
            return (*this);
        }

        _NPS_CONSTEXPR17 range_iterator operator--(int)
        {
            range_iterator temp = *this;
            m_value -= static_cast<_Ty>(m_step);
            return temp;
        }

        _NPS_CONSTEXPR17 bool operator==(const range_iterator& right) const
        {
            return ((m_step == right.m_step) && (m_step > 0)) ? (m_value >= right.m_value) : (m_value <= right.m_value);
        }

        _NPS_CONSTEXPR17 bool operator!=(const range_iterator& right) const
        {
            return !this->operator==(right);
        }

    private:
        _Ty m_value;
        _Sty m_step;
    };

    template <class _Ty, std::enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
    class range
    {
    public:
        using range_type = _Ty;
        using step_type = std::conditional_t<std::is_integral_v<_Ty>, long long, _Ty>;
        using size_type = step_type;
        using iterator = range_iterator<_Ty, step_type>;

        constexpr range() = default;

        constexpr range(_Ty end)
        {
            reset(0, end);
        }

        constexpr range(_Ty start, _Ty end, step_type step = 1)
        {
            reset(start, end, step);
        }

        constexpr void reset(_Ty start, _Ty end, step_type step = 1) noexcept
        {
#ifdef _DEBUG
            _NPS_ASSERT(step != 0, "step cannot be equal to 0");
#else 
            if (step == 0)
                step = 1;
#endif // _DEBUG

            m_start = start;
            m_end = end;
            if (start <= end)
                m_step = abs(step);
            else
            {
                if (step > 0)
                    m_step = step * (-1);
                else
                    m_step = step;
            }
        }

        constexpr void swap(range& right) noexcept
        {
            if (std::addressof(right) != this)
            {
                std::swap(m_start, right.m_start);
                std::swap(m_end, right.m_end);
                std::swap(m_step, right.m_step);
            }
        }

        _NPS_NODISCARD constexpr iterator begin() noexcept
        {
            return iterator(m_start, m_step);
        }

        _NPS_NODISCARD constexpr iterator begin() const noexcept
        {
            return iterator(m_start, m_step);
        }

        _NPS_NODISCARD constexpr iterator end() noexcept
        {
            return iterator(m_end, m_step);
        }

        _NPS_NODISCARD constexpr iterator end() const noexcept
        {
            return iterator(m_end, m_step);
        }

        _NPS_NODISCARD constexpr range step(step_type new_step) const noexcept
        {
            return range(m_start, m_end, new_step);
        }

        _NPS_NODISCARD constexpr range reverse() const noexcept
        {
            if (m_step < 0)
            {
                return range(m_end + 1, m_start + 1, -m_step);
            }
            return range(m_end - 1, m_start - 1, -m_step);
        }

        _NPS_NODISCARD constexpr _Ty nth_step(size_type n) const noexcept
        {
            return m_start + (m_step * (n - 1));
        }

        _NPS_NODISCARD constexpr size_type size() const noexcept
        {
            size_type result = 0;
            if (m_start == m_end) return result;
            if constexpr (std::is_integral_v<_Ty>)
            {
                result = abs((m_end - m_start) / abs(m_step));
                if (m_step != 1) result += 1;
                return result;
            }
            else if constexpr (std::is_floating_point_v<_Ty>)
            {
                result = ceil(abs((m_end - m_start) / m_step));
                return result;
            }
            return result;
        }

    private:
        _Ty m_start;
        _Ty m_end;
        step_type m_step;
    };

    template <class _Ty>
    void swap(range<_Ty>& lhs, range<_Ty>& rhs)
    {
        lhs.swap(rhs);
    }

    using crange = range<char>;
    using ucrange = range<unsigned char>;
    using srange = range<short>;
    using usrange = range<unsigned short>;
    using irange = range<int>;
    using uirange = range<unsigned int>;
    using lrange = range<long>;
    using ulrange = range<unsigned long>;
    using llrange = range<long long>;
    using ullrange = range<unsigned long long>;
    using frange = range<float>;
    using drange = range<double>;
    using ldrange = range<long double>;
}

#if defined(_MSC_VER) && !_HAS_CXX17
#pragma warning(pop)
#endif // !_HAS_CXX17

#endif // !_NPS_RANGE_
