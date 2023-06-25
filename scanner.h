#pragma once

#include <string>
#include <string_view>

template <typename CharT>
class BasicScanner
{
public:
    using char_type = CharT;
    using string_type = typename std::basic_string<char_type>;
    using size_type = typename string_type::size_type; /* size_t */
    using difference_type = typename string_type::difference_type; /* signed size_t */
    using cstr_type = typename string_type::const_pointer; /* const char* */
    using strview_type = typename std::basic_string_view<char_type>;

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

        size_type num_of_chars() const
        {
            if(_match.empty() || _is_char_class) return 1;
            
            return _match.length();
        }
    };

    class iterator
    {
    private:
        const BasicScanner<CharT>& _scan;
        size_type _i;

        iterator(const BasicScanner<CharT>& s, size_type i)
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

        friend BasicScanner<CharT>;
    };

    BasicScanner(strview_type s)
    :
    _source(s)
    {
    }

    BasicScanner()
    {
    }

    BasicScanner<CharT>& operator=(strview_type s)
    {
        _source = s;
        _index = 0;

        return *this;
    }

    BasicScanner<CharT>& operator=(iterator it)
    {
        _index = it.index();

        return *this;
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
        return iterator(*this, ++_index);
    }

    iterator operator++(int)
    {
        iterator it = iterator(*this, _index++);

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

    // Matches only if the match text occurs in the scanner buffer
    // An empty string will indicate a wildcard
    static matcher_t match_lexeme(string_type match)
    {
        return matcher_t {
            ._match = match,
            ._is_char_class = false
        };
    }

    // Matches one of any of the match characters
    // An empty string will indicate a wildcard
    static matcher_t match_chars(string_type match)
    {
        return matcher_t {
            ._match = match,
            ._is_char_class = true
        };
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

using Scanner = BasicScanner<std::string::value_type>;

