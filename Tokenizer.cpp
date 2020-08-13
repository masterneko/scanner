#include "Tokenizer.h"

bool char_contains(char c, std::string s)
{
	for(int i = 0; i < s.length(); i++)
	{
		if(s.at(i) == c)
		{
			return true;
		}
	}
	return false;
}

Lexer::Lexer(const std::string text)
{
	m_Text = text;
	advance();
}

void Lexer::advance()
{
	m_Pos++;

	if(m_Pos < m_Text.length())
	{
		m_Current = m_Text.at(m_Pos);
	} else {
		m_Current = 0;
	}
}

Token Lexer::make_number()
{
	std::string num_str = "";
	int dot_count = 0;

	while(char_contains(m_Current, "0123456789."))
	{
		if(m_Current == '.')
		{
			dot_count++;
			num_str += ".";
		} else
		{
			num_str += std::string(1, m_Current);
		}
		advance();

		// Prevent going over
		if(!char_contains(m_Current, "0123456789."))
		{
			m_Pos -= 1;
			m_Current = m_Text.at(m_Pos);
			break;
		}
	}

	if(dot_count > 0)
	{
		Token t(Tokens::Float, num_str);
		return t;
	} else {
		Token t(Tokens::Int, num_str);
		return t;
	}
}

std::string Lexer::make_keyword()
{
	std::string keyword_str = "";
	bool is_str = false;

	if(m_Current == '"')
	{
		is_str = true;
		keyword_str += std::string(1, m_Current);
		advance();
	}

	while(m_Pos < m_Text.length() && m_Current != ' ')
	{

		if(is_str && m_Current == '"')
		{
			break;
		}
		keyword_str += std::string(1, m_Current);
		advance();
		if(char_contains(m_Current, "[]{}+-*/=()<>;"))
		{
			m_Pos -= 1;
			m_Current = m_Text.at(m_Pos);
			break;
		}
	}
	return keyword_str;
}

Token MakeToken(Tokens type, std::string value)
{
	Token t(type, value);
    return t;
}

std::string TokenValue(Tokens t)
{
	return symbol_types[(int)t];
}

std::vector<Token> Lexer::make_tokens()
{
	std::vector<Token> tokens;
	for(int i = 0; i < m_Text.length(); i++)
	{
		if(char_contains(m_Current, " \t\n"))
		{
			advance();
		}
		else if(char_contains(m_Current, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYX"))
		{
			tokens.push_back(MakeToken(Tokens::Keyword, make_keyword()));
			advance();
		}
		else if(char_contains(m_Current, "0123456789"))
		{
			tokens.push_back(make_number());
			advance();
		}
		else if(m_Current == '+')
		{
			if(m_Text.at(m_Pos + 1) == '+')
			{
				tokens.push_back(MakeToken(Tokens::PlusPlus, TokenValue(Tokens::PlusPlus)));
				advance();
			}
			else if(m_Text.at(m_Pos + 1) == '=')
			{
				tokens.push_back(MakeToken(Tokens::PlusEquals, TokenValue(Tokens::PlusEquals)));
				advance();
			} else {
				tokens.push_back(MakeToken(Tokens::Plus, TokenValue(Tokens::Plus)));
			}
			advance();
		}
		else if(m_Current == '-')
		{
			tokens.push_back(MakeToken(Tokens::Minus, TokenValue(Tokens::Minus)));
			advance();
		}
		else if(m_Current == '*')
		{
			tokens.push_back(MakeToken(Tokens::Mul, TokenValue(Tokens::Mul)));
			advance();
		}
		else if(m_Current == '/')
		{
			tokens.push_back(MakeToken(Tokens::Div, TokenValue(Tokens::Div)));
			advance();
		}
		else if(m_Current == '=')
		{
			if(m_Text.at(m_Pos + 1) == '=')
			{
				tokens.push_back(MakeToken(Tokens::EqualsEquals, TokenValue(Tokens::EqualsEquals)));
				advance();
			}
			else {
				tokens.push_back(MakeToken(Tokens::Equals, TokenValue(Tokens::Equals)));
			}
			advance();
		}
		else if(m_Current == '"')
		{
			tokens.push_back(MakeToken(Tokens::Keyword, make_keyword()));
			advance();
		}
		else if(m_Current == ',')
		{
			tokens.push_back(MakeToken(Tokens::Comma, TokenValue(Tokens::Comma)));
			advance();
		}
		else if(m_Current == '&')
		{
			if(m_Text.at(m_Pos + 1) == '&')
			{
				tokens.push_back(MakeToken(Tokens::AndAnd, TokenValue(Tokens::AndAnd)));
				advance();
			}
			else {
				tokens.push_back(MakeToken(Tokens::And, TokenValue(Tokens::And)));
			}
			advance();
		}
		else if(m_Current == '!')
		{
			if(m_Text.at(m_Pos + 1) == '=')
			{
				tokens.push_back(MakeToken(Tokens::NotEqual, TokenValue(Tokens::NotEqual)));
				advance();
			}
			else {
				tokens.push_back(MakeToken(Tokens::Not, TokenValue(Tokens::Not)));
			}
			advance();
		}
		else if(m_Current == '|')
		{
			if(m_Text.at(m_Pos + 1) == '|')
			{
				tokens.push_back(MakeToken(Tokens::PipePipe, TokenValue(Tokens::PipePipe)));
				advance();
			}
			else {
				tokens.push_back(MakeToken(Tokens::Pipe, TokenValue(Tokens::Pipe)));
			}
			advance();
		}
		else if(m_Current == '<')
		{
			if(m_Text.at(m_Pos + 1) == '<')
			{
				tokens.push_back(MakeToken(Tokens::Leftshift, TokenValue(Tokens::Leftshift)));
				advance();
			}
			else {
				tokens.push_back(MakeToken(Tokens::Less, TokenValue(Tokens::Less)));
			}
			advance();
		}
		else if(m_Current == '>')
		{
			if(m_Text.at(m_Pos + 1) == '>')
			{
				tokens.push_back(MakeToken(Tokens::Rightshift, TokenValue(Tokens::Rightshift)));
				advance();
			}
			else {
				tokens.push_back(MakeToken(Tokens::Greater, TokenValue(Tokens::Greater)));
			}
			advance();
		}
		else if(m_Current == '(')
		{
			tokens.push_back(MakeToken(Tokens::LParen, TokenValue(Tokens::LParen)));
			advance();
		}
		else if(m_Current == ')')
		{
			tokens.push_back(MakeToken(Tokens::RParen, TokenValue(Tokens::RParen)));
			advance();
		}
		else if(m_Current == '{')
		{
			tokens.push_back(MakeToken(Tokens::LCurlyBracket, TokenValue(Tokens::LCurlyBracket)));
			advance();
		}
		else if(m_Current == '}')
		{
			tokens.push_back(MakeToken(Tokens::RCurlyBracket, TokenValue(Tokens::RCurlyBracket)));
			advance();
		}
		else if(m_Current == '[')
		{
			tokens.push_back(MakeToken(Tokens::LSquareBracket, TokenValue(Tokens::LSquareBracket)));
			advance();
		}
		else if(m_Current == ']')
		{
			tokens.push_back(MakeToken(Tokens::RSquareBracket, TokenValue(Tokens::RSquareBracket)));
			advance();
		}
		else if(m_Current == '#')
		{
			tokens.push_back(MakeToken(Tokens::Hash, TokenValue(Tokens::Hash)));
			advance();
		}
		else if(m_Current == ';')
		{
			tokens.push_back(MakeToken(Tokens::Semicolon, TokenValue(Tokens::Semicolon)));
			advance();
		} else {
			if(!m_Current == ' ') {
				printf("Lexer error. Undefined token. %c\n", m_Current);
			}
		}
	}

	return tokens;
}
