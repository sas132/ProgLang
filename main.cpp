#include "main.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <cstring>

int LexerParse::lexan()
{
	//std::cout << "test1\n";
	std::string varName = "";
	while(true)
	{
		//std::cout << "test2\n";
		//std::cout << line << std::endl;
		char ch;
		input.get(ch);
		//std::cout << "test3\n";
		if(ch == ' ' || ch == '\t')
		{
			//std::cout << "test6\n";
		}
		else if(ch == '\n')
		{
			//std::cout << "test7\n";
			lineCnt++;
		}
		else if(isdigit(ch))
		{
			std::cout << ch << std::endl;
			std::cout << numLexeme << std::endl;
			int numLexeme = atoi(&ch);
			//std::cout << "test5\n\n";
			//std::cout << numLexeme << " should be 1 " << std::endl;

			return NUM;
		}
		else if(isalpha(ch))
		{
			//std::cout << "testA\n";
			char previous = ' ';
			do{
				//std::cout << "testB\t" << ch << "\n";
				varName += ch;
				previous = ch;
				input.get(ch);
			}while(isdigit(ch) || isalpha(ch) || (ch == '_' && previous != '_')); 

			if(begun && strcmp(varName.c_str(), "end") == 0)
			{
				std::cout << "has ended\n\n";
				ended = true;
				return END;
			}

			if(begun && !ended)
			{
				int type = lookup(varName);
				if(type == -1)
				{
					currentVar++;
					insert(varName, ID);
				}
				std::cout << "\t\tvariable: " << varName << "\n";
				return ID;
			}
			else if(strcmp(varName.c_str(), "begin") == 0)
			{
				std::cout << "has begun\n\n";
				begun = true;
				return BEGIN;
			}
			else
			{
				std::cerr << "Error: file does not properly begin.\n";
				return -1;
			}
		}
		else if(ch == EOF)
		{
			if(!ended)
			{
				std::cerr << "Error: file ended improperly. missing 'end.'\n";
			}
			else
				std::cout << "reached end of file.\n";
			return -1; //temp for DONE
		}
		else if(ch == '~')
		{
			std::cout << "comment: ";

			while(ch != '\n')
			{
				std::cout << ch;
				input.get(ch);
			}

			std::cout << "\n";
		}
		else
		{
			std::cout << "neither an alpha, digit, newline, eof, space, or tab.\n" << ch << "\n";
			return ch;
		}
	}
}

void LexerParse::insert(std::string name, int type)
{
	varNames[currentVar] = name;
}

int LexerParse::lookup(std::string value)
{
	int i = 0;

	//std::cout << "made it to lookup\t" << sizeof(varNames) << std::endl;
	while(i < 20)
	{
		std::cout << "\ti = " << i << std::endl;
		if(strcmp(varNames[i].c_str(), value.c_str()) == 0)
		{
			std::cout << "returning " << varTypes[i];
			return varTypes[i];
		}
		i++;		
	}
	std::cout << "never found.\n";
	return -1;
}

void LexerParse::AssignStmt()
{
	// 61 == '='
	
	 match(ID);
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
	// 43 == '+'
	// 45 == '-'

	term();
	while(lookahead == 43 || lookahead == 45)
	{
	 	match(lookahead);
	 	term();
	}
}

void LexerParse::term()
{
	// 42 == '*'
	// 47 == '/'

	 factor();
	 while(lookahead == 42 || lookahead == 47)
	 {
	 	match(lookahead);
	 	factor();
	 }
}

void LexerParse::factor()
{
	// 40 == '('
	// 41 == ')'
	
	if(lookahead == ID)
	{
		match(ID);
	}
	else if(lookahead == NUM)
	{
		match(NUM);
	}
	else if(lookahead == 40)
	{
		match(40);
		expression();
		match(41);
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
		begun = false;
		ended = false;
		currentVar = 0;
		std::cout << "going to lexan\n";
		//std::cout << lexan() << "\n";

		int returnState = 0;
		while(returnState != -1 && returnState != 401)
		{
			returnState = lexan();
			std::cout << "\tlexan: " << returnState << "\n";
		}
	}
	else
	{
		std::cerr << "error opening file.\n";
		return;
	}
}

void LexerParse::setArrays()
{
	for(int i = 0; i < 20; i++)
	{
		varNames[i] = "";
		varTypes[i] = 0;
	}
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
