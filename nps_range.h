/*
 * nps_range.h - Simple and Flexible Range Iterator for C++
 *
 * Contact: Cihan Bilgihan
 * Email: cihanbilgihan@gmail.com
 * GitHub: https://github.com/tynes0
 *
 * License:
 * This project is licensed under the MIT License.
 *
 * The MIT License is a permissive free software license that allows for
 * the reuse of the software within proprietary software, provided
 * that all copies include the original copyright notice and license.
 *
 * This license permits:
 * - Commercial use
 * - Modification
 * - Distribution
 * - Private use
 *
 */

#pragma once
#ifndef _NPS_RANGE_
#define _NPS_RANGE_

#include <cstdlib>
#include <type_traits>
#include <utility>

#include <vector>
#include <list>

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
        #include <cassert>
        #define _NPS_ASSERT(cond, message) assert(cond && message) // You can define and use your own assert to handle conditions.
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
    template <class _Ty, class _Sty, std::enable_if_t<std::is_arithmetic_v<_Ty>&& std::is_arithmetic_v<_Sty>, int> = 0>
    class range_iterator
    {
    public:
        template <class, class>
        friend class circular_range_iterator;
        template <class, class, class>
        friend class patterned_range_iterator;

        _NPS_CONSTEXPR17 range_iterator() = default;

        _NPS_CONSTEXPR17 range_iterator(_Ty value, _Sty step) : m_value(value), m_step(step) {}

        _NPS_CONSTEXPR17 range_iterator(const range_iterator& other) : m_value(other.m_value), m_step(other.m_step) {}

        _NPS_CONSTEXPR17 range_iterator& operator=(const range_iterator& other)
        {
            m_value = other.m_value;
            m_step = other.m_step;
            return *this;
        }

        _NPS_CONSTEXPR17 range_iterator(range_iterator&& other) noexcept : m_value(std::move(other.m_value)), m_step(std::move(other.m_step)) {}

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
        _Ty m_value;    // Current value in the range.
        _Sty m_step;    // Step value for iteration.
    };

    template <class _Ty, class _Sty>
    class circular_range_iterator : public range_iterator<_Ty, _Sty>
    {
    public:
        using base = range_iterator<_Ty, _Sty>;

        circular_range_iterator(_Ty start, _Ty end, _Sty step, long long count = 0)
            : base(start, step), m_start(start), m_end(end), m_max(count), m_count(0) {}

        circular_range_iterator& operator++() noexcept
        {
            base::operator++();
            if(base::m_step > 0 && base::m_value >= m_end)
                base::m_value = m_start;
            else if(base::m_step < 0 && base::m_value <= m_end)
                base::m_value = m_start;
            if (m_max)
                m_count++;
            return *this;
        }

        _NPS_CONSTEXPR17 bool operator==(const circular_range_iterator& right) const
        {
            return m_max == right.m_max && m_max == m_count && m_max != 0;
        }

        _NPS_CONSTEXPR17 bool operator!=(const circular_range_iterator& right) const
        {
            return !this->operator==(right);
        }

    private:
        _Ty m_start; // Beginning of the circular range.
        _Ty m_end;   // End of the circular range.
        long long m_max;
        long long m_count;
    };

    template <class _Ty, class _Sty, class _Rty>
    class patterned_range_iterator : public range_iterator<_Ty, _Sty>
    {
    public:
        using base = range_iterator<_Ty, _Sty>;

        patterned_range_iterator(_Ty start, _Sty step, _Ty(*pattern_func)(_Ty))
            : base(start, step), m_pattern_func(pattern_func) {}

        patterned_range_iterator& operator++() noexcept
        {
            base::m_value = m_pattern_func(base::m_value);
            return (*this);
        }

        _NPS_CONSTEXPR17 bool operator==(const patterned_range_iterator& right) const
        {
            return (m_pattern_func == right.m_pattern_func) && base::operator==(right);
        }

        _NPS_CONSTEXPR17 bool operator!=(const patterned_range_iterator& right) const
        {
            return !this->operator==(right);
        }
    private:
        _Ty(*m_pattern_func)(_Ty);
    };

    template <class _Ty = int, std::enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
    class circular_range
    {
    public:
        using range_type = _Ty;
        using step_type = std::conditional_t<std::is_integral_v<_Ty>, long long, _Ty>;
        using size_type = step_type;
        using iterator = circular_range_iterator<_Ty, step_type>;

        constexpr circular_range() = default;

        constexpr circular_range(_Ty end, long long count = 0)
        {
            reset(0, end, 1, count);
        }

        constexpr circular_range(_Ty start, _Ty end, step_type step, long long count = 0)
        {
            reset(start, end, step, count);
        }

        constexpr void reset(_Ty start, _Ty end, step_type step = 1, long long count = 0) noexcept
        {
#ifdef _DEBUG
            _NPS_ASSERT(step != 0, "step cannot be equal to 0");
#else 
            if (step == 0)
                step = 1;
#endif // _DEBUG

            m_count = count;
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

        _NPS_NODISCARD constexpr iterator begin() const noexcept
        {
            return iterator(m_start, m_end, m_step, m_count);
        }

        _NPS_NODISCARD constexpr iterator end() const noexcept
        {
            return iterator(m_end, m_end, m_step, m_count);
        }
    private:
        _Ty m_start;    // Start value of the range.
        _Ty m_end;      // End value of the range.
        step_type m_step; // Step value for iteration.
        long long m_count;
    };

    template <class _Ty = int, std::enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
    class patterned_range
    {
    public:
        using range_type = _Ty;
        using step_type = std::conditional_t<std::is_integral_v<_Ty>, long long, _Ty>;
        using size_type = step_type;
        using iterator = patterned_range_iterator<_Ty, step_type, _Ty>;

        constexpr patterned_range() = default;

        constexpr patterned_range(_Ty end, _Ty(*pattern_fun)(_Ty) = [](_Ty v)->_Ty { return v++; })
        {
            reset(0, end, pattern_fun);
        }

        constexpr patterned_range(_Ty start, _Ty end, _Ty(*pattern_fun)(_Ty) = [](_Ty v)->_Ty { return v++; })
        {
            reset(start, end, pattern_fun);
        }
        constexpr void reset(_Ty start, _Ty end, _Ty(*pattern_fun)(_Ty) = [](_Ty v)->_Ty { return v++; }) noexcept
        {
            m_start = start;
            m_end = end;
            m_negative = (start > end);
            m_pattern_fun = pattern_fun;
        }

        _NPS_NODISCARD constexpr iterator begin() const noexcept
        {
            return iterator(m_start, m_negative ? static_cast<step_type>(-1) : static_cast<step_type>(1), m_pattern_fun);
        }

        _NPS_NODISCARD constexpr iterator end() const noexcept
        {
            return iterator(m_end, m_negative ? static_cast<step_type>(-1) : static_cast<step_type>(1), m_pattern_fun);
        }
    private:
        _Ty m_start;
        _Ty m_end;
        bool m_negative;
        _Ty(*m_pattern_fun)(_Ty);
    };

    template <class _Ty = int, std::enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
    class range
    {
    public:
        using range_type    = _Ty;
        using step_type     = std::conditional_t<std::is_integral_v<_Ty>, long long, _Ty>;
        using size_type     = step_type;
        using iterator      = range_iterator<_Ty, step_type>;

        constexpr range() = default;

        constexpr range(_Ty end)
        {
            reset(0, end);
        }

        constexpr range(_Ty start, _Ty end, step_type step = 1)
        {
            reset(start, end, step);
        }

        constexpr range reset(_Ty start, _Ty end, step_type step = 1) noexcept
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

            if (m_start == m_end)
                m_step = 0;

            return *this;
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

        _NPS_NODISCARD constexpr range scale(_Ty factor) const noexcept
        {
            return range(m_start * factor, m_end * factor, m_step * factor);
        }

        _NPS_NODISCARD constexpr range intersection(const range& other) const noexcept
        {
            _Ty new_start = std::max(m_start, other.m_start);
            _Ty new_end = std::min(m_end, other.m_end);
            if (new_start >= new_end)
                return range(new_start, new_start);
            return range(new_start, new_end, m_step);
        }

        _NPS_NODISCARD constexpr circular_range<_Ty> circular(long long count = 0) const noexcept
        {
            return circular_range<_Ty>(m_start, m_end, m_step, count);
        }

        _NPS_NODISCARD constexpr patterned_range<_Ty> patterned(_Ty(*pattern_fun)(_Ty)) const noexcept
        {
            return patterned_range<_Ty>(m_start, m_end, pattern_fun);
        }

        _NPS_NODISCARD constexpr range slice(size_type start_index, size_type end_index) const noexcept
        {
            _NPS_ASSERT(start_index <= end_index, "start_index cannot be greater than end_index");
            _Ty new_start = nth_step(start_index + 1);
            _Ty new_end = nth_step(end_index + 1);
            return range(new_start, new_end, m_step);
        }

        _NPS_NODISCARD constexpr range odd() const noexcept
        {
            return range(m_start % 2 == 1 ? m_start : m_start + 1, m_end, 2);
        }

        _NPS_NODISCARD constexpr range even() const noexcept
        {
            return range(m_start % 2 == 0 ? m_start : m_start + 1, m_end, 2);
        }

        _NPS_NODISCARD constexpr _Ty nth_step(size_type n) const noexcept
        {
            return m_start + static_cast<_Ty>(m_step * (n - 1));
        }

        _NPS_NODISCARD constexpr size_type size() const noexcept
        {
            size_type result = 0;
            if (m_start == m_end) return result;
            if constexpr (std::is_integral_v<_Ty>)
            {
                result = abs((m_end - m_start) / abs(m_step));
                return result;
            }
            else if constexpr (std::is_floating_point_v<_Ty>)
            {
                result = ceil(abs((m_end - m_start) / m_step));
                return result;
            }
            return result;
        }

        template <class _Rty, class _Uty, std::enable_if_t<std::is_arithmetic_v<_Uty>, int> = 0>
        constexpr void for_each(_Rty (*func)(_Uty)) const noexcept
        {
            for (_Ty val : (*this))
                (void)func(static_cast<_Uty>(val));
        }

        template <class _Uty, std::enable_if_t<std::is_arithmetic_v<_Uty>, int> = 0>
        _NPS_NODISCARD iterator stop_when(bool (*predicate)(_Uty)) const noexcept
        {
            for (iterator it = begin(); it != end(); ++it)
            {
                if (predicate(static_cast<_Uty>(*it)))
                    return it;
            }
            return end();
        }
        
        template <class _Uty, std::enable_if_t<std::is_arithmetic_v<_Uty>, int> = 0>
        _NPS_NODISCARD constexpr iterator step_while(bool (*predicate)(_Uty)) const noexcept
        {
            iterator it = begin();
            while (it != end() && predicate(static_cast<_Uty>(*it)))
                ++it;
            return it;
        }

        template <class _Uty, std::enable_if_t<std::is_arithmetic_v<_Uty>, int> = 0>
        constexpr bool all_of(bool (*predicate)(_Uty)) const noexcept
        {
            for (auto val : *this)
            {
                if (!predicate(static_cast<_Uty>(val)))
                    return false;
            }
            return true;
        }

        template <class _Uty, std::enable_if_t<std::is_arithmetic_v<_Uty>, int> = 0>
        constexpr bool any_of(bool (*predicate)(_Uty)) const noexcept
        {
            for (auto val : *this)
            {
                if (predicate(static_cast<_Uty>(val)))
                    return true;
            }
            return false;
        }

        template <class _Uty, std::enable_if_t<std::is_arithmetic_v<_Uty>, int> = 0>
        constexpr bool none_of(bool (*predicate)(_Uty)) const noexcept
        {
            return !any_of(predicate);
        }

        constexpr bool contains(_Ty value) const noexcept
        {
            // todo: check
            if (m_step > 0)
                return value >= m_start && value < m_end && ((value - m_start) % m_step == 0);
            return value <= m_start && value > m_end && ((m_start - value) % (-m_step) == 0);
        }

        constexpr bool empty() const noexcept
        {
            return size() == 0;
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

        std::vector<_Ty> to_vector() const
        {
            std::vector<_Ty> result;
            result.reserve(size());
            for (_Ty item : (*this))
                result.emplace_back(item);
            return result;
        }
        
        std::list<_Ty> to_list() const
        {
            std::list<_Ty> result;
            for (_Ty item : (*this))
                result.emplace_back(item);
            return result;
        }

        _NPS_CONSTEXPR17 bool operator==(const range& right) const noexcept
        {
            return m_start == right.m_start && m_end == right.m_end && m_step == right.m_step;
        }

        _NPS_CONSTEXPR17 bool operator!=(const range& right) const noexcept
        {
            return !(*this == right);
        }

        _NPS_NODISCARD constexpr iterator begin() const noexcept
        {
            return iterator(m_start, m_step);
        }

        _NPS_NODISCARD constexpr iterator end() const noexcept
        {
            return iterator(m_end, m_step);
        }

        _NPS_NODISCARD constexpr iterator rbegin() noexcept
        {
            return iterator(static_cast<_Ty>(m_end - m_step), -m_step);
        }

        _NPS_NODISCARD constexpr iterator rend() noexcept
        {
            return iterator(static_cast<_Ty>(m_start - m_step), -m_step);
        }
    private:
        _Ty m_start;    // Start value of the range.
        _Ty m_end;      // End value of the range.
        step_type m_step; // Step value for iteration.
    };

    // Swap function for range objects.
    // @param lhs The left-hand side range to swap.
    // @param rhs The right-hand side range to swap.
    template <class _Ty>
    void swap(range<_Ty>& lhs, range<_Ty>& rhs)
    {
        lhs.swap(rhs);
    }

    // Type aliases for common numeric ranges.
    // These types allow convenient access to common range types for various data types.

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

    template <class _Ty = int, std::enable_if_t<std::is_arithmetic_v<_Ty>, int> = 0>
    constexpr range<_Ty> empty_range = range{};
}

#if defined(_MSC_VER) && !_HAS_CXX17
    #pragma warning(pop)
#endif // !_HAS_CXX17

#endif // !_NPS_RANGE_
