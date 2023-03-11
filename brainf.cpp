#include <cstdint>
#include <iostream>
#include <vector>
#include "scanner.h"

class BrainFInterpreter
{
private:
    std::vector<uint8_t> _buffer;
    size_t _index;

    void executor(Scanner scan, Scanner::char_type end_at = '\0')
    {
        while(scan)
        {
            if(scan.current() == end_at) break;

            if(scan.skip(scan.match_chars(">")))
            {
                increment_ptr();
            }
            else if(scan.skip(scan.match_chars("<")))
            {
                decrement_ptr();
            }
            else if(scan.skip(scan.match_chars("+")))
            {
                increment_data();
            }
            else if(scan.skip(scan.match_chars("-")))
            {
                decrement_data();
            }
            else if(scan.skip(scan.match_chars(".")))
            {
                print_data();
            }
            else if(scan.skip(scan.match_chars(",")))
            {
                receive_data();
            }
            else if(scan.skip(scan.match_chars("[")))
            {
                while(scan && _buffer[_index] != 0)
                {
                    executor(scan, ']');
                }

                scan.skip_while(!scan.match_chars("]"));

                if(!scan.skip(scan.match_chars("]")))
                {
                    std::cerr << "ErRoR: unterminated []" << std::endl;
                    std::exit(EXIT_FAILURE);
                }
            }
            else if(scan.skip(scan.match_chars("]")))
            {
                while(scan && _buffer[_index] == 0)
                {
                    executor(scan, '[');
                }

                scan.skip_while(!scan.match_chars("["));

                if(!scan.skip(scan.match_chars("[")))
                {
                    std::cerr << "ErRoR: unterminated ][" << std::endl;
                    std::exit(EXIT_FAILURE);
                }
            }
            else if(scan.skip(scan.match_chars("#")))
            {
                std::clog << "[" << _index << "]: " << (int)_buffer[_index] << std::endl;
            }
            else
            {
                // ignore
                scan++;
            }
        }
    }

public:
    BrainFInterpreter()
    :
    _buffer(512, 0) /* allocate 512 bytes and set to zero */
    {
    }

    void increment_ptr()
    {
        ++_index;

        if(_index >= _buffer.size())
        {
            _buffer.resize(_buffer.size() + 5, 0);
        }
    }

    void decrement_ptr()
    {
        if(_index == 0)
        {
            std::cerr << "RanGe ERr0r!" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        --_index;
    }

    void increment_data()
    {
        ++_buffer[_index];
    }

    void decrement_data()
    {
        --_buffer[_index];
    }

    void print_data() const
    {
        std::cout << _buffer[_index];
    }

    void receive_data()
    {
        std::cin >> _buffer[_index];
    }

    BrainFInterpreter& execute(Scanner::strview_type source)
    {
        executor(source);

        return *this;
    }
};

int main()
{
    BrainFInterpreter brainf;

    brainf.execute("print instructions +[--------->++<]>+.++++++.-[---->+<]>++.-[--->++<]>--.+.[--->+<]>---.---[->++++<]>.-----.[--->+<]>-----.[->+++<]>++.++++.>++++++++++.[->++++++<]>++.[-->+<]>+.-[--->++<]>-.+++++.-----------.[--->+<]>-.[-->+++++++<]>.++++.--.>++++++++++.[->++++++<]>.[-->+<]>++.+[->+++<]>+.+.--.[--->+<]>-.[-->+++++++<]>.++++.--.>++++++++++.+[->++++<]>-.-----------.-[--->++<]>-.+++++.-----------.[--->+<]>-.--[->++++<]>--.[->+++<]>-.+++++++++++.>++++++++++.-[->+++++<]>.[--->++<]>++.+[->+++<]>+.+.--.[--->+<]>-.--[->++++<]>--.[->+++<]>-.+++++++++++.>++++++++++.[--->++<]>-.++.[--->+<]>+.--[->++++<]>-.+[->+++<]>.+.+++.-------.--[--->+<]>-.+.--[->++<]>-.+[-->+<]>+.[-->+++<]>.[--->++<]>.--[->+++<]>+.-[--->+<]>++.+[->+++<]>+.+++++++++++.[--->+<]>-----.-[->+++<]>.>++++++++++.[--->++<]>+.--.-[--->+<]>++.--[->++++<]>-.+[->+++<]>.+.+++.-------.--[--->+<]>-.-[->++<]>-..+[-->+<]>+.[-->+++<]>.[--->++<]>.-[->+++<]>.[--->+<]>+.+[->+++<]>+.+++++++++++.[--->+<]>-----.--[->+++<]>+.>++++++++++.");
    brainf.execute("clear current cell [-]");

    while(true)
    {
        std::string expr;

        std::cout << "> ";

        std::getline(std::cin, expr);

        brainf.execute(expr);

        std::cout << std::endl;
    }

    return 0;
}
