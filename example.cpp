/*
This is a very simple example which parses a file path.
*/

#include <iostream>
#include "scanner.h"

int main()
{
    // example one
    // path -> [ a, b, c... ]

    Scanner scan("/a/path/which/leads/to/a/file");

    std::cout << scan.get_source() << " ->\n";

    while(scan)
    {
        auto it = scan.current();

        while (scan.skip_char_outside('/'));

        if(it != scan.current())
        {
            std::cout << it.slice() << "\n";
        }

        scan++;
    }

    // example two
    // filename -> filename + extension

    // reload the source string
    scan = "file.tar.gz";

    auto it = scan.current();

    while (scan.skip_char_outside("."));

    Scanner::string_type filename { it.slice() };
    Scanner::string_type extension;

    while(scan)
    {
        if(!extension.empty())
        {
            filename.append(".");
            filename.append(extension);
        }

        it = scan.current();

        while (scan.skip_char_outside("."));

        extension = it.slice();

        scan++;
    }

    std::cout << "\n" << scan.get_source() << " ->\nfilename: " << filename << "\nextension: " << extension << std::endl;

    return 0;
}
