/*
This is a very simple example which parses a file path.
*/

#include <iostream>
#include "lexer.h"

int main()
{
    // example one
    // path -> [ a, b, c... ]

    std::string s = "/a/path/which/leads/to/a/file";
    lexer lex(s);

    std::cout << s << " ->\n";

    while(lex)
    {
        // split is much like strtok and takes in a delimiter
        std::string component = lex.split("/");

        if(!component.empty())
        {
            std::cout << component << "\n";
        }

        // advancing is not required here because split does it for us
        // lex.advance();
    }

    // example two
    // filename -> filename + extension

    // reload the source string
    s = "file.tar.gz";
    lex.set_source(s);

    std::string filename = lex.split(".");
    std::string extension;

    while(lex)
    {
        if(!extension.empty())
        {
            filename.append(".");
            filename.append(extension);
        }

        extension = lex.split(".");
    }

    std::cout << "\n" << s << " ->\nfilename: " << filename << "\nextension: " << extension << std::endl;

    return 0;
}
