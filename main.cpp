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
			int numLexeme = 0;
			char tempChar = input.peek();
			//input.peek(&tempChar);

			while(isdigit(tempChar))
			{
				input.get(ch);
				numLexeme = (numLexeme * 10) + atoi(&ch);
				tempChar = input.peek();
			}

			//lookahead = tempChar;
			insert(std::to_string(numLexeme), NUM);

			return NUM;
		}
		else if(isalpha(ch))
		{
			//std::cout << "testA\n";
			char peeked = input.peek();
			varName += ch;
			while(isdigit(peeked) || isalpha(peeked) || peeked == '_')
			{
				//std::cout << "testB\t" << ch << "\n";
				input.get(ch);
				varName += ch;
				//previous = ch;
				//input.get(ch);
				peeked = input.peek();
				if(peeked == '_' && ch == '_')
				{
					std::cerr << "Error: two underscores in row.\n";
					exit(1);
				}
			}


			std::cout << "\n\nlast char: " << ch << "\n\n";
			if(ch == '_')
			{
				std::cerr << "Error: variable ends in underscore.\n";
				exit(1);
			}

			else if(begun && strcmp(varName.c_str(), "end") == 0)
			{
				std::cout << "has ended\n\n";
				ended = true;
				input.close();
				return END;
			}

			else if(begun && !ended)
			{
				int type = lookup(varName);
				if(type == -1)
				{
					insert(varName, ID);
				}
				std::cout << "\t\tvariable: " << varName << "\n";
				//lookahead = previous;
				return ID;
			}
			else if(strcmp(varName.c_str(), "begin") == 0)
			{
				std::cout << "has begun\n\n";
				begun = true;
				lookahead = lexan();
				return lookahead;
			}
			else
			{
				std::cerr << "Error: file does not properly begin.\n";
				exit(1);
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
			input.close();
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
			char tempChar = input.peek();
			//input.peek(tempChar);
			lookahead = tempChar;
			return ch;
		}
	}
}

void LexerParse::insert(std::string name, int type)
{
	varNames[currentVar] = name;
	varTypes[currentVar] = type;
	currentVar++;
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
	
	if(lookahead == 401)
	{
		return;
	}
	
	 match(ID);
	 if(lookahead != 61)
	 {
		 std::cerr << "syntax error";
		 exit(1);
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
		exit(1);
	}
}

void LexerParse::match(int t)
{
	std::cout << "matching " << t << " = " << lookahead << "\n";
	if(lookahead == t)
	{
		lookahead = lexan();
	}
	else
	{
		std::cerr << "syntax error. Unable to match in line " << lineCnt << "\n";
		exit(1);
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

		//int returnState = 0;
		lexan();
		while(input.is_open())
		{
			AssignStmt();
		}
		print();
	}
	else
	{
		std::cerr << "error opening file.\n";
		exit(1);
	}
}

void LexerParse::print()
{
	std::cout << "| position | value | type |\n";
	for(int i = currentVar - 1; i >= 0; i--)
	{
		std::cout << "| -------- | ----- | ---- |\n";
		if(i >= 10)
		{
			std::cout << "| " << i << " ----- ";
		}
		else
		{
			std::cout << "| " << i << " ------ ";
		}
		std::cout << "| " << varNames[i] << " ";
		std::cout << "| " << varTypes[i] << "  |\n";
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
		exit(1);
		std::cout << "testing\n";
	}
	return 0;
}
