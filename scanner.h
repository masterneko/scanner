#pragma once

#include <string>
#include <string_view>
class Scanner
{
public:
    using string_type = std::string;
    using size_type = string_type::size_type; /* size_t */
    using difference_type = string_type::difference_type; /* signed size_t */
    using char_type = string_type::value_type; /* char */
    using cstr_type = string_type::const_pointer; /* const char* */
    using strview_type = std::basic_string_view<char_type>;

private:
    strview_type _source;
    size_type _index = 0;

public:
    struct matcher_t
    {
        strview_type _match;
        bool _is_char_class;
        bool _true_value = true;

        matcher_t operator!() const
        {
            matcher_t matcher = *this;

            matcher._true_value = !matcher._true_value;

            return matcher;
        }

        size_t num_of_chars() const
        {
            if(_match.empty()) return 1;

            if(_is_char_class)
            {
                return 1;
            }
            
            return _match.length();
        }
    };

    class iterator
    {
    private:
        const Scanner& _scan;
        size_type _i;

        iterator(const Scanner& s, size_type i)
        :
        _scan(s),
        _i(i)
        {
        }

        char_type get(size_type index) const
        {
            if(index >= _scan._source.size() || index < 0) return 0;

            return _scan._source[index];
        }

    public:
        operator char_type() const
        {
            return get(_i);
        }

        size_type line_number() const
        {
            size_type line = 1;

            for(size_type i = 0; i < _i; ++i)
            {
                char_type c = get(i);

                if(c == '\n')
                {
                    ++line;
                }
                else if(c == 0)
                {
                    break;
                }
            }

            return line;
        }

        size_type column_number() const
        {
            size_type column = 1;

            for(size_t i = 0; i < _i; ++i)
            {
                char_type c = get(i);

                if(c == '\n')
                {
                    column = 1;
                }
                else if(c == 0)
                {
                    break;
                }
                else
                {
                    ++column;
                }
            }

            return column;
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

        bool operator==(const iterator& rhs) const
        {
            return &rhs._scan == &_scan && rhs._i == _i;
        }

        bool operator!=(const iterator& rhs) const
        {
            return &rhs._scan != &_scan || rhs._i != _i;
        }

        const char_type operator*() const
        {
            return get(_i);
        }

        bool operator==(const matcher_t& match) const
        {
            char_type current = get(_i);

            if(current == 0)
            {
                return !match._true_value;
            }
            else if(match._match.empty()) /* wildcard */
            {
                return true;
            }

            if(match._is_char_class)
            {
                for(char_type c : match._match)
                {
                    if(current == c)
                    {
                        return match._true_value;
                    }
                }

                return !match._true_value;
            }
            
            for(size_type i = 0; i < match._match.size(); ++i)
            {
                if(get(_i + i) != match._match[i])
                {
                    return !match._true_value;
                }
            }

            return match._true_value;
        }

        bool operator!=(const matcher_t& match) const
        {
            return !operator==(match);
        }

        friend Scanner;
    };

    Scanner(strview_type s)
    :
    _source(s)
    {
    }

    Scanner()
    {
    }

    void set_source(strview_type s)
    {
        _source = s;
        _index = 0;
    }

    strview_type get_source() const
    {
        return _source;
    }

    iterator current()
    {
        return iterator(*this, _index);
    }

    const iterator current() const
    {
        return iterator(*this, _index);
    }

    iterator peek(difference_type how_many = 1)
    {
        return iterator(*this, _index + how_many);
    }

    const iterator peek(difference_type how_many = 1) const
    {
        return iterator(*this, _index + how_many);
    }

    iterator operator+=(difference_type how_many)
    {
        _index += how_many;

        return iterator(*this, _index);
    }

    iterator operator++()
    {
        _index++;

        return iterator(*this, _index);
    }

    iterator operator++(int)
    {
        iterator it = iterator(*this, _index);

        _index++;

        return it;
    }

    // Advances the scanner until the match is no longer true 
    // If the match is inverted then, the number of characters matched is retured
    // else the number occurences of the match found is returned
    size_type skip_while(const matcher_t& match, size_type max = 0)
    {
        size_type n = 0;

        while(current() && current() == match)
        {
            if(max > 0 && n == max) break;

            if(match._true_value == true)
            {
                _index += match.num_of_chars();
            }
            else
            {
                ++_index;
            }

            n++;
        }

        return n;
    }

    // Skips match only once in contrast to skip_while
    // which continously matches
    bool skip(const matcher_t& match)
    {
        if(current() == match)
        {
            if(match._true_value == true)
            {
                _index += match.num_of_chars();
            }
            else
            {
                ++_index;
            }

            return true;
        }

        return false;
    }

    strview_type take_while(const matcher_t& match)
    {
        size_t start_pos = _index;
        
        skip_while(match);
        
        return _source.substr(start_pos, _index - start_pos);
    }

    iterator begin()
    {
        return iterator(*this, 0);
    }

    iterator end()
    {
        return iterator(*this, _source.size());
    }

    operator bool() const
    {
        return *current() != 0;
    }
};


// An empty string will indicate a wildcard
static inline Scanner::matcher_t match_lexeme(Scanner::strview_type match)
{
    return Scanner::matcher_t {
        ._match = match,
        ._is_char_class = false
    };
}

// An empty string will indicate a wildcard
static inline Scanner::matcher_t match_chars(Scanner::string_type match)
{
    return Scanner::matcher_t {
        ._match = match,
        ._is_char_class = true
    };
}
