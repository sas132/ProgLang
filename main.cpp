#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#define ID 300
#define NUM 301
#define BEGIN 400
#define END 401

int lookahead = 0;
int lineCnt = 0;
int numLexeme = 0;
std::ifstream input;
//std::string line;

void assignStmt()
{

}

int lexan()
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

void readLine(std::string fileName)
{
	input.open(fileName);

	if(input.is_open())
	{
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

int main(int argc, char** argv)
{
	if(argc == 2)
	{
		readLine(argv[1]);
	}
}
