#include "main.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <ctype.h>

void LexerParse::assignStmt()
{

}

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
			//do a thing??
		}
		else if(ch == EOF)
		{
			return -1; //temp for DONE
		}
		else
		{
			return ch;
		}
	}
}

void LexerParse::AssignStmt()
{
	
	 match(ID);
	 if(lookahead != '=')
	 {
		 std::cerr << "syntax error";
	 }
	 else
	 {
		match(lookahead);
		//expression
		match(';');
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
		std::cerr << "syntax error";
	}
}

void LexerParse::readLine(std::string fileName)
{
	input.open(fileName);

	if(input.is_open())
	{
		std::cout << "going to lexan\n";
		lexan();
	}
	else
	{
		std::cout << "error opening " << fileName << "\n";
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

int LexerParse::main(int argc, char** argv)
{
	if(argc == 2)
	{
		readLine(argv[1]);
	}
}
