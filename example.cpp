/*
This is a very simple example which parses a file path.
*/

#include <iostream>
#include "scanner.h"

// path -> [ a, b, c... ]
void split_path(std::string_view path)
{
    Scanner scan(path);

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
}

// filename -> filename + extension
void show_extension(std::string_view full_filename)
{
    Scanner scan(full_filename);
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

    std::cout << "\n" << full_filename << " ->\nfilename: " << filename << "\nextension: " << extension << std::endl;
}

int main()
{
    // example one
    const char* path = "/a/path/which/leads/to/a/file";

    std::cout << path << " ->\n";

    split_path(path);

    // example two
    show_extension("file.tar.gz");

    return 0;
}
