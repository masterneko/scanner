#include "Tokenizer.h"

enum class Errors {
   NO_VALUE_AFTER_EQUALS
};

void print_error(Errors error, std::string str, size_t character)
{
      switch(error) {
          case Errors::NO_VALUE_AFTER_EQUALS:
//              printf("\nline:%i, on character %i. \x1B[30mError\033[0m\t\t Expected a value after '='\n", get_line_from_char(str, character), (int)character);
              break;
      }
}

int main()
{
std::string input_buf = R"(
#include <cstdio>
int main() {
	printf("Hello World!");
    return 0;
}
)";
	Lexer lexer(input_buf);
	std::vector<Token> tokens = lexer.make_tokens();

	printf("[");
	for(auto &item : tokens)
	{
		printf(" %s", item.get_value().c_str());
		if(&item != &tokens.back())
		{
			printf(",");
		}
	}
	printf(" ]\n");
	return 0;
}
