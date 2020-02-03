#include "main.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <ctype.h>

int LexerParse::lexan()
{
	std::cout << "test1\n";
	while(true)
	{
		std::cout << "test2\n";
		//std::cout << line << std::endl;
		char ch;
		input.get(ch);
		std::cout << "test3\n";
		if(ch == ' ' || ch == '\t')
		{
			std::cout << "test6\n";
		}
		else if(ch == '\n')
		{
			std::cout << "test7\n";
			lineCnt++;
		}
		else if(isdigit(ch))
		{
			std::cout << ch << std::endl;
			std::cout << numLexeme << std::endl;
			int numLexeme = atoi(&ch);
			std::cout << "test5\n\n";
			std::cout << numLexeme << " should be 1 " << std::endl;
			return NUM;
		}
		else if(isalpha(ch))
		{
			/*
			 * get identifier into value
			 * type = lookup(value);
			 * if(type == NOT_FOUND)
			 * {
			 *	insert value into symbolTable;
			 * }
			 * else
			 * 	return type;
			 */
		}
		else if(ch == EOF)
		{
			std::cout << "reached end of file.\n";
			return -1; //temp for DONE
		}
		else
		{
			std::cout << "neither an alpha, digit, newline, eof, space, or tab.\n" << ch << "\n";
			return ch;
		}
	}
}

void LexerParse::AssignStmt()
{	
	 match(ID);
	 //61 == '='
	 if(lookahead != 61)
	 {
		 std::cerr << "syntax error";
	 }
	 else
	 {
		match(lookahead);
		expression();
		match(';');
	 }
}

void LexerParse::expression()
{
	term();
	while(lookahead == '+' || lookahead == '-')
	{
	 	match(lookahead);
	 	term();
	}
}

void LexerParse::term()
{
	 factor();
	 while(lookahead == '*' || lookahead == '/')
	 {
	 	match(lookahead);
	 	factor();
	 }
}

void LexerParse::factor()
{
	if(lookahead == ID)
	{
		match(ID);
	}
	else if(lookahead == NUM)
	{
		match(NUM);
	}
	else if(lookahead == '(')
	{
		match('(');
		expression();
		match(')');
	}
	else
	{
		std::cerr << "syntax error. Neither an ID, NUM, or (\n";
	}
}

void LexerParse::match(int t)
{
	if(lookahead == t)
	{
		lookahead = lexan();
	}
	else
	{
		std::cerr << "syntax error. Unable to match\n";
	}
}

void LexerParse::readLine(std::string fileName)
{
	input.open(fileName);

	if(input.is_open())
	{
		std::cout << "going to lexan\n";
		std::cout << lexan() << "\n";
	}
	else
	{
		std::cerr << "error opening file.\n";
		return;
	}

	/*while(!input.eof())
	{
		lexan();
		std::cout << lineCnt << line << "\n";
		getline(input, line);
		//lexan();
	}*/
}

int main(int argc, char** argv)
{
	if(argc == 2)
	{
		LexerParse* parse = new LexerParse();
		parse->readLine(argv[1]);
	}
	else
	{
		std::cerr << "Error. No file name listed.\n";
	}
	return 0;
}
