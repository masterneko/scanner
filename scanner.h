#pragma once

#include <string>
#include <string_view>
#include <vector>

template <typename ValueT>
class BasicScanner
{
public:
    using value_type = ValueT;
    using string_type = typename std::basic_string<value_type>;
    using view_type = typename std::basic_string_view<value_type>;
    using size_type = typename string_type::size_type; /* size_t */
    using cstr_type = typename string_type::const_pointer; /* const char* */

    static constexpr value_type null_value = value_type();

private:
    view_type _source;
    size_type _index = 0;
    mutable std::vector<size_type> _lines; /* maps line number to index */

    void build_line_index() const
    {
        if (!_lines.empty()) return;

        _lines.push_back(0);

        for (size_type i = 0; i < _source.size(); i++)
        {
            if (_source[i] == '\n')
            {
                _lines.push_back(i + 1);
            }
        }
    }

    void index_to_line_column_number(size_type index, size_type& line, size_type& column) const
    {
        build_line_index();

        size_t line_index = 0;

        for (size_t i = 0; i < _lines.size(); i++)
        {
            if (index >= _lines[i])
            {
                line_index = i;
            }
        }

        line = line_index + 1;
        column = index - _lines[line_index] + 1;
    }

public:
    class iterator
    {
    private:
        const BasicScanner<ValueT>* _scan;
        size_type _i;

        iterator(const BasicScanner<ValueT>& s, size_type i)
        :
        _scan(&s),
        _i(i)
        {
        }

        value_type get(size_type index) const
        {
            if (!_scan || index >= _scan->_source.size() || index < 0) return null_value;

            return _scan->_source[index];
        }

    public:
        iterator(const iterator& rhs)
        :
        _scan(rhs._scan),
        _i(rhs._i)
        {
        }

        size_type index() const
        {
            return _i;
        }

        iterator& operator++()
        {
            ++_i;

            return *this;
        }

        iterator operator++(int)
        {
            iterator old = *this;

            ++_i;

            return old;
        }

        iterator& operator--()
        {
            --_i;

            return *this;
        }

        iterator operator--(int)
        {
            iterator old = *this;

            --_i;

            return old;
        }

        bool operator<(const iterator& rhs) const
        {
            return rhs._scan == _scan && _i < rhs._i;
        }

        bool operator<=(const iterator& rhs) const
        {
            return rhs._scan == _scan && _i <= rhs._i;
        }

        bool operator>(const iterator& rhs) const
        {
            return rhs._scan == _scan && _i > rhs._i;
        }

        bool operator>=(const iterator& rhs) const
        {
            return rhs._scan == _scan && _i >= rhs._i;
        }

        bool operator==(const iterator& rhs) const
        {
            return rhs._scan == _scan && _i == rhs._i;
        }

        bool operator!=(const iterator& rhs) const
        {
            return rhs._scan != _scan || _i != rhs._i;
        }

        const value_type operator*() const
        {
            return get(_i);
        }

        bool operator==(const view_type& sequence) const
        {
            for (size_type i = 0; i < sequence.size(); ++i)
            {
                if (get(_i + i) != sequence[i])
                {
                    return false;
                }
            }

            return true;
        }

        bool operator[](view_type char_set)
        {
            value_type current = get(_i);

            for (auto c : char_set)
            {
                if (current == c)
                {
                    return true;
                }
            }

            return false;
        }

        bool operator!=(const view_type& match) const
        {
            return !operator==(match);
        }

        operator bool() const
        {
            return get(_i) != null_value;
        }

        // Retrieves a slice from current scanner index to the index of this iterator
        view_type slice() const
        {
            if (!_scan) return view_type();

            if (_i > _scan->_index)
            {
                return _scan->_source.substr(_scan->_index, _i - _scan->_index);
            }
            
            return _scan->_source.substr(_i, _scan->_index - _i);
        }

        struct location
        {
            size_type index;
            size_type line, column;
        };

        location get_location() const
        {
            size_type line, column;

            _scan->index_to_line_column_number(_i, line, column);

            return { _i, line, column };
        }

        friend BasicScanner<ValueT>;
    };

    BasicScanner(view_type s)
    :
    _source(s)
    {
    }

    BasicScanner(const value_type* s, size_type n)
    :
    _source(s, n)
    {
    }

    BasicScanner()
    {
    }

    BasicScanner<ValueT>& operator=(iterator it)
    {
        _index = it.index();

        return *this;
    }

    BasicScanner<ValueT>& operator=(view_type s)
    {
        _index = 0;
        _source = s;
        _lines.clear();

        return *this;
    }

    iterator current() const
    {
        return iterator(*this, _index);
    }

    iterator peek(size_type how_many = 1) const
    {
        return iterator(*this, _index + how_many);
    }

    iterator operator+=(size_type how_many)
    {
        _index += how_many;

        return iterator(*this, _index);
    }

    iterator operator++()
    {
        return iterator(*this, ++_index);
    }

    iterator operator++(int)
    {
        iterator it = iterator(*this, _index++);

        return it;
    }

    bool skip_char(view_type char_set)
    {
        if (current()[char_set])
        {
            _index++;

            return true;
        }

        return false;
    }

    bool skip_char(value_type c)
    {
        if (*current() == c)
        {
            _index++;

            return true;
        }

        return false;
    }

    bool skip_char(value_type c1, value_type c2)
    {
        value_type a = c1 < c2 ? c1 : c2;
        value_type b = c1 > c2 ? c1 : c2;

        if (*current() >= a && *current() <= b)
        {
            _index++;

            return true;
        }

        return false;
    }

    bool skip_char_outside(view_type char_set)
    {
        if (current() && !current()[char_set])
        {
            _index++;

            return true;
        }

        return false;
    }

    bool skip_char_outside(value_type c)
    {
        if (current() && *current() != c)
        {
            _index++;

            return true;
        }

        return false;
    }

    bool skip_char_outside(value_type c1, value_type c2)
    {
        value_type a = c1 < c2 ? c1 : c2;
        value_type b = c1 > c2 ? c1 : c2;

        if (current() && (*current() < a || *current() > b))
        {
            _index++;

            return true;
        }

        return false;
    }

    bool skip_sequence(view_type sequence)
    {
        if (current() == sequence)
        {
            _index += sequence.length();

            return true;
        }

        return false;
    }

    iterator begin()
    {
        return iterator(*this, 0);
    }

    iterator end()
    {
        return iterator(*this, _source.size());
    }

    iterator at(size_type index)
    {
        if (index >= _source.size())
        {
            throw std::out_of_range("BasicScanner.at(index): index is out of bounds");
        }

        return iterator(*this, index);
    }

    iterator at(size_type line, size_type column)
    {
        build_line_index();

        if (line > _lines.size())
        {
            throw std::out_of_range("BasicScanner.at(line, column): line is out of bounds");
        }

        size_type index = _lines[line - 1] + column - 1;

        if (index >= _source.size())
        {
            throw std::out_of_range("BasicScanner.at(line, column): column is out of bounds");
        }

        return iterator(*this, index);
    }

    operator bool() const
    {
        return *current() != null_value;
    }
};

using Scanner = BasicScanner<std::string::value_type>;

