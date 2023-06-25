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
        auto component = scan.take_while(!scan.match_chars("/"));

        if(!component.empty())
        {
            std::cout << component << "\n";
        }

        scan++;
    }

    // example two
    // filename -> filename + extension

    // reload the source string
    scan = "file.tar.gz";

    Scanner::string_type filename { scan.take_while(!scan.match_chars(".")) };
    Scanner::string_type extension;

    while(scan)
    {
        if(!extension.empty())
        {
            filename.append(".");
            filename.append(extension);
        }

        extension = scan.take_while(!scan.match_chars("."));

        scan++;
    }

    std::cout << "\n" << scan.get_source() << " ->\nfilename: " << filename << "\nextension: " << extension << std::endl;

    return 0;
}
