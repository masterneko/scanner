#include <iostream>
#include <string>
#include <vector>

enum class Tokens {
	Int,
	Float,
	Keyword,
	DoubleQuote,
	Comma,
	And,
	AndAnd,
	Not,
	NotEqual,
	Pipe,
	PipePipe,
	Plus,
	PlusPlus,
	PlusEquals,
	Minus,
	Mul,
    Div,
	Equals,
	EqualsEquals,
	Less,
	Greater,
	Leftshift,
	Rightshift,
	LParen,
	RParen,
	LCurlyBracket,
	RCurlyBracket,
	LSquareBracket,
	RSquareBracket,
	Hash,
	Semicolon,
	Count
};

const std::string symbol_types[] =
{
	"int",
	"float",
	"keyword",
	"\"",
	",",
	"&",
	"&&",
	"!",
	"!=",
	"|",
	"||",
	"+",
	"++",
	"+=",
	"-",
	"*",
	"/",
	"=",
	"==",
	"<",
	">",
	"<<",
	">>",
	"(",
	")",
	"{",
	"}",
	"[",
	"]",
	"#",
	";",
	"\0"
};

class Token {
private:
	Tokens m_Type;
	std::string m_Value;
public:
	std::string get_value() { return m_Value; }
	Tokens get_type() { return m_Type; }

	Token(Tokens type, std::string value) { m_Type = type; m_Value = value; }
};

class Lexer {
private:
	std::string m_Text;
	int m_Pos = -1;
	char m_Current = 0;
	Token make_number();
	std::string make_keyword();

public:
	Lexer(const std::string text);
	void advance();
	std::vector<Token> make_tokens();
};
