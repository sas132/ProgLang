#ifndef MAIN
#define MAIN

#include <iostream>
#include <string>
#include <fstream>
#include <stdio.h>
#include <ctype.h>

#define ID 300
#define NUM 301
#define BEGIN 400
#define END 401

class LexerParse
{
private:
	int lookahead;
	int lineCnt;
	int numLexeme;
	std::ifstream input;

public:
	void assignStmt();
	int lexan();
	void AssignStmt();
	void match(int t);
	void readLine(std::string filename);
	int main(int argc, char** argv);
};

#endif
