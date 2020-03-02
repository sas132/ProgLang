#include "main.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <cstring>
#include <iomanip>

int LexerParse::lexan()
{
	std::string varName = "";
	while(true)
	{
		char ch;
		input.get(ch);
		if(ch == ' ' || ch == '\t')
		{
			;
		}
		else if(ch == '\n')
		{
			lineCnt++;
		}
		else if(isdigit(ch))
		{
			int numLexeme = 0;
			char tempChar = input.peek();

			while(isdigit(tempChar))
			{
				input.get(ch);
				numLexeme = (numLexeme * 10) + atoi(&ch);
				tempChar = input.peek();
			}
			insert(std::to_string(numLexeme), NUM);
			return NUM;
		}
		else if(isalpha(ch))
		{
			char peeked = input.peek();
			varName += ch;
			while((isdigit(peeked) || isalpha(peeked) || peeked == '_') && peeked != ',')
			{
				input.get(ch);
				varName += ch;
				peeked = input.peek();
				if(peeked == '_' && ch == '_')
				{
					std::cerr << "Error: two underscores in row in line " << lineCnt << "\n";
					exit(1);
				}
			}

			if(ch == '_')
			{
				std::cerr << "Error: variable ends in underscore in line " << lineCnt << "\n";
				exit(1);
			}

			else if(begun && strcmp(varName.c_str(), "end") == 0)
			{
				ended = true;
				input.close();
				insert("end", END);
				return END;
			}

			else if(begun && strcmp(varName.c_str(), "int") == 0)
			{
				newValue = true;
				return INT;
			}

			else if(begun && !ended)
			{
				int type = lookup(varName);
				if(!newValue && type == -1)
				{
					std::cerr << "Error: varible undeclared in line " << lineCnt << "\n";
					exit(1);
				}
				else if(newValue && type != -1)
				{
					std::cerr << "Error: variable declared twice in line " << lineCnt << "\n";
					exit(1);
				}
				else if(newValue)
				{
					insert(varName, ID);
				}
				else if(strcmp(finalVar.c_str(), "_none") == 0)
				{
					std::cout << "varName: " << varName << "\n";
					finalVar = varName;
					std::cout << "finalVar: " << finalVar << "\n";
				}
				std::cout << "FINALVAR = " << finalVar << "\n";
				return ID;
			}
			else if(strcmp(varName.c_str(), "begin") == 0)
			{
				//insert("begin", BEGIN);
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
			input.close();
			return -1; //temp for DONE
		}
		else if(ch == '~')
		{
			while(ch != '\n')
			{
				input.get(ch);
			}

		}
		else if(ch == ';')
		{
			newValue = false;
			return ch;
		}
		else
		{
			//char tempChar = input.peek();
			//lookahead = tempChar;
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

void LexerParse::newInt()
{
	std::cout << "TEST XYZ\n";
	match(INT);
	while(true)
	{
		//match(INT);
		match(ID);
		if(lookahead == ';')
		{
			match(';');
			return;
		}
		else if(lookahead == ',')
		{
			match(',');
		}
		else
		{
			std::cerr << "Error: lack of semicolon or comma in line " << lineCnt << "\n";
		}
	}
}

int LexerParse::lookup(std::string value)
{
	int i = 0;

	while(i < 20)
	{
		if(strcmp(varNames[i].c_str(), value.c_str()) == 0)
		{
			return varTypes[i];
		}
		i++;		
	}
	return -1;
}

void LexerParse::AssignStmt()
{
	// 61 == '='
	//finalVar = "_none"; // can't start with underscore, good for testing
	
	if(lookahead == 401)
	{
		return;
	}

	if(lookahead != 500)
	{	
		//finalVar = "_none";
		//std::cout << "TESTABC\n";
	 	match(ID);
		std::cout << "finalVar should be a: " << finalVar << "\n";
	 	if(lookahead != 61)
	 	{
			std::cerr << "syntax error, missing '=' in line " << lineCnt << "\n";
		 	exit(1);
	 	}
	 	else
	 	{
			match(lookahead);
			expression();
			match(';');
	 	}
		std::cout << finalVar << "\n";
	}
	else
	{
		newInt();
	}
	//finalVar = "_none";

	//std::cout << finalVar << "\n";
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
		std::cerr << "syntax error. Neither an ID, NUM, or ( in line " << lineCnt << "\n";
		exit(1);
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
		std::cerr << "syntax error. Unable to match in line " << lineCnt << "\n";
		exit(1);
	}
}

void LexerParse::readLine(std::string fileName)
{
	input.open(fileName);

	if(input.is_open())
	{
		finalVar = "_none";
		//finalVar = "_none";
		begun = false;
		ended = false;
		currentVar = 0;
		lexan();
		int temp = 0;
		while(input.is_open())
		{
			std::cout << "TESTABC " << temp << "\n";
			temp++;
			//finalVar = "_none";
			AssignStmt();
			//finalVar = "_none";
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
	std::cout << "| position |    value   | type |\n";
	for(int i = currentVar - 1; i >= 0; i--)
	{
		std::cout << "| ";
		std::cout.width(8);
		std::cout << std::setfill('-') << std::left << i;
		std::cout << " | ";
		std::cout.width(10);
		std::cout << std::setfill('-') << std::left << varNames[i];
		std::cout << " | ";
		std::cout.width(4);
		std::cout << std::setfill('-') << std::left << varTypes[i];
		std::cout << " |\n";
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
