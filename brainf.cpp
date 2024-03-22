#include <cstdint>
#include <iostream>
#include <vector>
#include "scanner.h"

class BrainFInterpreter
{
private:
    std::vector<uint8_t> _buffer;
    size_t _index;

    void executor(Scanner& scan, Scanner::value_type end_at = Scanner::null_value)
    {
        while(scan)
        {
            if(scan.current() == end_at) break;

            if(scan.skip_char('>'))
            {
                increment_ptr();
            }
            else if(scan.skip_char('<'))
            {
                decrement_ptr();
            }
            else if(scan.skip_char('+'))
            {
                increment_data();
            }
            else if(scan.skip_char('-'))
            {
                decrement_data();
            }
            else if(scan.skip_char('.'))
            {
                print_data();
            }
            else if(scan.skip_char(','))
            {
                receive_data();
            }
            else if(scan.skip_char('['))
            {
                while (scan && _buffer[_index] != 0)
                {
                    executor(scan, ']');
                }

                if (!scan.skip_char(']'))
                {
                    std::cerr << "ErRoR: unterminated []" << std::endl;
                    std::exit(EXIT_FAILURE);
                }
            }
            else if(scan.skip_char(']'))
            {
                while(scan && _buffer[_index] == 0)
                {
                    executor(scan, '[');
                }

                if(!scan.skip_char('['))
                {
                    std::cerr << "ErRoR: unterminated ][" << std::endl;
                    std::exit(EXIT_FAILURE);
                }
            }
            else if(scan.skip_char('#'))
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
    _buffer(512, 0), /* allocate 512 bytes and set to zero */
    _index(0)
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

    BrainFInterpreter& execute(Scanner::view_type source)
    {
        Scanner scan(source);

        executor(scan);

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
